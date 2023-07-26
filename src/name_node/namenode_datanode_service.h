#pragma once 
#include <memory>

#include "NameNode.grpc.pb.h"
#include"NameNode.pb.h"
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <string>

using namespace Rpc;
// class DataNodeControlServiceImpl final 
//     : public DataNodeControlerService::Service {
    
//   public:
//     grpc::Status CheckHeartbeat(
//         grpc::ServerContext* context, const HeartbeatRequest* request, HeartbeatReply* reply) override {
//         // just to check if datanode is alive
//         *reply->mutable_request() = *request;
//         return grpc::Status::OK;
//     }
    

//     grpc::Status CopyChunk(
//         grpc::ServerContext* context, const CopyChunkRequest* request,CopyChunkReply* reply) override {
//         // to do
        
//         return grpc::Status::OK;
//     }
// };
class DataNodeControlServiceImpl final 
    : public DataNodeControlerService::Service {
    
  public:
    grpc::Status CheckHeartbeat(
        grpc::ServerContext* context, const HeartbeatRequest* request, HeartbeatReply* reply) override {
        // just to check if datanode is alive
        *reply->mutable_request() = *request;
        return grpc::Status::OK;
    }
    

    grpc::Status CopyChunk(
        grpc::ServerContext* context, const CopyChunkRequest* request,CopyChunkReply* reply) override {
        // to do
        
        return grpc::Status::OK;
    }
};






// class DataNodeControlerClient {
//     public:
//         DataNodeControlerClient(std::shared_ptr<grpc::Channel> channel)
//       : stub_(DataNodeControlerService::NewStub(channel)) {}

//         grpc::Status SendRequest(const HeartbeatRequest& request) {
//             grpc::ClientContext default_context;
//             return SendRequest(request, default_context);
//         }
//         grpc::Status SendRequest(const HeartbeatRequest& request, grpc::ClientContext& context) {
//             HeartbeatReply reply;
//             grpc::Status status = stub_->CheckHeartbeat(&context, request, &reply);
//             return status;
//         }

//         grpc::Status SendRequest(const CopyChunkRequest& request) {
//             grpc::ClientContext default_context;
//             return SendRequest(request, default_context);
//         }
//         grpc::Status SendRequest(const CopyChunkRequest& request, grpc::ClientContext& context) {
//             CopyChunkReply reply;
//             grpc::Status status = stub_->CopyChunk(&context, request, &reply);
//             return status;
//         }
//     private:
//         std::unique_ptr<DataNodeControlerService::Stub> stub_;
// };

