#include <iostream>
#include <fstream>
#include <memory>
#include <inttypes.h>
#include <csignal>
#include <grpc++/grpc++.h>
#include <google/protobuf/timestamp.pb.h>
//#include "./gen_code/gfs.grpc.pb.h"
//#include "./gen_code/namenode.grpc.pb.h"
// #include "./gen_code/final.grpc.pb.h"
#include "datanode.h"
using namespace std;

//leveldb::DB* Datanode_static::ldbptr;
std::string Datanode_static::db_path = "";
int Datanode_static::chunk_size=CHUNK_SIZE_IN_BYTES;
std::mutex Datanode_static::chunk_size_mutex;
std::mutex Datanode_static::write_mutex;
std::set<int> Datanode_static::metadata;
std::mutex  Datanode_static::metadata_mutex, Datanode_static::am_i_dead_mutex;
bool Datanode_static::am_i_dead;
std::string Datanode_static::location_me = "";//chunk_server的ip:port



Status Cli2ChunkServer_over::SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) {
    std::string prefix("Hello ");
    std::cout<<request->name()<<std::endl;
    reply->set_message(prefix + request->name());
    return Status::OK;
  }



DataNodeControlerService_over::DataNodeControlerService_over(std::string server_address,std::vector<string> master_address) { //std::string master_address
  location_me = server_address;
  am_i_dead = false;
  std::string tem="meta";
  std::string tem2=".db";
  db_path=tem+location_me+tem2;
  //ldbptr = nullptr;
  
  for(int i=0;i<master_address.size();i++){
        stub_master.push_back(Rpc::ReportDataNodeSerivce::NewStub(grpc::CreateChannel
                      (master_address[i],
                      grpc::InsecureChannelCredentials())));

      //chunk_Server一启动，就立即向namenode上报信息
      ReportDataNodeRequest request;
      ReportDataNodeReply reply;
      ClientContext context;

      //ip:port
      request.set_location(location_me);
      //chunkserver的可用size大小
      request.set_available_size(chunk_size);
      
      Status status = stub_master[i]->ReportDataNode(&context, request, &reply);
      if (status.ok()) {
        LOG(INFO)<< "Successfully registered with master.";
      } else {
        LOG(FATAL)<< "Failed to register with master.";
      }
       heartbeat_thread.push_back(std::thread(std::bind(&DataNodeControlerService_over::chunkserver_report, this,i,master_address[i])));
       sleep(1);
      //heartbeat_thread[i] = std::thread(GFSServiceImpl::chunkserver_report,i);


  }
}
//datanode----namenode:每隔两秒向namenode上报一次信息
void DataNodeControlerService_over::chunkserver_report(int i,std::string master_ip){
     //打开leveldb数据库
     leveldb::DB *ldbptr_tem;//leveldb数据库指针
      leveldb::Options options;
      options.create_if_missing = true;
      while(1){
        leveldb::Status status = leveldb::DB::Open(options, db_path, &ldbptr_tem);
        if(!status.ok()) {
            LOG(ERROR)<< "open level db error:" << status.ToString();
        }
        if(ldbptr_tem!=nullptr)
          break;
      }
      assert(ldbptr_tem != nullptr);
      //LOG(INFO)<< "open level db successfully !" ;

    // 取出数据库中所有的chunhandle,放入集合set中
    {
        leveldb::Iterator *iterator = ldbptr_tem->NewIterator(leveldb::ReadOptions());
        if(!iterator) {
            LOG(ERROR)<< "can not new iterator";
        }
        // caller must call one of the Seek methods on the iterator before using it
        iterator->SeekToFirst();
        while(iterator->Valid())
        {
            leveldb::Slice sKey = iterator->key();
            std::string temp=sKey.ToString();
            //std::lock_guard<std::mutex> guard_v(metadata_mutex);
            metadata_mutex.lock();
            metadata.insert(atoi(temp.c_str())); 
            metadata_mutex.unlock();
            iterator->Next();
        }
        delete(iterator);
    }
    delete(ldbptr_tem);

    

    //循环上报信息
  while(true){

        am_i_dead_mutex.lock();
        if (am_i_dead) {
          break;
        }
        am_i_dead_mutex.unlock();
                  ClientContext context;
                  ReportDataNodeRequest request; 
                  ReportDataNodeReply  reply; 
                    //ip:port
                  request.set_location(location_me);
                  //可用大小
                  //std::lock_guard<std::mutex> guard_S(chunk_size_mutex);
                  chunk_size_mutex.lock();
                  request.set_available_size(chunk_size);
                  chunk_size_mutex.unlock();
                  //拥有的chunkhandle
                  
                 // std::lock_guard<std::mutex> guard(metadata_mutex);
                 metadata_mutex.lock();
                  for(auto it:metadata){
                    request.add_stored_chunk_handles(it);
                  }
                  metadata_mutex.unlock();
                 
                  Status status_s = stub_master[i]->ReportDataNode(&context,request,&reply);
                  if (status_s.ok()) {
                    LOG(INFO)<< location_me << "info report to "<< master_ip <<"master suceess!" ;
                  }
                  std::this_thread::sleep_for(std::chrono::seconds(HEARTBEAT_DURATION_SECONDS));            
            }
            
}
//将chunkserver的chunkhandle放入set集合中
void Datanode_static::ReportChunkInfo(int chunkhandle) {
  // Acquire the metadata mutex
  //std::lock_guard<std::mutex> guard(metadata_mutex);
    metadata_mutex.lock();
    metadata.insert(chunkhandle);
    metadata_mutex.unlock();
}
//namenode-----datanode的心跳
Status DataNodeControlerService_over::CheckHeartbeat(ServerContext* context, const HeartbeatRequest* request, HeartbeatReply* reply){
       return Status::OK;
}

