#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>
#include<algorithm>
#include <iostream>
#include<string>
#include"datanode_manager.h"
#include"NameNode.h"
#include "datanode_manager.cc"
#include"common.h"

using namespace grpc;
#define HeartbeatSleep 35 //小于 选举时间才能压制
//本机

struct Request{
   int serverindex;
   ServiceImpl* ts;
};
ServiceImpl::ServiceImpl(int index)
{
   GF=new GetFiles("/");
   // for(int i=8083;i<8200;i++)
   // {
   //    vport.push_back(i);
   // }
   //日志初始化
   el::Configurations conf("log.conf");
   el::Loggers::reconfigureAllLoggers(conf);
   
   //master三副本
   for(int i=0;i<3;i++)
   {
      std::shared_ptr<grpc::Channel> channel =grpc::CreateChannel("localhost:900"+to_string(i), grpc::InsecureChannelCredentials());
       // 创建一个stub
      std::shared_ptr<Rpc::Cli2Master::Stub> stub = Rpc::Cli2Master::NewStub(channel);
      _Stub.push_back(stub);
   }
  
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mu,NULL);
   _myindex=index;
   status = Follower;
   votedFor = -1;
   currentTerm = 0;
	voteNum = 0;
   lastApplied = 0;//第一个日志不需要同步
	commitIndex = 0;
   sleep(3);
   gettimeofday(&votedTimer,NULL);
   hanlde=1010;
    logs.resize(0);
   logs.push_back(Rpc::LogEntry{});
   nextIndex.resize(3,1);//默认3个
   matchIndex.resize(3);
   pthread_t tid;//test
   int ret = pthread_create(&tid, NULL, electionTicker, (void*)this);
   if(ret < 0)
   {
      std::cout<<"create electionTicker error"<<std::endl;
      exit(1);
   }
   ret = pthread_create(&tid, NULL,appendTicker, (void*)this);
   if(ret < 0)
   {
      std::cout<<"create appendTicker error"<<std::endl;
      exit(1);
   }
   //test 
   // ret = pthread_create(&tid, NULL,isleader, (void*)this);
   // if(ret < 0)
   // {
   //    std::cout<<"create isleader error"<<std::endl;
   //    exit(1);
   // }
   DataNodeManager::GetInstance().StartHeartbeat();

   LOG(INFO) << "***** init success  *****";
}

ServiceImpl::~ServiceImpl()
{

   delete GF;
   pthread_mutex_destroy(&mu);
   pthread_cond_destroy(&cond);
}

