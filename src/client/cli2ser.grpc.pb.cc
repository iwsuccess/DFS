// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: cli2ser.proto

#include "cli2ser.pb.h"
#include "cli2ser.grpc.pb.h"

#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace Rpc {

static const char* Cli2Master_method_names[] = {
  "/Rpc.Cli2Master/FindAddr",
  "/Rpc.Cli2Master/GetWriteAddr",
  "/Rpc.Cli2Master/Stat",
  "/Rpc.Cli2Master/Rename",
  "/Rpc.Cli2Master/Delete",
  "/Rpc.Cli2Master/Mkdir",
  "/Rpc.Cli2Master/GetSubDirNames",
  "/Rpc.Cli2Master/RequestVote",
  "/Rpc.Cli2Master/AppendEntries",
};

std::unique_ptr< Cli2Master::Stub> Cli2Master::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Cli2Master::Stub> stub(new Cli2Master::Stub(channel));
  return stub;
}

Cli2Master::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_FindAddr_(Cli2Master_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetWriteAddr_(Cli2Master_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Stat_(Cli2Master_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Rename_(Cli2Master_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Delete_(Cli2Master_method_names[4], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_Mkdir_(Cli2Master_method_names[5], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetSubDirNames_(Cli2Master_method_names[6], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_RequestVote_(Cli2Master_method_names[7], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_AppendEntries_(Cli2Master_method_names[8], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Cli2Master::Stub::FindAddr(::grpc::ClientContext* context, const ::Rpc::FindAddrRequest& request, ::Rpc::FindAddrReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_FindAddr_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::FindAddrReply>* Cli2Master::Stub::AsyncFindAddrRaw(::grpc::ClientContext* context, const ::Rpc::FindAddrRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::FindAddrReply>::Create(channel_.get(), cq, rpcmethod_FindAddr_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::FindAddrReply>* Cli2Master::Stub::PrepareAsyncFindAddrRaw(::grpc::ClientContext* context, const ::Rpc::FindAddrRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::FindAddrReply>::Create(channel_.get(), cq, rpcmethod_FindAddr_, context, request, false);
}

::grpc::Status Cli2Master::Stub::GetWriteAddr(::grpc::ClientContext* context, const ::Rpc::GetWriteAddrRequest& request, ::Rpc::GetWriteAddrReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_GetWriteAddr_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::GetWriteAddrReply>* Cli2Master::Stub::AsyncGetWriteAddrRaw(::grpc::ClientContext* context, const ::Rpc::GetWriteAddrRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::GetWriteAddrReply>::Create(channel_.get(), cq, rpcmethod_GetWriteAddr_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::GetWriteAddrReply>* Cli2Master::Stub::PrepareAsyncGetWriteAddrRaw(::grpc::ClientContext* context, const ::Rpc::GetWriteAddrRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::GetWriteAddrReply>::Create(channel_.get(), cq, rpcmethod_GetWriteAddr_, context, request, false);
}

::grpc::Status Cli2Master::Stub::Stat(::grpc::ClientContext* context, const ::Rpc::StatRequest& request, ::Rpc::StatReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Stat_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::StatReply>* Cli2Master::Stub::AsyncStatRaw(::grpc::ClientContext* context, const ::Rpc::StatRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::StatReply>::Create(channel_.get(), cq, rpcmethod_Stat_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::StatReply>* Cli2Master::Stub::PrepareAsyncStatRaw(::grpc::ClientContext* context, const ::Rpc::StatRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::StatReply>::Create(channel_.get(), cq, rpcmethod_Stat_, context, request, false);
}

::grpc::Status Cli2Master::Stub::Rename(::grpc::ClientContext* context, const ::Rpc::RenameRequest& request, ::Rpc::RenameReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Rename_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::RenameReply>* Cli2Master::Stub::AsyncRenameRaw(::grpc::ClientContext* context, const ::Rpc::RenameRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::RenameReply>::Create(channel_.get(), cq, rpcmethod_Rename_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::RenameReply>* Cli2Master::Stub::PrepareAsyncRenameRaw(::grpc::ClientContext* context, const ::Rpc::RenameRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::RenameReply>::Create(channel_.get(), cq, rpcmethod_Rename_, context, request, false);
}

::grpc::Status Cli2Master::Stub::Delete(::grpc::ClientContext* context, const ::Rpc::DeleteRequest& request, ::Rpc::DeleteReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Delete_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::DeleteReply>* Cli2Master::Stub::AsyncDeleteRaw(::grpc::ClientContext* context, const ::Rpc::DeleteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::DeleteReply>::Create(channel_.get(), cq, rpcmethod_Delete_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::DeleteReply>* Cli2Master::Stub::PrepareAsyncDeleteRaw(::grpc::ClientContext* context, const ::Rpc::DeleteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::DeleteReply>::Create(channel_.get(), cq, rpcmethod_Delete_, context, request, false);
}

::grpc::Status Cli2Master::Stub::Mkdir(::grpc::ClientContext* context, const ::Rpc::MkdirRequest& request, ::Rpc::MkdirReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Mkdir_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::MkdirReply>* Cli2Master::Stub::AsyncMkdirRaw(::grpc::ClientContext* context, const ::Rpc::MkdirRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::MkdirReply>::Create(channel_.get(), cq, rpcmethod_Mkdir_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::MkdirReply>* Cli2Master::Stub::PrepareAsyncMkdirRaw(::grpc::ClientContext* context, const ::Rpc::MkdirRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::MkdirReply>::Create(channel_.get(), cq, rpcmethod_Mkdir_, context, request, false);
}

::grpc::Status Cli2Master::Stub::GetSubDirNames(::grpc::ClientContext* context, const ::Rpc::GetSubDirNamesRequest& request, ::Rpc::GetSubDirNamesReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_GetSubDirNames_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::GetSubDirNamesReply>* Cli2Master::Stub::AsyncGetSubDirNamesRaw(::grpc::ClientContext* context, const ::Rpc::GetSubDirNamesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::GetSubDirNamesReply>::Create(channel_.get(), cq, rpcmethod_GetSubDirNames_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::GetSubDirNamesReply>* Cli2Master::Stub::PrepareAsyncGetSubDirNamesRaw(::grpc::ClientContext* context, const ::Rpc::GetSubDirNamesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::GetSubDirNamesReply>::Create(channel_.get(), cq, rpcmethod_GetSubDirNames_, context, request, false);
}

::grpc::Status Cli2Master::Stub::RequestVote(::grpc::ClientContext* context, const ::Rpc::RequestVoteArgs& request, ::Rpc::RequestVoteReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_RequestVote_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::RequestVoteReply>* Cli2Master::Stub::AsyncRequestVoteRaw(::grpc::ClientContext* context, const ::Rpc::RequestVoteArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::RequestVoteReply>::Create(channel_.get(), cq, rpcmethod_RequestVote_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::RequestVoteReply>* Cli2Master::Stub::PrepareAsyncRequestVoteRaw(::grpc::ClientContext* context, const ::Rpc::RequestVoteArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::RequestVoteReply>::Create(channel_.get(), cq, rpcmethod_RequestVote_, context, request, false);
}

::grpc::Status Cli2Master::Stub::AppendEntries(::grpc::ClientContext* context, const ::Rpc::AppendEntriesArgs& request, ::Rpc::AppendEntriesReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_AppendEntries_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::AppendEntriesReply>* Cli2Master::Stub::AsyncAppendEntriesRaw(::grpc::ClientContext* context, const ::Rpc::AppendEntriesArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::AppendEntriesReply>::Create(channel_.get(), cq, rpcmethod_AppendEntries_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::AppendEntriesReply>* Cli2Master::Stub::PrepareAsyncAppendEntriesRaw(::grpc::ClientContext* context, const ::Rpc::AppendEntriesArgs& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::AppendEntriesReply>::Create(channel_.get(), cq, rpcmethod_AppendEntries_, context, request, false);
}

Cli2Master::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::FindAddrRequest, ::Rpc::FindAddrReply>(
          std::mem_fn(&Cli2Master::Service::FindAddr), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::GetWriteAddrRequest, ::Rpc::GetWriteAddrReply>(
          std::mem_fn(&Cli2Master::Service::GetWriteAddr), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::StatRequest, ::Rpc::StatReply>(
          std::mem_fn(&Cli2Master::Service::Stat), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::RenameRequest, ::Rpc::RenameReply>(
          std::mem_fn(&Cli2Master::Service::Rename), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::DeleteRequest, ::Rpc::DeleteReply>(
          std::mem_fn(&Cli2Master::Service::Delete), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::MkdirRequest, ::Rpc::MkdirReply>(
          std::mem_fn(&Cli2Master::Service::Mkdir), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[6],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::GetSubDirNamesRequest, ::Rpc::GetSubDirNamesReply>(
          std::mem_fn(&Cli2Master::Service::GetSubDirNames), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[7],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::RequestVoteArgs, ::Rpc::RequestVoteReply>(
          std::mem_fn(&Cli2Master::Service::RequestVote), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2Master_method_names[8],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2Master::Service, ::Rpc::AppendEntriesArgs, ::Rpc::AppendEntriesReply>(
          std::mem_fn(&Cli2Master::Service::AppendEntries), this)));
}

Cli2Master::Service::~Service() {
}

::grpc::Status Cli2Master::Service::FindAddr(::grpc::ServerContext* context, const ::Rpc::FindAddrRequest* request, ::Rpc::FindAddrReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2Master::Service::GetWriteAddr(::grpc::ServerContext* context, const ::Rpc::GetWriteAddrRequest* request, ::Rpc::GetWriteAddrReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2Master::Service::Stat(::grpc::ServerContext* context, const ::Rpc::StatRequest* request, ::Rpc::StatReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2Master::Service::Rename(::grpc::ServerContext* context, const ::Rpc::RenameRequest* request, ::Rpc::RenameReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2Master::Service::Delete(::grpc::ServerContext* context, const ::Rpc::DeleteRequest* request, ::Rpc::DeleteReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2Master::Service::Mkdir(::grpc::ServerContext* context, const ::Rpc::MkdirRequest* request, ::Rpc::MkdirReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2Master::Service::GetSubDirNames(::grpc::ServerContext* context, const ::Rpc::GetSubDirNamesRequest* request, ::Rpc::GetSubDirNamesReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2Master::Service::RequestVote(::grpc::ServerContext* context, const ::Rpc::RequestVoteArgs* request, ::Rpc::RequestVoteReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2Master::Service::AppendEntries(::grpc::ServerContext* context, const ::Rpc::AppendEntriesArgs* request, ::Rpc::AppendEntriesReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* Cli2ChunkServer_method_names[] = {
  "/Rpc.Cli2ChunkServer/ReadChunk",
  "/Rpc.Cli2ChunkServer/WriteChunk",
  "/Rpc.Cli2ChunkServer/SayHello",
};

std::unique_ptr< Cli2ChunkServer::Stub> Cli2ChunkServer::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Cli2ChunkServer::Stub> stub(new Cli2ChunkServer::Stub(channel));
  return stub;
}

Cli2ChunkServer::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_ReadChunk_(Cli2ChunkServer_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_WriteChunk_(Cli2ChunkServer_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SayHello_(Cli2ChunkServer_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Cli2ChunkServer::Stub::ReadChunk(::grpc::ClientContext* context, const ::Rpc::ReadChunkRequest& request, ::Rpc::ReadChunkReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_ReadChunk_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::ReadChunkReply>* Cli2ChunkServer::Stub::AsyncReadChunkRaw(::grpc::ClientContext* context, const ::Rpc::ReadChunkRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::ReadChunkReply>::Create(channel_.get(), cq, rpcmethod_ReadChunk_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::ReadChunkReply>* Cli2ChunkServer::Stub::PrepareAsyncReadChunkRaw(::grpc::ClientContext* context, const ::Rpc::ReadChunkRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::ReadChunkReply>::Create(channel_.get(), cq, rpcmethod_ReadChunk_, context, request, false);
}

::grpc::Status Cli2ChunkServer::Stub::WriteChunk(::grpc::ClientContext* context, const ::Rpc::WriteChunkRequest& request, ::Rpc::WriteChunkReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_WriteChunk_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::WriteChunkReply>* Cli2ChunkServer::Stub::AsyncWriteChunkRaw(::grpc::ClientContext* context, const ::Rpc::WriteChunkRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::WriteChunkReply>::Create(channel_.get(), cq, rpcmethod_WriteChunk_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::WriteChunkReply>* Cli2ChunkServer::Stub::PrepareAsyncWriteChunkRaw(::grpc::ClientContext* context, const ::Rpc::WriteChunkRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::WriteChunkReply>::Create(channel_.get(), cq, rpcmethod_WriteChunk_, context, request, false);
}

::grpc::Status Cli2ChunkServer::Stub::SayHello(::grpc::ClientContext* context, const ::Rpc::HelloRequest& request, ::Rpc::HelloReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_SayHello_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::HelloReply>* Cli2ChunkServer::Stub::AsyncSayHelloRaw(::grpc::ClientContext* context, const ::Rpc::HelloRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::HelloReply>::Create(channel_.get(), cq, rpcmethod_SayHello_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::HelloReply>* Cli2ChunkServer::Stub::PrepareAsyncSayHelloRaw(::grpc::ClientContext* context, const ::Rpc::HelloRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::HelloReply>::Create(channel_.get(), cq, rpcmethod_SayHello_, context, request, false);
}

Cli2ChunkServer::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2ChunkServer_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2ChunkServer::Service, ::Rpc::ReadChunkRequest, ::Rpc::ReadChunkReply>(
          std::mem_fn(&Cli2ChunkServer::Service::ReadChunk), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2ChunkServer_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2ChunkServer::Service, ::Rpc::WriteChunkRequest, ::Rpc::WriteChunkReply>(
          std::mem_fn(&Cli2ChunkServer::Service::WriteChunk), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Cli2ChunkServer_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Cli2ChunkServer::Service, ::Rpc::HelloRequest, ::Rpc::HelloReply>(
          std::mem_fn(&Cli2ChunkServer::Service::SayHello), this)));
}

Cli2ChunkServer::Service::~Service() {
}

::grpc::Status Cli2ChunkServer::Service::ReadChunk(::grpc::ServerContext* context, const ::Rpc::ReadChunkRequest* request, ::Rpc::ReadChunkReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2ChunkServer::Service::WriteChunk(::grpc::ServerContext* context, const ::Rpc::WriteChunkRequest* request, ::Rpc::WriteChunkReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Cli2ChunkServer::Service::SayHello(::grpc::ServerContext* context, const ::Rpc::HelloRequest* request, ::Rpc::HelloReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* DataNodeControlerService_method_names[] = {
  "/Rpc.DataNodeControlerService/CheckHeartbeat",
  "/Rpc.DataNodeControlerService/CopyChunk",
};

std::unique_ptr< DataNodeControlerService::Stub> DataNodeControlerService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< DataNodeControlerService::Stub> stub(new DataNodeControlerService::Stub(channel));
  return stub;
}

DataNodeControlerService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_CheckHeartbeat_(DataNodeControlerService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_CopyChunk_(DataNodeControlerService_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status DataNodeControlerService::Stub::CheckHeartbeat(::grpc::ClientContext* context, const ::Rpc::HeartbeatRequest& request, ::Rpc::HeartbeatReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_CheckHeartbeat_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::HeartbeatReply>* DataNodeControlerService::Stub::AsyncCheckHeartbeatRaw(::grpc::ClientContext* context, const ::Rpc::HeartbeatRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::HeartbeatReply>::Create(channel_.get(), cq, rpcmethod_CheckHeartbeat_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::HeartbeatReply>* DataNodeControlerService::Stub::PrepareAsyncCheckHeartbeatRaw(::grpc::ClientContext* context, const ::Rpc::HeartbeatRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::HeartbeatReply>::Create(channel_.get(), cq, rpcmethod_CheckHeartbeat_, context, request, false);
}

::grpc::Status DataNodeControlerService::Stub::CopyChunk(::grpc::ClientContext* context, const ::Rpc::CopyChunkRequest& request, ::Rpc::CopyChunkReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_CopyChunk_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::CopyChunkReply>* DataNodeControlerService::Stub::AsyncCopyChunkRaw(::grpc::ClientContext* context, const ::Rpc::CopyChunkRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::CopyChunkReply>::Create(channel_.get(), cq, rpcmethod_CopyChunk_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::CopyChunkReply>* DataNodeControlerService::Stub::PrepareAsyncCopyChunkRaw(::grpc::ClientContext* context, const ::Rpc::CopyChunkRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::CopyChunkReply>::Create(channel_.get(), cq, rpcmethod_CopyChunk_, context, request, false);
}

DataNodeControlerService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      DataNodeControlerService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< DataNodeControlerService::Service, ::Rpc::HeartbeatRequest, ::Rpc::HeartbeatReply>(
          std::mem_fn(&DataNodeControlerService::Service::CheckHeartbeat), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      DataNodeControlerService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< DataNodeControlerService::Service, ::Rpc::CopyChunkRequest, ::Rpc::CopyChunkReply>(
          std::mem_fn(&DataNodeControlerService::Service::CopyChunk), this)));
}

DataNodeControlerService::Service::~Service() {
}

::grpc::Status DataNodeControlerService::Service::CheckHeartbeat(::grpc::ServerContext* context, const ::Rpc::HeartbeatRequest* request, ::Rpc::HeartbeatReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status DataNodeControlerService::Service::CopyChunk(::grpc::ServerContext* context, const ::Rpc::CopyChunkRequest* request, ::Rpc::CopyChunkReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* ReportDataNodeSerivce_method_names[] = {
  "/Rpc.ReportDataNodeSerivce/ReportDataNode",
};

std::unique_ptr< ReportDataNodeSerivce::Stub> ReportDataNodeSerivce::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ReportDataNodeSerivce::Stub> stub(new ReportDataNodeSerivce::Stub(channel));
  return stub;
}

ReportDataNodeSerivce::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_ReportDataNode_(ReportDataNodeSerivce_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status ReportDataNodeSerivce::Stub::ReportDataNode(::grpc::ClientContext* context, const ::Rpc::ReportDataNodeRequest& request, ::Rpc::ReportDataNodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_ReportDataNode_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::ReportDataNodeReply>* ReportDataNodeSerivce::Stub::AsyncReportDataNodeRaw(::grpc::ClientContext* context, const ::Rpc::ReportDataNodeRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::ReportDataNodeReply>::Create(channel_.get(), cq, rpcmethod_ReportDataNode_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::ReportDataNodeReply>* ReportDataNodeSerivce::Stub::PrepareAsyncReportDataNodeRaw(::grpc::ClientContext* context, const ::Rpc::ReportDataNodeRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::ReportDataNodeReply>::Create(channel_.get(), cq, rpcmethod_ReportDataNode_, context, request, false);
}

ReportDataNodeSerivce::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ReportDataNodeSerivce_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ReportDataNodeSerivce::Service, ::Rpc::ReportDataNodeRequest, ::Rpc::ReportDataNodeReply>(
          std::mem_fn(&ReportDataNodeSerivce::Service::ReportDataNode), this)));
}

ReportDataNodeSerivce::Service::~Service() {
}

::grpc::Status ReportDataNodeSerivce::Service::ReportDataNode(::grpc::ServerContext* context, const ::Rpc::ReportDataNodeRequest* request, ::Rpc::ReportDataNodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* ChunkServer2Chunkserver_method_names[] = {
  "/Rpc.ChunkServer2Chunkserver/MultiWrite",
  "/Rpc.ChunkServer2Chunkserver/CopyChunkWrite",
};

std::unique_ptr< ChunkServer2Chunkserver::Stub> ChunkServer2Chunkserver::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< ChunkServer2Chunkserver::Stub> stub(new ChunkServer2Chunkserver::Stub(channel));
  return stub;
}

ChunkServer2Chunkserver::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_MultiWrite_(ChunkServer2Chunkserver_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_CopyChunkWrite_(ChunkServer2Chunkserver_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status ChunkServer2Chunkserver::Stub::MultiWrite(::grpc::ClientContext* context, const ::Rpc::MultiWriteRequest& request, ::Rpc::MultiWriteReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_MultiWrite_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::MultiWriteReply>* ChunkServer2Chunkserver::Stub::AsyncMultiWriteRaw(::grpc::ClientContext* context, const ::Rpc::MultiWriteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::MultiWriteReply>::Create(channel_.get(), cq, rpcmethod_MultiWrite_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::MultiWriteReply>* ChunkServer2Chunkserver::Stub::PrepareAsyncMultiWriteRaw(::grpc::ClientContext* context, const ::Rpc::MultiWriteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::MultiWriteReply>::Create(channel_.get(), cq, rpcmethod_MultiWrite_, context, request, false);
}

::grpc::Status ChunkServer2Chunkserver::Stub::CopyChunkWrite(::grpc::ClientContext* context, const ::Rpc::CopyChunkWriteRequest& request, ::Rpc::CopyChunkWriteReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_CopyChunkWrite_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::Rpc::CopyChunkWriteReply>* ChunkServer2Chunkserver::Stub::AsyncCopyChunkWriteRaw(::grpc::ClientContext* context, const ::Rpc::CopyChunkWriteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::CopyChunkWriteReply>::Create(channel_.get(), cq, rpcmethod_CopyChunkWrite_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::Rpc::CopyChunkWriteReply>* ChunkServer2Chunkserver::Stub::PrepareAsyncCopyChunkWriteRaw(::grpc::ClientContext* context, const ::Rpc::CopyChunkWriteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::Rpc::CopyChunkWriteReply>::Create(channel_.get(), cq, rpcmethod_CopyChunkWrite_, context, request, false);
}

ChunkServer2Chunkserver::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ChunkServer2Chunkserver_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ChunkServer2Chunkserver::Service, ::Rpc::MultiWriteRequest, ::Rpc::MultiWriteReply>(
          std::mem_fn(&ChunkServer2Chunkserver::Service::MultiWrite), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      ChunkServer2Chunkserver_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< ChunkServer2Chunkserver::Service, ::Rpc::CopyChunkWriteRequest, ::Rpc::CopyChunkWriteReply>(
          std::mem_fn(&ChunkServer2Chunkserver::Service::CopyChunkWrite), this)));
}

ChunkServer2Chunkserver::Service::~Service() {
}

::grpc::Status ChunkServer2Chunkserver::Service::MultiWrite(::grpc::ServerContext* context, const ::Rpc::MultiWriteRequest* request, ::Rpc::MultiWriteReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status ChunkServer2Chunkserver::Service::CopyChunkWrite(::grpc::ServerContext* context, const ::Rpc::CopyChunkWriteRequest* request, ::Rpc::CopyChunkWriteReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace Rpc