Status DataNodeControlerService_over::CopyChunk(ServerContext* context,const CopyChunkRequest *request, CopyChunkReply *reply){
  int chunkhandle = request->chunk_handle();
  std::string data = "";

  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, db_path, &ldbptr);
      if(!status.ok()) {
          LOG(ERROR)<< "open level db error:" << status.ToString();

          delete(ldbptr);
          return Status::OK;
      }
      assert(ldbptr != nullptr);
      LOG(INFO)<< "open level db successfully !" ;

  // get data
    {
        leveldb::ReadOptions getOptions;
        status = ldbptr->Get(getOptions,std::to_string(chunkhandle), &data);
        if (!status.ok()) {
            // std::cerr << "get data error:" << status.ToString() << std::endl;
            LOG(ERROR)<< "get data error:" << status.ToString();
        }else{
          LOG(INFO)<< "get data successfully:" << data ;
        }
    }

    delete(ldbptr);   


    ClientContext context_copychunk;
    CopyChunkWriteRequest request_copychunk;
    CopyChunkWriteReply reply_copychunk;
    request_copychunk.set_data(data);
    request_copychunk.set_chunk_handle(request -> chunk_handle());

    std::unique_ptr<Rpc::ChunkServer2Chunkserver::Stub> stub_copychunk = Rpc::ChunkServer2Chunkserver::NewStub(grpc::CreateChannel(request -> location(), grpc::InsecureChannelCredentials())); // ????
    Status status_copychunk = stub_copychunk->CopyChunkWrite(&context_copychunk, request_copychunk, &reply_copychunk);
    if(status_copychunk.ok()){
      return Status::OK;
    }
    else {
      return Status(grpc::NOT_FOUND,"娌℃湁澶嶅埗鎴愬姛");
      // return Status::OK;
    }
}





Status Cli2ChunkServer_over::ReadChunk(ServerContext* context,
                                 const ReadChunkRequest* request,
                                 ReadChunkReply* reply) {
  int chunkhandle = request->chunkhandle();
  int offset = request->offset();
  int length = request->length();
  std::string data(length, ' ');

  if ((length + offset > CHUNK_SIZE_IN_BYTES)) {
    LOG(INFO)<< "Read exceeds chunk size: " << length + offset;
    reply->set_bytes_read(0);
    return Status::OK;
  }

  //打开数据库
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, db_path, &ldbptr);
      if(!status.ok()) {
          // std::cerr << "open level db error:" << status.ToString() << std::endl;
          LOG(ERROR)<< "open level db error:" << status.ToString();
          //关闭数据库
          delete(ldbptr);
          return Status::OK;
      }
      assert(ldbptr != nullptr);
      LOG(INFO)<< "open level db successfull !";

  // get data
    {
        leveldb::ReadOptions getOptions;
        status = ldbptr->Get(getOptions,std::to_string(chunkhandle), &data);
        if (!status.ok()) {
            // std::cerr << "get daINFO;/ta error:" << status.ToString() << std::endl;
            LOG(ERROR)<< "get daINFO;/ta error:" << status.ToString();
            reply->set_bytes_read(0);
        }else{
          LOG(INFO)<< "get all data successfully:" << data ;
          //分割获取到的data，取出真正需要给客户端返回的data
          std::string data_=data.substr(offset,length-offset); 
          int str_length=data.length();
          reply->set_bytes_read(str_length);
          reply->set_data(data_);
          LOG(INFO)<< "get true data successfully:" << data_ ;
        }
    }
    //关闭数据库
    delete(ldbptr);
    return Status::OK;
}