// void* ServiceImpl::isleader(void* arg)
// {
//    auto ts=(ServiceImpl*) arg;
//    while(true)
//    {
//        sleep(5);
//       if(ts->status==Leader)
//       {
//          std::cout<<"i am a leader"<<std::endl;
//       }
//    }
// }
void* ServiceImpl::appendTicker(void* arg)
{
   auto ts=(ServiceImpl*) arg;
   pthread_detach(pthread_self());
   while(true)
   {
      usleep(40*1000);
		if (ts->status == Leader) {
         //   std::cout<<"leader run"<<std::endl;
			leaderAppendEntries(ts);
		}
    
			//不是leader不能append

   }  
}
int count1=0;
void* ServiceImpl::_leaderAppendEntries(void* arg)
{
   pthread_detach(pthread_self());
   auto r=(Request*) arg;
   auto ts=r->ts;
    if (ts->status != Leader)
    {
      delete r;
			return 0;
	 }
   auto index=r->serverindex;
   //锁竞争一直饥饿
   pthread_mutex_lock(&ts->mu);
   
    //todo 快照 不做
    //installSnapshot，如果rf.nextIndex[i]-1小于等lastIncludeIndex,说明followers的日志小于自身的快照状态，将自己的快照发过去
			// 同时要注意的是比快照还小时，已经算是比较落后
			// if rf.nextIndex[server]-1 < rf.lastIncludeIndex {
			// 	go rf.leaderSendSnapShot(server)
			// 	rf.mu.Unlock()
			// 	return
			// }
   int prevLogIndex=getLastIndex(ts);
   int prevLogTerm=ts->logs[ts->logs.size()-2].term();
   //leader的最大小标大于等于了需要发送的下标
   Rpc::AppendEntriesArgs args;
   args.set_aterm(ts->currentTerm);
   args.set_leaderid(ts->_myindex);

   args.set_prevlogindex(ts->nextIndex[index]);
   args.set_prevlogterm(ts->logs[ts->nextIndex[index]-1].term());

   args.set_leadercommit(ts->commitIndex);
   int s=0;
   if (getLastIndex(ts) >= ts->nextIndex[index]) 
   {
         std::vector<Rpc::LogEntry> Entries(ts->logs.begin()+ts->nextIndex[index],ts->logs.end());
         s=Entries.size();
         for(size_t i=0;i<Entries.size();i++)
         {
            
            auto log=args.add_entries();
            log->set_term(Entries[i].term());
            log->set_command(Entries[i].command());
            log->set_filepath(Entries[i].filepath());
            log->set_src(Entries[i].src());
            log->set_des(Entries[i].des());
            log->set_size(Entries[i].size());
            // std::cout<<Entries[i].term()<<":"<<Entries[i].command()<<":"<<Entries[i].mkdir_filepath()<<std::endl;
         }
	} 
      else{
         //心跳 没发请求
      }
   Rpc::AppendEntriesReply  reply; 
	 

   
   grpc::ClientContext context;
   //usleep(10);
   gettimeofday(&ts->votedTimer,NULL);
   pthread_mutex_unlock(&ts->mu);
   ts-> _Stub[index]->AppendEntries(&context,args, &reply);
  
   if(ts->status != Leader) {
        delete r;
				return 0;
	}

    mylock_guard l(&ts->mu);
   //  std::cout<<reply.appendentriesstate()<<std::endl;
   switch (reply.appendentriesstate()){
      case 0:
      //正常
      // std::cout<<"正常"<<std::endl;
      // std::cout<<reply.success()<<reply.term()<<ts->currentTerm<<s<<std::endl;
      if (reply.success() && reply.term() == ts->currentTerm && ts->appendNums < 3/2 &&s!=0)
      {
         // std::cout<<"++"<<std::endl;
				ts->appendNums++;
		}
			// 说明返回的值已经大过了自身数组
		if((size_t )ts->nextIndex[index] > ts->logs.size()+1) 
      {
			return 0;
		}
        
		ts->nextIndex[index] += args.entries().size();

		if(ts->appendNums >= 3/2) 
      {
         
				// 保证幂等性，不会提交第二次
			ts->appendNums = 0;
         if (ts->logs.size() == 0 || ts->logs[ts->logs.size()-1].term() != ts->currentTerm) 
         {
               delete r;
					return 0;
			}
        
         int size=ts->logs.size();
         // std::cout<<size<<std::endl;
         //减去首个日志
			for (;ts->lastApplied <size-1;) 
         {
            // for(auto& x:ts->logs)
            // {
            //    std::cout<<x.term()<<x.command()<<std::endl;
            // }
            // std::cout<<ts->lastApplied<<":"<<size<<std::endl;
            // std::cout<<ts->lastApplied<<":"<<size-1<<std::endl;
				ts->lastApplied++;
               //server todo
               if(ts->logs[ts->lastApplied].command()==4)
               {
                     //mkdir
                     // std::cout<<"123"<<std::endl;
                     // _Mkdir(ts,ts->logs[ts->lastApplied].filepath());
                     ts->mkdir=true;
               }
               else if(ts->logs[ts->lastApplied].command()==3)
               {
                  // _Delete(ts,ts->logs[ts->lastApplied].filepath());
                  ts->del=true;
               }
               else if(ts->logs[ts->lastApplied].command()==2)
               {
                  // _Rename(ts,ts->logs[ts->lastApplied].src(),ts->logs[ts->lastApplied].des());
                   ts->rename=true;
               }
               else if(ts->logs[ts->lastApplied].command()==1)
               {
                  //_GetWriteAddr(ts,ts->logs[ts->lastApplied].filepath(),ts->logs[ts->lastApplied].size());
                 
                  ts->isgetw=true;
               }
               else 
               {
                  //command=0的首个日志不用执行
                  //  std::cerr<<"error"<<std::endl;
               }
            // std::cout<<"leader todo"<<std::endl;
            
               ts->commitIndex = ts->lastApplied;
			
					//fmt.Printf("[	sendAppendEntries func-rf(%v)	] commitLog  \n", rf.me)
			}
      }
      gettimeofday(&ts->votedTimer,NULL);
         break;
      case 1:
      ts->status = Follower;
		ts->votedFor = -1;
		gettimeofday(&ts->votedTimer,NULL);
		ts->currentTerm = reply.term();
      break;
      case 2:
      if(args.aterm() != ts->currentTerm) {
			return 0;
		}
		ts->nextIndex[index] = reply.upnextindex();
      break;
      case 3:
      if(args.aterm() != ts->currentTerm) {
			return 0;
		}
		ts->nextIndex[index] = reply.upnextindex();
      break;
      default:
      LOG(WARNING) << "***** warning log  command *****";
      break;
   
   };
    delete r;
//    if (reply.Term > ts->currentTerm)
//     {
//       //成为从
// 			ts->currentTerm = reply.Term;
// 			ts->status = Follower;
// 			ts->votedFor = -1;
// 			ts->voteNum = 0;
//          gettimeofday(&ts->votedTimer,NULL);
// 			return;
// 	}

//    if (reply.Success) 
//    {
// 			// ts->commitIndex = ts->lastIncludeIndex;
// 			ts->matchIndex[index] = args.PrevLogIndex + len(args.Entries);
// 			ts->nextIndex[index] = rf.matchIndex[index] + 1;
// 			// 外层遍历下标是否满足,从快照最后开始反向进行
// 			for (int _index = getLastIndex(ts); _index >= ts->lastIncludeIndex+1; index--)
//           {
// 				sum := 0
// 				for (int i = 0; i < 3; i++) {
// 					if( i == index)
//                 {
// 						sum += 1
// 						continue
// 					}
// 					if(ts->matchIndex[i] >= index)
//                 {
// 							sum += 1
// 					}
// 			      }

// 						// 大于一半，且因为是从后往前，一定会大于原本commitIndex
// 						if sum >= len(rf.peers)/2+1 && rf.restoreLogTerm(index) == rf.currentTerm 
//                   {
// 							rf.commitIndex = index
// 							break
// 						}

// 				}
// } else { // 返回为冲突
// 					// 如果冲突不为-1，则进行更新
// 					if reply.UpNextIndex != -1
//                {
// 						rf.nextIndex[server] = reply.UpNextIndex
// 					}
// 				}
			
}
grpc::Status ServiceImpl::AppendEntries(grpc::ServerContext*  context,const Rpc::AppendEntriesArgs* args,Rpc::AppendEntriesReply* reply)
{
   //在占用锁时一直调度选举
   gettimeofday(&votedTimer,NULL);
   // std::cout<<"更新心跳"<<Getms(&votedTimer)<<std::endl;
   // pthread_cond_signal(&cond);
   mylock_guard l(&mu);

   if (args->aterm() < currentTerm) {
        LOG(INFO) << "***** leader任期小变从 *****";
		reply->set_term(currentTerm);//error 
      reply->set_appendentriesstate(1);
		reply->set_success(false);
		// reply->set_UpNextIndex(-1);
		return Status::OK;
	}
   int tmps=logs.size();//1
   if (args->prevlogindex() > 0 && (tmps < args->prevlogindex() || logs[args->prevlogindex()-1].term() != args->prevlogterm())) {
      //对比日志
      // std::cout<<tmps<<":"<<args->prevlogindex()<<":"<<logs[args->prevlogindex()-1].term()<<":"<<args->prevlogterm()<<std::endl;
		reply->set_appendentriesstate(3);
		reply->set_term(currentTerm);
		reply->set_success(false);
		reply->set_upnextindex(lastApplied + 1);
		return Status::OK;
	}

   //提交过了
   if(args->prevlogindex()!=-1 && lastApplied > args->prevlogindex()) {
		//已经提交过该日志了 
      reply->set_appendentriesstate(2);
		reply->set_term(currentTerm);
		reply->set_success(false);
		reply->set_upnextindex(lastApplied + 1);
     
		return Status::OK;
	}

   //重置
   currentTerm = args->aterm();
	votedFor = args->leaderid();
	status = Follower;


	// 对返回的reply进行赋值
	reply->set_appendentriesstate(0);
	reply->set_term(currentTerm);
	reply->set_success(true);

   if (args->entries().size()!=0) {
      //切割
		logs =std::vector<Rpc::LogEntry>(logs.begin(),logs.begin()+args->prevlogindex());//+1
      for(auto& x:args->entries())
      logs.push_back(x);
	}
   int i=0;
   for(;lastApplied < args->leadercommit();) 
   {

      lastApplied++;
       LOG(INFO) << "***** 执行日志  *****";
      //  if(logs[lastApplied].command()==0)
      //  {
      //     std::cout<<"初始日志跳过"<<std::endl;
      //  }
      //提交执行
      //  auto log=args.add_entries();
      // log->set_term(Entries[i].term());
      // log->set_command(Entries[i].command());
      // log->set_mkdir_filepath(Entries[i].mkdir_filepath());
      if(logs[lastApplied].command()==4)
      {
         //mkdir
         _Mkdir(this,logs[lastApplied].filepath());
      }
      else if(logs[lastApplied].command()==3)
      {
         _Delete(this,logs[lastApplied].filepath());
      }
      else if(logs[lastApplied].command()==2)
      {
          _Rename(this,logs[lastApplied].src(),logs[lastApplied].des());
      }
      else if(logs[lastApplied].command()==1)
      {
         _GetWriteAddr(this,logs[lastApplied].filepath(),logs[lastApplied].size());
      }
      else
      {
         LOG(WARNING) << "***** log command error  *****";
      }

      // std::cout<<"执行命令"<<std::endl;
	}
   commitIndex = lastApplied;

   return Status::OK;

	// // 如果当前节点提交的Index比传过来的还高，说明当前节点的日志已经超前,需返回过去
	// if args.PrevLogIndex != -1 && rf.lastApplied > args.PrevLogIndex {
	// 	reply.AppState = AppCommitted
	// 	reply.Term = rf.currentTerm
	// 	reply.Success = false
	// 	reply.UpNextIndex = rf.lastApplied + 1
	// 	return
	// }


   // reply->set_Success(true);
	// reply->set_Term(args->Term());
	// // reply—>set_UpNextIndex -1

	// status = Follower;
	// currentTerm = args->Term();
	// votedFor = -1;
	// voteNum = 0;
	// //rf.persist()
	// gettimeofday(&votedTimer,NULL);

   // if (getLastIndex(this) < args->PrevLogIndex()) {
   //    //自身log下标比leader小 有缺失
	// 	reply.Success = false;
   //    reply.UpNextIndex = getLastIndex(this)+1;
	// 	return Status::OK;
	// } else {
   //    //自身小标大于等于leader
	// 	if(logs[args->PrevLogIndex()].Term!= args->PrevLogTerm()) {
   //       //在前一个下标并不一致回退
	// 		// reply->set_Success(false);
	// 		// auto tempTerm = logs[args->PrevLogIndex()].Term;
	// 		// for(int index = args->PrevLogIndex(); index >= rf.lastIncludeIndex; index--) {
	// 		// 	if(logs[args->PrevLogIndex()].Term!= tempTerm) {
	// 		// 		reply.UpNextIndex = index + 1;
	// 		// 		break
	// 		// 	}
	// 		// }
   //       reply->set_Success(false);
	// 		return Status::OK;
	// 	}
	// }
   // logs= append(rf.logs[:args.PrevLogIndex+1-rf.lastIncludeIndex], args.Entries...)
   // if args.LeaderCommit > rf.commitIndex {
	// 	ts->commitIndex = min(args.LeaderCommit, rf.getLastIndex())
	// }
	// return Status::OK;

}
void* ServiceImpl::leaderAppendEntries(void* arg)
{
     auto ts=(ServiceImpl*) arg;
   
     for(int i=0;i<3;i++)
     {
         if(ts->_myindex==i)
         continue;
         Request* rq=new Request;
         rq->serverindex=i;
         rq->ts=ts;
         pthread_t tid;
         int ret = pthread_create(&tid, NULL,_leaderAppendEntries,(void*)rq);
         if(ret < 0)
         {
            LOG(ERROR) << "***** create append leaderAppendEntries error  *****";
            exit(1);
         }
     }
      //  pthread_cond_wait(&ts->cond,&ts->mu);
}

