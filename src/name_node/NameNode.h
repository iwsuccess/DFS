#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>
#include <time.h> 
#include<sys/time.h>
#include<pthread.h>
#include <unistd.h>
#include"find_files.cpp"
#include <stdlib.h>



#include <iostream>
#include<mutex>
#include<string>

#include<vector>



#define CHUNK_COUNT 3
#define CHUNK_SIZE 64
INITIALIZE_EASYLOGGINGPP
struct mylock_guard{
    mylock_guard(pthread_mutex_t* mt)
    :mtx(mt)
    {
        pthread_mutex_lock(mtx);
    }
    ~mylock_guard()
    {
        pthread_mutex_unlock(mtx);
    }
    private:
    pthread_mutex_t* mtx; 
};
enum role{
    Follower=1,
	Candidate=2,
	Leader=3
};
// enum AppendEntriesState{
//     Normal,     //0
// 	OutOfDate,                           
// 	Committed,                          
// 	Mismatch              
// }


struct ChunkLocation {
    ChunkLocation(std::string s)
    :location(s)
    {
        
    }
    std::string location; // "ip:port"
    //int64_t version;
};
//  struct ChunkServer{
//      std::unique_ptr<gfs::GFS::Stub> stub;
//      //租约
// }

// struct LogEntry{
//     int Term;
//     int command;  
// };


// AppendEntriesArgs Append Entries RPC structure
// struct AppendEntriesArgs  {
// 	int Term ;                // leader的任期
// 	int LeaderId  ;      // leader自身的ID
// 	int PrevLogIndex   ;      // 用于匹配日志位置是否是合适的，初始化rf.nextIndex[i] - 1
// 	int PrevLogTerm     ;     // 用于匹配日志的任期是否是合适的是，是否有冲突
// 	std::vector<LogEntry> Entries   ;    // 预计存储的日志（为空时就是心跳连接）
// 	int LeaderCommit    ;     // leader的commit index指的是最后一个被大多数机器都复制的日志Index
// };

// struct AppendEntriesReply  {
// 	int Term       ;   // leader的term可能是过时的，此时收到的Term用于更新他自己
// 	bool Success    ;  //	如果follower与Args中的PreLogIndex/PreLogTerm都匹配才会接过去新的日志（追加），不匹配直接返回false
// 	int UpNextIndex  ; // 如果发生conflict时reply传过来的正确的下标用于更新nextIndex[i]
// };



struct Chunk {

    std::vector<ChunkLocation> location;//0was primary
    int chunk_size;
};

//文件元数据
// struct FileMetadata {
//    // File name
//    std::string filename;
//    std::string file_path;
//    uint64_t size;
//    // An map of chunk index to chunk handle
//    std::unordered_map<int, int64_t> chunk_handles;
// };


class ServiceImpl:public Rpc::Cli2Master::Service {
    public:                                                                                                              
    grpc::Status FindAddr(grpc::ServerContext*  context,const Rpc::FindAddrRequest* request,Rpc::FindAddrReply* response) override;
    //对应1 command
    static grpc::Status _GetWriteAddr(ServiceImpl* ts,std::string filepath,int64_t write_size);
    grpc::Status GetWriteAddr(grpc::ServerContext*  context,const Rpc::GetWriteAddrRequest* request,Rpc::GetWriteAddrReply* response) override;
    grpc::Status Stat(grpc::ServerContext*  context,const Rpc::StatRequest* request,Rpc::StatReply* response) override;
    //对应2
    static grpc::Status _Rename(ServiceImpl* ts,string src,string dest);
    grpc::Status Rename(grpc::ServerContext*  context,const Rpc::RenameRequest* request,Rpc::RenameReply* response) override;
    //对应3
    static grpc::Status _Delete(ServiceImpl* ts,string path);
    grpc::Status Delete(grpc::ServerContext*  context,const Rpc::DeleteRequest* request,Rpc::DeleteReply* response) override;
    //对应4
    grpc::Status Mkdir(grpc::ServerContext*  context,const Rpc::MkdirRequest* request,Rpc::MkdirReply* response) override;
    static grpc::Status _Mkdir(ServiceImpl* ts,string path);
    grpc::Status GetSubDirNames(grpc::ServerContext*  context,const Rpc::GetSubDirNamesRequest* request,Rpc::GetSubDirNamesReply* response) override;
    grpc::Status RequestVote(grpc::ServerContext*  context,const Rpc::RequestVoteArgs* args,Rpc::RequestVoteReply* reply) override; 
    grpc::Status AppendEntries(grpc::ServerContext*  context,const Rpc::AppendEntriesArgs* args,Rpc::AppendEntriesReply* reply) override; 

    // bool InsertMkdir(&request); 
    bool isgetw;
    bool rename;
    bool del;
    bool mkdir;
    ServiceImpl(int index);
    
    ~ServiceImpl();
    static void*electionTicker(void* arg);
    static void*sendElection(void* arg);
    static void*_sendElection(void* arg);
    static void*appendTicker(void* arg);
    static void*leaderAppendEntries(void* arg);
    static void*_leaderAppendEntries(void * arg);
    static void*isleader(void* arg);
    int _myindex;
    int appendNums=0;
   
    std::vector<Rpc::LogEntry>  logs ;

    role status;//0fowller 1can 2leader
    // // ServiceImpl(int index)=default;
    // private:
     FileTree GetNode(std::string filepath);
    GetFiles* GF;
    //bool CreateNode(std::string filepath,size_t size,int64_t hanlde);
    //路径->文件元数据
    //std::unordered_map<std::string, FileTree*> _PathNode;
    //chunk handle->chunk
    std::vector<int> vport;//记录可用location
    // std::unordered_map<int64_t,Chunk*> _chunks; 
    //ip：port to chunkServer
    std::unordered_map<std::string,ChunkLocation> _Chunklocation;//暂时这样写
    int64_t hanlde;
       //ip+port->chunkServer
    //  std::unordered_map<std::string, ChunkServer> chunk_servers;
    std::mutex _mtx;

    //Raft
    std::vector<std::shared_ptr<Rpc::Cli2Master::Stub>> _Stub; 
    int currentTerm;//当前任期
    int votedFor;//投票给了谁
    int commitIndex;
    timeval votedTimer;
	int lastApplied;
    // nextIndex与matchIndex初始化长度应该为len(peers)
    std::vector<int> nextIndex;//下一个log下标
	std::vector<int> matchIndex;//已经提交的下标匹配
 
	int voteNum;     // 记录当前投票给了谁
    pthread_cond_t cond;
    pthread_mutex_t  mu;


};