#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "./cli2ser.grpc.pb.h"
#include "./cli2ser.pb.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP
#include <grpcpp/grpcpp.h>
using namespace Rpc;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
// using namespace Rpc;

// CHUNKSIZE should be define globally, here for test.
#define CHUNKSIZE 64
#define CLINETRETRYTIMES 4

class Client {
private:
  std::unique_ptr<Cli2Master::Stub> cli_master_stub_;
  std::unique_ptr<Cli2Master::Stub> cli_master_stub_backup_;
  std::map<std::string, std::unique_ptr<Cli2ChunkServer::Stub>>
      cli_chunkserver_stub_map_;
  int client_id_;

public:
  Client(std::shared_ptr<Channel> channel,
         std::shared_ptr<Channel> channel_backup, int client_id)
      : cli_master_stub_(Cli2Master::NewStub(channel)),
        cli_master_stub_backup_(Cli2Master::NewStub(channel_backup)),
        client_id_(client_id) {
    LOG(INFO) << "client_id: " << client_id_ << " init.";
  }

  // TODO:
  // 1. All APIs return value -> error code or error info.
  // 2. Get(), Put() have no consideration of data copies.
  // 3. Every RPC invoke, should do corresponding handlings by specific errors.

  // Get(): get the remote file, and write it into the local_file_path.
  void Get(const std::string &remote_file_path,
           const std::string &local_file_path);

  // Put(): writing local file into remote file
  void Put(const std::string &remote_file_path,
           const std::string &local_file_path);

  // Stat(): get the remote file's size and path info, if it does not exist,
  // report an exception.
  void Stat(const std::string &file_path);

  // Reanme(): rename the remote file(`src_path`) to `des_path`
  void Rename(const std::string &src_path, const std::string &des_path);

  // Delete(): delete the remote file.
  void Delete(const std::string &remote_file_path);

  // Mkdir(): create a new remote file.
  void Mkdir(const std::string &new_file_path);

  // List(): get information about the files in the remote directory.
  void List(const std::string &remote_file_path);

  void TestCreate(std::string remote_file_path);
  void TestWrite(std::string remote_file_path);
  void TestHello(std::string name);

public:
  // ReadChunk():
  // `addr`: chunk server's ip and port.
  // `chunk_handle`: identifies the chunk to be read.
  // `offset`: where to begin reading.
  // `length`: length of data to be read.
  // `res`: store all the data read out.
  Status ReadChunk(const std::string &addr, int64_t chunk_handle,
                   int64_t offset, size_t length, std::string *res);

  // WriteChunk():
  // `addr`: chunk server's ip and port.
  // `chunk_handle`: identifies the chunk to be written.
  // `offset`: where to begin writing.
  // `length`: length of data to be written.
  // `data`: the data to be written.
  Status WriteChunk(const std::string &addr, int64_t chunk_handle,
                    int64_t offset, size_t length,
                    const std::vector<std::string> &locations,
                    const std::string &data);

  Cli2ChunkServer::Stub *GetCliChunkServerStub(const std::string &addr);

  // GetFileInfo(): returns the file's path and the file size.
  // if return empty file'path means no this file.
  std::pair<std::string, int> GetFileInfo(const std::string &filename);

  // ParseIpPort(): parse the ip port info from the addr.
  std::pair<std::string, unsigned short> ParseIpPort(const std::string &addr);

  // GetFileSize(): get the local file size.
  int GetFileSize(const std::string &filename);
};
