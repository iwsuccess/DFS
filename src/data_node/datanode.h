#include <string>
#include <mutex>
#include <thread>
#include <leveldb/db.h> // leveldb::*
#include <leveldb/write_batch.h> // leveldb::WriteBatch
#include <cassert> // assert
#include <iostream> // cout, cerr
#include "gfs_common.h"
#include "./eazylog/src/easylogging++.h"
INITIALIZE_EASYLOGGINGPP 


using Rpc::MultiWriteRequest;
using Rpc::MultiWriteReply;
using Rpc::ReportDataNodeRequest;
using Rpc::ReportDataNodeReply;
using Rpc::HeartbeatRequest;
using Rpc::HeartbeatReply;
using Rpc::CopyChunkWriteRequest;
using Rpc::CopyChunkWriteReply;
using Rpc::CopyChunkRequest;
using Rpc::CopyChunkReply;
using Rpc::HelloRequest;
using Rpc::HelloReply;

typedef struct ChunkId {
  int client_id;
} ChunkId;

typedef struct WriteChunkInfo {
  int client_id;
  int chunkhandle;
  int offset;
  bool padded_chunk;
  std::string data;
} WriteChunkInfo;

class Datanode_static{
  public:
    static int chunk_size;//当前chunkserver的剩余大小
    static std::string db_path;//leveldb数据库存放路径
    static std::string location_me;//chunk_server的ip:port
    static std::mutex chunk_size_mutex;
    static std::mutex write_mutex;
    static std::set<int> metadata; //存放chunkhandle的集合
    static std::mutex  metadata_mutex, am_i_dead_mutex;
    static bool am_i_dead;
    int PerformLocalWriteChunk(const WriteChunkInfo& wc_info);
    void ReportChunkInfo(int chunkhandle);  // fin
    leveldb::DB *ldbptr;//leveldb数据库指针
};
// Logic and data behind the server's behavior.
class Cli2ChunkServer_over :public Cli2ChunkServer::Service, public Datanode_static{
  public :
    Status ReadChunk(ServerContext* context, const ReadChunkRequest* request,
                   ReadChunkReply* reply);

    Status WriteChunk(ServerContext* context, const WriteChunkRequest* request,
                    WriteChunkReply* reply);

    Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply);
    int SendSerializedWriteChunk(WriteChunkInfo& wc_info,const std::string location,std::string data);
    void multiwrite_to_chunkserver(std::vector<std::string> locations,WriteChunkInfo wc_info);
    // void multiwrite_to_chunkserver(   const WriteChunkRequest* request  ,WriteChunkInfo wc_info);
  private:
    std::thread chunk_write_thread;

    

};
class DataNodeControlerService_over: public DataNodeControlerService::Service, public Datanode_static{
  public :
    ~DataNodeControlerService_over();
    DataNodeControlerService_over(std::string server_address,std::vector<std::string> master_address);        // fin 
    Status CheckHeartbeat(ServerContext* context, const HeartbeatRequest* request, HeartbeatReply* reply);    // fin
    Status CopyChunk(ServerContext* context,const CopyChunkRequest *request, CopyChunkReply *reply);          // 
    void chunkserver_report(int i,std::string master_ip);         //finish
  private:

    std::vector<std::unique_ptr<Rpc::ReportDataNodeSerivce::Stub>> stub_master;
    std::vector<std::thread> heartbeat_thread;
};



class ChunkServer2Chunkserver_over :public ChunkServer2Chunkserver::Service, public Datanode_static{
  public:
    Status MultiWrite(ServerContext* context,const MultiWriteRequest* request,MultiWriteReply* reply);
    Status CopyChunkWrite(ServerContext* context, const CopyChunkWriteRequest *request, CopyChunkWriteReply *reply);
  private:

};

//   //每写一次，就将handle映射在set集合
//   //心跳，直接返回ok
//   //每隔两秒，向namenode上报信息 