void* ServiceImpl::electionTicker(void* arg)
{
   pthread_detach(pthread_self());
   auto ts=(ServiceImpl*) arg;
     
    int count=0;
   //std::cout<<"hello world"<<std::endl;
   while(true)//test
   {
       struct timeval now;
      gettimeofday(&now,NULL);
      // std::cout<<"start"<<Getms(&now)<<std::endl;
      //可能会因为网络等原因而++选举切换
      // if(count!=0&&ts->status==Follower)
      // pthread_cond_wait(&ts->cond,&ts->mu);
      
      int sleeptime=GetTime()+100;//ms
     
      usleep(sleeptime*1000);//微秒 毫秒记成微秒
      // std::cout<<"end"<<Getms(&ts->votedTimer)<<std::endl;

      
      if(Getms(&ts->votedTimer)<Getms(&now)&& ts->status != Leader)
      {
         count++;
         //进入选举
         ts->status = Candidate;
			ts->votedFor = ts->_myindex;
			ts->voteNum = 1;
         ts->currentTerm += 1;
         //	rf.persist()
         LOG(INFO) << "***** 发起选举  *****";

         sendElection(ts);
			gettimeofday(&ts->votedTimer,NULL);
      }
      // if(count!=0&&ts->status==Follower)
      // pthread_cond_wait(&ts->cond,&ts->mu);
      sleep(1);
   }
   return 0;
}