int Datanode_static::PerformLocalWriteChunk(const WriteChunkInfo& wc_info)
{
  ChunkId chunk_id;
  chunk_id.client_id = wc_info.client_id;
  int chunkhandle = wc_info.chunkhandle;
  int offset = wc_info.offset;

  
  std::string data=wc_info.data;

  int length = data.length();

  if ((length + offset > CHUNK_SIZE_IN_BYTES)) {
    LOG(ERROR)<< "Write exceeds chunk size: " << length + offset ;
    return 0;
  }

  //打开数据库
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, db_path, &ldbptr);
      if(!status.ok()) {
          // std::cerr << "open level db error:" << status.ToString() << std::endl;
          LOG(ERROR)<< "open level db error:" << status.ToString();
          //关闭数据库
        delete(ldbptr);
          return 0;
      }
      assert(ldbptr != nullptr);
      LOG(INFO)<< "open level db successfully !";
  
   // put insert
    {
        leveldb::WriteOptions putOptions;
        leveldb::ReadOptions getOptions;
        //putOptions.sync = true;
        putOptions.sync = false;
        std::string old_data;
         //查看该chunkhandle下是否已经写入过数据
        //若写入过，则需要根据偏移，长度，重新组织数据
        //若未写入过，则直接写入
        status = ldbptr->Get(getOptions, std::to_string(chunkhandle), &old_data);
        if (!status.ok()) {//未找到chunkhandle,直接写入
             status = ldbptr->Put(putOptions, std::to_string(chunkhandle), data);
            if (!status.ok()) {
              // std::cerr << "put data error:" << status.ToString() << std::endl;
              LOG(ERROR)<< "put data error:" << status.ToString();
              //关闭数据库
              delete(ldbptr);
              return 0;
            }
              LOG(INFO)<< "put data successfully!" ;
              delete(ldbptr);
              return length;
        }
        else{//找到chunkhandle，根据偏移，写入的数据长度，重新组织数据
          //进行偏移，写入真正需要的数据
          int old_data_lenth=old_data.length();

          if(offset<old_data_lenth){//偏移小于已经存在的数据长度，说明原始数据需要截断，和新数据进行拼接组合
            std::string data_1=old_data.substr(0,offset); 
             std::string data_;
            if(old_data_lenth-length-offset>0){
              std::string data_2=old_data.substr(offset+length,old_data_lenth-length-offset);
               data_=data_1+data+data_2;//真正写入的数据
            }
            else
              {
                data_=data_1+data;
              }
            status = ldbptr->Put(putOptions, std::to_string(chunkhandle), data_);
            if (!status.ok()) {
              // std::cerr << "put data error:" << status.ToString() << std::endl;
              LOG(ERROR)<< "put data error:" << status.ToString();

              
              delete(ldbptr);
              return 0;
            }
              LOG(INFO)<< "put data successfully!";
              
              delete(ldbptr);
              return length;
          }
          else{//偏移大于等于已经存在的数据长度，直接在原始数据的后面追加
            std::string data_=old_data+data;
            status = ldbptr->Put(putOptions, std::to_string(chunkhandle), data_);
            if (!status.ok()) {
              // std::cerr << "put data error:" << status.ToString() << std::endl;
              LOG(ERROR)<< "put data error:" << status.ToString();
              delete(ldbptr);
              return 0;
            }
              LOG(INFO)<< "put data successfully!" ;
              delete(ldbptr);
              return length;

          }
    
        }   
    }

    if(ldbptr!=nullptr)
      delete(ldbptr);
    return 0;
}



