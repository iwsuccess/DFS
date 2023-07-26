/**
 * To manage the interact between namenode and datanode
 * Including  datanode register/unregister, heartbeat 
 * by DeepZheng 8/8
**/
#pragma once 

#include <iostream>
#include <thread>
#include "NameNode.grpc.pb.h"
#include"NameNode.pb.h"
#include <unordered_map>


 #include "namenode_datanode_service.h"
#include "easylogging++.h"
using namespace Rpc;
class DataNodeAvailableSizeGreaterCompare {
    public:
        bool operator() (const Rpc::DataNode& lhs,
                        const Rpc::DataNode& rhs) const {
            return lhs.available_size() > rhs.available_size();
        }

        bool operator() (const std::shared_ptr<Rpc::DataNode>& lhs,
                        const std::shared_ptr<Rpc::DataNode>& rhs) const {
            return lhs->available_size() > rhs->available_size();
        }
};
class DataNodeControlerClient {
    public:
        DataNodeControlerClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(DataNodeControlerService::NewStub(channel)) {}

        grpc::Status SendRequest(const HeartbeatRequest& request) {
            grpc::ClientContext default_context;
            return SendRequest(request, default_context);
        }
        grpc::Status SendRequest(const HeartbeatRequest& request, grpc::ClientContext& context) {
            HeartbeatReply reply;
            grpc::Status status = stub_->CheckHeartbeat(&context, request, &reply);
            return status;
        }

        grpc::Status SendRequest(const CopyChunkRequest& request) {
            grpc::ClientContext default_context;
            return SendRequest(request, default_context);
        }
        grpc::Status SendRequest(const CopyChunkRequest& request, grpc::ClientContext& context) {
            CopyChunkReply reply;
            grpc::Status status = stub_->CopyChunk(&context, request, &reply);
            return status;
        }
    private:
        std::unique_ptr<DataNodeControlerService::Stub> stub_;
};


class DataNodeManager {
    public:
        // the sinleton instance
        static DataNodeManager& GetInstance() {
            static DataNodeManager instance;
            return instance;
        }

        DataNodeManager() = default;
        DataNodeManager(const DataNodeManager&) = delete;
        void operator=(const DataNodeManager&) = delete;

     Chunk AllocateDataNode(const int64_t &chunk_handle, const ushort &datenode_count,int64_t chunk_size);

        bool RegisterDataNode(const std::shared_ptr<Rpc::DataNode> datanode);
        void UnRegisterDataNode(const std::string location);
        
        const Rpc::DataNode GetDataNode(const std::string& location);
        
        const Rpc::Chunk GetChunk(uint64_t &chunk_handle) {
            return chunk_map_[chunk_handle];
        }
        bool CheckExistDataNode(const std::string& location);
        // update datanode from report message
        void UpdateDataNode(const Rpc::DataNode& new_datanode);
        
        void StartHeartbeat(); 
        void HeartbeatTask();
        
        std::shared_ptr<DataNodeControlerClient> GetOrCreateDataNodeControlClient(const std::string& address);

    private:
        // backgound thread used for running heartbeat task
        std::unique_ptr<std::thread> heartbeat_thread_;
        
        uint16_t max_heartbeat_attempts {3};
        // location to datanode
        std::unordered_map<std::string, std::shared_ptr<Rpc::DataNode>> datanode_map_;

        std::list<std::shared_ptr<Rpc::DataNode>> datanode_priority_list_;
        // chunk handle to chunk
        std::unordered_map<int64_t, Rpc::Chunk> chunk_map_;
        // location to client to send message
        std::unordered_map<std::string, std::shared_ptr<DataNodeControlerClient>> datanode_control_clients_;
};

class ReportDataNodeServiceImpl final: public ReportDataNodeSerivce::Service {
    public:
         grpc::Status ReportDataNode(
             grpc::ServerContext* context, const ReportDataNodeRequest* request, ReportDataNodeReply* reply) override;
};