void* ServiceImpl::sendElection(void* arg)
{
   ServiceImpl* ts=(ServiceImpl*) arg;
   for(int i=0;i<3;i++)
   {
      if(i==ts->_myindex)
      continue;
       Request* rq=new Request;
       rq->serverindex=i;
       rq->ts=ts;
       pthread_t tid;
      
      int ret = pthread_create(&tid, NULL,_sendElection, (void*)rq);
      
      if(ret < 0)
      {
         LOG(WARNING) << "***** create electionTicker  error*****";
         exit(1);
      }
   }
   //一轮过后wait 只能让它更新一次
 
   return 0;
}

Status ServiceImpl::RequestVote(grpc::ServerContext*  context,const Rpc::RequestVoteArgs* args,Rpc::RequestVoteReply* reply)
{
   

   // 任期小直接返回，网络分区
  
    gettimeofday(&votedTimer,NULL);
   reply->set_term(currentTerm);
    
	if (args->term() < currentTerm) 
   {
      //任期小不投票
    
      reply->set_votegranted(false);
		return  Status::OK;
	}
   //任期大于该结点的任期更新
	if (args->term() > currentTerm) {
		currentTerm = args->term();
		status = Follower;
		votedFor = -1;
		voteNum = 0;
		// persist()
	}
   //判断日志是否最新
   //最后一个日志的任期更小或者任期相等时index更小
      auto judgelog=[&]{
         return args->lastlogterm()> getLastTerm(this) || ( args->lastlogterm() == getLastTerm(this) &&  args->lastlogindex() >= getLastIndex(this)) ;
      };

   if(!judgelog()||votedFor != -1 && votedFor != args->candidateid() && args->term() == reply->term()){
      //日志不是最新或者已经给别的候选人投过票 只能投一次
      reply->set_votegranted(false);
      reply->set_term(currentTerm);
      // pthread_cond_signal(&cond);
		return  Status::OK;
   }//投票
   else {
      //日志一样新，并且没投票或者投给了该候选人
		reply->set_votegranted(true);
      reply->set_term(currentTerm);
		votedFor = args->candidateid();
		currentTerm = args->term();
		// persist()
     
		return  Status::OK;
	}

   return Status::OK;
}
void* ServiceImpl::_sendElection(void* arg)
{
   
   Request* r=(Request*) arg;
     
   pthread_detach(pthread_self());
  
   auto ts=r->ts;
   
   Rpc::RequestVoteArgs args;
  
   args.set_term(ts->currentTerm);
  
   args.set_candidateid(ts->_myindex);
 
   args.set_lastlogindex(getLastIndex(ts));
  
   args.set_lastlogterm(getLastTerm(ts));
   
   Rpc::RequestVoteReply reply;

   grpc::ClientContext context;
  
   ts-> _Stub[r->serverindex]->RequestVote(&context,args, &reply);

   // std::cout<<"发送成功"<<endl;
   gettimeofday(&ts->votedTimer,NULL);
   // pthread_cond_signal(&ts->cond);
   if (ts->status != Candidate || args.term() < ts->currentTerm) 
   {
      //自己不是候选者了，或任期冲突   
       delete r;
		return 0;
	}

   // 返回者的任期大于args（网络分区原因)进行返回
	if (reply.term() > args.term())
   {
      //   pthread_cond_signal(&ts->cond);
		if (ts->currentTerm < reply.term()) {
			 ts->currentTerm = reply.term();
		}
      
      //变从
		ts->status = Follower;
		ts->votedFor = -1;
		ts->voteNum = 0;
    
		gettimeofday(&ts->votedTimer,NULL);
      
		//rf.persist()
       delete r;
		return 0;
	}

   auto term=reply.term();
   if (reply.votegranted() == true && ts->currentTerm == term)
    {
		ts->voteNum += 1;
		if (ts->voteNum >= 3/2+1)
      {
      //当选leader
    
         LOG(INFO) << "***** 当选leader  *****";
         ts->status = Leader;
         ts->votedFor = -1;
         ts->voteNum = 0;
			// rf.persist()
		for(size_t i = 0; i < ts->nextIndex.size(); i++)
      {
			ts->nextIndex[i] = getLastIndex(ts) + 1;
		}
		ts->matchIndex[ts->_myindex] = getLastIndex(ts);
		
		}
       delete r;
		return 0;
	}


}