Status Cli2ChunkServer_over::WriteChunk(ServerContext* context,
                                  const WriteChunkRequest* request,
                                  WriteChunkReply* reply) {
  int bytes_written;
  WriteChunkInfo wc_info;

  //std::lock_guard<std::mutex> guard(write_mutex);
  LOG(INFO)<< "Got server WriteChunk for chunkhandle = " << request->chunkhandle() ;

  wc_info.client_id = request->client_id();
  wc_info.chunkhandle = request->chunkhandle();
  wc_info.offset = request->offset();
  wc_info.data=request->data();
  wc_info.padded_chunk = false;

  bytes_written = PerformLocalWriteChunk(wc_info);

 //更新chunk_size

  chunk_size_mutex.lock();
  chunk_size=chunk_size-bytes_written-wc_info.offset;
  chunk_size_mutex.unlock();


  if(bytes_written){
       reply->set_bytes_read(bytes_written);
       LOG(TRACE)<<wc_info.data;
       ReportChunkInfo(wc_info.chunkhandle);
  }

  //std::cout<<"data type"<<typeid(request->locations()).name()<<std::endl;
   //std::cout<<"out"<<std::endl;
  if(bytes_written){
  //  chunk_write_thread=std::thread(std::bind(&Cli2ChunkServer_over::multiwrite_to_chunkserver, this,request->locations(),wc_info));
    vector<string>locations_mult;
    for(auto location : request->locations()){
      locations_mult.push_back(location);
    }
    
   chunk_write_thread=std::thread(std::bind(&Cli2ChunkServer_over::multiwrite_to_chunkserver, this,locations_mult,wc_info));
 
   chunk_write_thread.detach();
   
  }
  // if(bytes_written){
  //   for (const auto& location : request->locations()) {
  //       LOG(INFO)<< "CS location: " << location ;
  //       std::cout<<"location == "<<location<<"location_me == "<<location_me<<std::endl;
  //       if (location == location_me){
  //         std::cout<<location<<std::endl;
  //         continue;
  //       }
  //       chunk_write_thread.push_back(std::thread(std::bind(&Cli2ChunkServer_over::multiwrite_to_chunkserver, this,location,wc_info)));
  //     }
  // }
  LOG(TRACE)<<"client rev data ok!";
  return Status::OK;



 // If the local write succeeded, send SerializedWrites to backups
  //将数据写入同一个chunkhandle下的其他chunkserver中，进行备份


  // reply->set_bytes_read(bytes_written);
  // LOG(TRACE)<<wc_info.data;
  //写入成功，则更新存储chunkhandle的set集合
  // if (bytes_written) {
  //   ReportChunkInfo(wc_info.chunkhandle);
  // }

}


void Cli2ChunkServer_over::multiwrite_to_chunkserver(std::vector<std::string> locations,WriteChunkInfo wc_info){
// void Cli2ChunkServer_over::multiwrite_to_chunkserver(  const WriteChunkRequest* request ,WriteChunkInfo wc_info){

      //std::cout<<"in"<<std::endl;
          for (const auto& location : locations) {
              LOG(INFO)<< "CS location: " << location ;
              std::cout<<"location == "<<location<<"location_me == "<<location_me<<std::endl;
              if (location == location_me){
                std::cout<<location<<std::endl;
                continue;
              }
              if(!SendSerializedWriteChunk(wc_info, location,wc_info.data)) {
                //bytes_written = 0;
                LOG(ERROR)<< "SerializedWrite failed for location: " << location;
                break;
              }
            }
        
    
}




int Cli2ChunkServer_over::SendSerializedWriteChunk(WriteChunkInfo& wc_info,
                                             const std::string location,std::string data) {

  MultiWriteRequest request;
  MultiWriteReply reply;
  ClientContext context;

  // Create a connection to replica ChunkServer
  std::unique_ptr<Rpc::ChunkServer2Chunkserver::Stub> stub =Rpc::ChunkServer2Chunkserver::NewStub(
      grpc::CreateChannel(location, grpc::InsecureChannelCredentials()));

  // Prepare Request -> Perform RPC -> Read reply -> Return
  request.set_client_id(wc_info.client_id);
  request.set_chunkhandle(wc_info.chunkhandle);
  request.set_offset(wc_info.offset);
  request.set_padded_chunk(wc_info.padded_chunk);
  request.set_data(data);
  Status status = stub->MultiWrite(&context, request, &reply);


  if (status.ok()) {
    LOG(INFO)<< "SerializedWrite bytes_written = " << reply.bytes_written() <<" at location: " << location ;
    return reply.bytes_written();
  } else {
    LOG(ERROR)<< "SerializedWrite failed at location: " << location;
    return 0;
  }

}

