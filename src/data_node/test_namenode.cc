#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "./gen_code/gfs.grpc.pb.h"
#include <google/protobuf/timestamp.pb.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <unistd.h>//sleep
#include "gfs_common.h"
using std::endl;
using std::cout;
using std::string;

using grpc::Channel;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;
using grpc::ServerContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using gfs::GFS;
using gfs::ReadChunkRequest;
using gfs::ReadChunkReply;
using gfs::WriteChunkRequest;
using gfs::WriteChunkReply;
using gfs::ReportDataNodeRequest;
using gfs::ReportDataNodeReply;


class Test1Client final:public GFS::Service{
public:
     Status ReportDataNode(ServerContext* context,const ReportDataNodeRequest *request,ReportDataNodeReply *reply){

            //auto& new_datanode = request.datanode();
            std::string location_me=request->location();
            int chunk_size=request->available_size();
            auto chunkhandles=request->handles();
            std::cout<<"ip:port:"<<location_me<<std::endl;
            std::cout<<"chunk_size:"<<chunk_size<<std::endl;
            for(auto it:chunkhandles){
                std::cout<<"chunk_handle:"<<it<<std::endl;
            }
            //sleep(2);
            return Status::OK;
        

    }

};


int main(int argc,char** argv)
{

    //Test1Client client1(grpc::CreateChannel("localhost:50051",grpc::InsecureChannelCredentials()));
    //client1.ReportDataNode();
      std::string server_address("127.0.0.1:50058");
        Test1Client service;

        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        // Register "service" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *synchronous* service.
        builder.RegisterService(&service);
        // Finally assemble the server.
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever return.
        server->Wait();

    return 0;
}