FileTree ServiceImpl::GetNode(string filepath)
{

   //GF->Get_FileMetadata("/A/B",node);
   FileTree node;
   if(GF->Get_FileMetadata(filepath,node)==false)
   {
      //没找到
       return FileTree();
   }
   return FileTree(node);
}

Status ServiceImpl::FindAddr(grpc::ServerContext*  context,const Rpc::FindAddrRequest* request,Rpc::FindAddrReply* response)
{
  
  std::lock_guard<std::mutex> lg(_mtx);

   std::string filepath=request->filepath();
    LOG(INFO) << "***** FindAddr:"<<filepath<<" *****";
    FileTree node=GetNode(filepath);
   if(node.file_path=="")
   {
      LOG(INFO) << "*****  Not Find Addr  *****";
      return Status(NOT_FOUND,"没有找到文件");
   }

   auto chunk_handles=node.chunk_handles;
     LOG(INFO) << "***** GetWriteAddr test nodes*****"<<node.file_name<<node.file_path<<node.chunk_handles.size();
   for(size_t  i=0;i<chunk_handles.size();i++)
   {
      uint64_t chunk_handle=chunk_handles[i];
      auto chunk=DataNodeManager::GetInstance().GetChunk(const_cast<uint64_t&>(chunk_handle)); 

      response->add_chunk_handles(chunk_handle);
      
      auto rep=response->add_addrs();
      LOG(INFO) << "test Find size:"<<chunk_handle;
      for(int i=0;i<chunk.location().size();i++)
      {
           rep->add_ip_ports(chunk.location()[i]);
            LOG(INFO) << "test Find:"<<chunk.location()[i];     
      }
   }
  LOG(INFO) << "test Find size:"<<response->chunk_handles_size();
   return Status::OK;
}
Status ServiceImpl::_GetWriteAddr(ServiceImpl* ts,std::string filepath,int64_t  write_size)
{
     std::lock_guard<std::mutex> lg(ts->_mtx);
   //fowwer执行
   LOG(INFO) << "***** GetWriteAddr:"<<filepath<<" *****";
    FileTree node=ts->GetNode(filepath);
    if(node.file_path!="")
    {
       LOG(INFO) << "***** GetWriteAddr 文件已经存在*****";
      return Status(ALREADY_EXISTS,"文件已经存在");
    }

    std::string filename=GetFileName(filepath);
    FileTree file=FileTree(filename,filepath,write_size,0);
   
  
 
    auto& chandle=file.chunk_handles;
    //64M
    for(int i=0;write_size>0;i++)
    {
        auto size=min(write_size,(int64_t)CHUNK_SIZE);
        chandle[i]=ts->hanlde;
      //   Chunk* c=new Chunk();//chunk
      //   c->chunk_size=size;
      //  std::string host="localhost:"+to_string(ihash(filepath+to_string(i))%ts->vport.size()+8083);
          
      //   c->location.push_back(ChunkLocation(host));//primary 只分配了
      //   ts-> _chunks[chandle[i]]=c;
         auto chunk=DataNodeManager::GetInstance().AllocateDataNode(ts->hanlde,CHUNK_COUNT,size);
         LOG(INFO) << "test chunk size:"<<chunk.size();
         chandle[i]=ts->hanlde;

         write_size-=size;
         ts->hanlde++;
    }
     LOG(INFO) << "ADD PATHtest "<< ts->GF->Add_Path(filepath,file);
        FileTree node1=ts->GetNode(filepath);
      LOG(INFO) << "***** GetWriteAddr testfilss*****"<<node1.chunk_handles.size();

    LOG(INFO) << "***** GetWriteAddr Success*****";
    return Status::OK;
}
//上传文件 PUT 文件
 Status ServiceImpl::GetWriteAddr(grpc::ServerContext*  context,const Rpc::GetWriteAddrRequest* request,Rpc::GetWriteAddrReply* response)
 {
   if(status!=Leader)
   return Status::OK;
  
   std::lock_guard<std::mutex> lg(_mtx);

   std::string filepath =request->remote_file_path();
    LOG(INFO) << "***** GetWriteAddr:"<<filepath<<"size:"<<request->write_data_size()<<" *****";
   int64_t write_size=request->write_data_size();
   isgetw=false;
   Rpc::LogEntry log;
   log.set_command(1);
   log.set_term(currentTerm);
   log.set_filepath(filepath);
   log.set_size(write_size);
   logs.push_back(log);
   sleep(1);

   if(isgetw==true)
   {
      FileTree node=GetNode(filepath);
    if(node.file_path!="")
    {
      LOG(INFO) << "***** GetWriteAddr 文件已经存在*****";
      return Status(ALREADY_EXISTS,"文件已经存在");
    }
      //nullptr
    std::string filename=GetFileName(filepath);
 
    FileTree file=FileTree(filename,filepath,write_size,0);
    
    auto& chandle=file.chunk_handles;
    //64M
    for(int i=0;write_size>0;i++)
    {
       
        auto size=min(write_size,(int64_t)CHUNK_SIZE);
        chandle[i]=hanlde;//填充目录树handle
        //std::string host="localhost:"+to_string(ihash(filepath+to_string(i))%vport.size()+8083);
         
      //   c->location.push_back(ChunkLocation(host));//primary 只分配了
        
         auto chunk=DataNodeManager::GetInstance().AllocateDataNode(hanlde,CHUNK_COUNT,size);
         LOG(INFO) << "test chunk size:"<<chunk.size();
         chandle[i]=hanlde;
            LOG(INFO) << "test chunk test2:"<<file.chunk_handles.size()<<chandle.size();
         write_size-=size;
         
         response->add_chunk_handles(hanlde);
          auto rep=response->add_addrs();
          for(int j=0;j<CHUNK_COUNT;j++)
          {
             rep->add_ip_ports(chunk.location()[j]);
          }
        

         hanlde++;
    }
      LOG(INFO) << "ADD PATHtest "<< GF->Add_Path(filepath,file);
    FileTree node1=GetNode(filepath);
      LOG(INFO) << "***** GetWriteAddr testfilss*****"<<node1.chunk_handles.size();
      LOG(INFO) << "***** GetWriteAddr success*****";
       GF->Print_Files("/");
    return Status::OK;
   }
   else
   {
      LOG(INFO) << "***** GetWriteAddr 没有获得一半的票*****";
      return Status(CANCELLED,"getw 没有获得一半的票");

   }
 }