Status ChunkServer2Chunkserver_over::MultiWrite(ServerContext* context,
                                  const MultiWriteRequest* request,
                                  MultiWriteReply* reply) {
  WriteChunkInfo wc_info;
  int bytes_written;

  wc_info.client_id = request->client_id();

  wc_info.chunkhandle = request->chunkhandle();
  wc_info.offset = request->offset();
  wc_info.padded_chunk = request->padded_chunk();
  wc_info.data=request->data();
  
 
  bytes_written = PerformLocalWriteChunk(wc_info);

  
  //更新chunk_size
  //std::lock_guard<std::mutex> guard_S(chunk_size_mutex);
  chunk_size_mutex.lock();
  chunk_size=chunk_size-bytes_written-wc_info.offset;
  chunk_size_mutex.unlock();

  reply->set_bytes_written(bytes_written);

  if (bytes_written) {
    ReportChunkInfo(wc_info.chunkhandle);
  }

  return Status::OK;
}


DataNodeControlerService_over::~DataNodeControlerService_over() {
  // Signal thread to shutdown.
  am_i_dead_mutex.lock();
  am_i_dead = true;
  am_i_dead_mutex.unlock();

  // Wait for thread to exit.
  for(int i=0;i<heartbeat_thread.size();i++)
     heartbeat_thread[i].join();
}

std::unique_ptr<Server> server;

void HandleTerminate(int signal) {
  if (server) {
    LOG(ERROR)<< "Shutting down." ;
    server->Shutdown();
  }
}


Status ChunkServer2Chunkserver_over::CopyChunkWrite(ServerContext* context, const CopyChunkWriteRequest *request, CopyChunkWriteReply *reply){

    //std::lock_guard<std::mutex> guard(write_mutex);
    WriteChunkInfo info_write;
    info_write.client_id = 0;
    info_write.chunkhandle = request->chunk_handle();
    info_write.offset = 0;
    info_write.data=request->data();
    info_write.padded_chunk = false;
    int length = PerformLocalWriteChunk(info_write);
    if(length == info_write.data.length())
      return Status::OK;
    else {
      return Status(grpc::NOT_FOUND,"write false");
      // return Status::OK;
    }
}


void RunServer(std::vector<std::string> master_address,std::string server_address) {

  DataNodeControlerService_over service1(server_address, master_address);

  ChunkServer2Chunkserver_over service2;
  Cli2ChunkServer_over service3;
  ServerBuilder builder;
  ServerBuilder builder2;
  ServerBuilder builder3;
  // Listen on the given address without any authentication mechanism.
  int maohao_posi = server_address.find(":");
  string location_port = "0.0.0.0" + server_address.substr(maohao_posi,server_address.length() - maohao_posi);
  LOG(INFO)<< "location_port is "<<location_port;
  // LOG(INFO)<< "location_port is "<<server_address;
  builder.AddListeningPort(location_port, grpc::InsecureServerCredentials());
  // builder2.AddListeningPort(location_port, grpc::InsecureServerCredentials());
  // builder3.AddListeningPort(location_port, grpc::InsecureServerCredentials());
   builder.SetMaxReceiveMessageSize(1024*1024*100);
  // builder2.SetMaxReceiveMessageSize(1024*1024*100);
  // builder3.SetMaxReceiveMessageSize(1024*1024*100);
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service1);
  builder.RegisterService(&service2);
  builder.RegisterService(&service3);
  // Finally assemble the server.
  server = builder.BuildAndStart();
// std::unique_ptr<Server> server2 = builder.BuildAndStart();
// std::unique_ptr<Server> server3 = builder.BuildAndStart();

  LOG(INFO)<< "Server listening on " << server_address ;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
  // server2->Wait();
  // server3->Wait();
}

int main(int argc, char** argv) {
  el::Configurations conf("../eazylog/config/log.conf");
  el::Loggers::reconfigureAllLoggers(conf);
  if (argc < 3) {
    std::cout << "Usage: ./datanode \
              master_address_list (like IP:port IP:port ...) \
              chunkserver_address (like IP:port)"
              << std::endl;
    return 1;
  }
  std::signal(SIGINT, HandleTerminate);
  std::signal(SIGTERM, HandleTerminate);
  std::vector<string>ip;
  for(int i = 0;i < argc - 2;i++){
    ip.push_back(argv[1 + i]);
  }
  


  RunServer(ip, argv[argc-1]);
  return 0;
}