Status ServiceImpl::Stat(grpc::ServerContext*  context,const Rpc::StatRequest* request,Rpc::StatReply* response)
{
   
   std::lock_guard<std::mutex> lg(_mtx);
   
   std::string filepath=request->filepath();
   LOG(INFO) << "***** Stat:"<<filepath<<" *****";
   FileTree node=GetNode(filepath);
   if(node.file_path=="")
   {
      response->set_existed(false);
      LOG(INFO) << "***** Stat Not Found file *****";
      return Status(NOT_FOUND,"没有找到文件");
   }


     
   response->set_existed(true);
   response->set_file_name(node.file_name);
   response->set_file_size(node.size);
    LOG(INFO) << "***** Stat Success *****"<<node.size;
   return Status::OK;
}
Status ServiceImpl::_Rename(ServiceImpl* ts,std::string src,std::string dest)
{
   FileTree node=ts->GetNode(src);

   if(node.file_path=="")
   {
       LOG(INFO) << "***** Rename Not Found error *****";
       return Status(NOT_FOUND,"没有找到文件");
   }
   //rename
   if(ts->GF->Rename_Path(src,dest)==false)
   {
       LOG(WARNING) << "***** Rename error  *****";
       return Status(NOT_FOUND,"RENAME失败");
   }
   ts->GF->Print_Files("/");
    LOG(INFO) << "***** Rename Success *****";
   return Status::OK;
}
Status ServiceImpl::Rename(grpc::ServerContext*  context,const Rpc::RenameRequest* request,Rpc::RenameReply* response)
{
    if(status!=Leader)
   return Status::OK;


   std::lock_guard<std::mutex> lg(_mtx);
   rename=false;
   auto dest=request->des_path();
   std::string src=request->src_path();
   LOG(INFO) << "***** Rename:"<<src<<"  "<<dest<<" *****";
   Rpc::LogEntry log;
   log.set_command(2);
   log.set_term(currentTerm);

   log.set_src(src);
   log.set_des(dest);
   logs.push_back(log);
   sleep(1);
   if(rename==true)
   {
      return _Rename(this,src,dest);
   }
   else
   {
     LOG(INFO) << "***** Rename没有获得一半的票 *****";
      return Status(CANCELLED,"rename 没有获得一半的票");
   }

}
Status ServiceImpl::_Delete(ServiceImpl* ts,string filepath)
{
   
   if(ts->GF->Del_Path(filepath)==false)
   {
      LOG(INFO) << "***** Delete Not Found *****";
      return  Status(NOT_FOUND,"没有找到删除的文件");
   }
   LOG(INFO) << "***** Delete Success *****";
   if(ts->status==Leader)
     ts->GF->Print_Files("/");
   return Status::OK;
}
Status ServiceImpl::Delete(grpc::ServerContext*  context,const Rpc::DeleteRequest* request,Rpc::DeleteReply* response)
{
   if(status!=Leader)
   return Status::OK;
  

   auto path=request->remote_file_path();
   std::lock_guard<std::mutex> lg(_mtx);
   LOG(INFO) << "***** Delete:"<<path<<" *****";
   del=false;

   Rpc::LogEntry log;
   log.set_command(3);
   log.set_term(currentTerm);
   log.set_filepath(path);
   logs.push_back(log);
   sleep(1);
   if(del==true)
   {
      return _Delete(this,path);
   }
   else
   {
       LOG(INFO) << "***** Delete没有获得一半的票 *****";
      return Status(CANCELLED,"mkdir 没有获得一半的票");
   }


}
// bool  ServiceImpl::InsertMkdir(const Rpc::MkdirRequest* request)
// {
//    Rpc::LogEntry log;
//    log.set_command(4);
//    log.set_term(currentTerm);
//    log.set_mkdir(*request);
//    return true;
// }
grpc::Status ServiceImpl::_Mkdir(ServiceImpl* ts,grpc::string path)
{
    
      //目录不用创建chunk
       
     
      int ret=ts->GF->Add_Path(path,FileTree(GetFileName(path),path,1,1));
      if(ret==0)
      {
          LOG(INFO) << "***** Mkdir目录存在 *****";
         return Status(ALREADY_EXISTS,"已经有该目录了");
      }
      else if(ret==-1)
      {
          LOG(INFO) << "***** Mkdir目录父路径不存在 *****";
          return Status(NOT_FOUND,"没有该路径");
      }
      // ts->GF->Print_Files("/");
      if(ts->status==Leader)
        ts->GF->Print_Files("/");
       LOG(INFO) << "***** Mkdir Success *****";

      return Status::OK;
}
Status ServiceImpl::Mkdir(grpc::ServerContext*  context,const Rpc::MkdirRequest* request,Rpc::MkdirReply* response)
{
   
   if(status!=Leader)
   return Status::OK;



   auto path=request->new_file_path();
   std::lock_guard<std::mutex> lg(_mtx);
     LOG(INFO) << "***** Mkdir:"<<path<<" *****";
   mkdir=false;

   Rpc::LogEntry log;
   log.set_command(4);
   log.set_term(currentTerm);
   log.set_filepath(path);
   logs.push_back(log);
   sleep(1);
   if(mkdir==true)
   {
      return _Mkdir(this,path);
   }
   else
   {
     LOG(INFO) << "***** Mkdir没有获得一半的票 *****";
      return Status(CANCELLED,"mkdir 没有获得一半的票");
   }

}
//list
Status ServiceImpl::GetSubDirNames(grpc::ServerContext*  context,const Rpc::GetSubDirNamesRequest* request,Rpc::GetSubDirNamesReply* response)
{
   // /A  /B
   
   std::lock_guard<std::mutex> lg(_mtx);

   auto dirpath=request->file_path();
   LOG(INFO) << "***** GetSubDirNames:"<<dirpath<<" *****";
   FileTree node=GetNode(dirpath);

   if(node.file_path=="")
   {
      LOG(INFO) << "***** GetSubDirNames Not Found *****";
       return Status(NOT_FOUND,"没有找到目录");
   }
   
   std::vector<FileTree> ret=GF->Get_Metadata(dirpath);
   for(auto& x:ret)
   {
       response->add_subdir_file_names(x.file_path);
   }
   //返回信息
   LOG(INFO) << "***** GetSubDirNames Success *****";
   return Status::OK;
}
