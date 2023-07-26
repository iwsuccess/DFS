#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "./gen_code/final.grpc.pb.h"
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


using Rpc::ReadChunkRequest;
using Rpc::ReadChunkReply;
using Rpc::WriteChunkRequest;
using Rpc::WriteChunkReply;
// using gfs::ReportDataNodeRequest;
// using gfs::ReportDataNodeReply;


class Test1Client final:public Rpc::Cli2ChunkServer::Service{
public:
    // create stub
    Test1Client(std::shared_ptr<Channel> channel):stub_(Rpc::Cli2ChunkServer::NewStub(channel)){}
    void GetReplyMsg(int chunkhandle,int offset,int lenth)
    {
        ReadChunkRequest filename_;
        ReadChunkReply read_reply;
        //filename_.set_filename(tmp);
        filename_.set_chunkhandle(chunkhandle);
        filename_.set_offset(offset);
        filename_.set_length(lenth);
        GetOneData(filename_,&read_reply);
    }
    void write_chunk_test(int chunkhandle,int offset,std::string content){

            WriteChunkRequest request;
            WriteChunkReply reply;
            request.set_client_id(42);
            //request.set_filename(tmp);
            request.set_chunkhandle(chunkhandle);
            request.set_offset(offset);
            request.set_data(content);
            string *s;
            std::vector<string>ip;
            ip.push_back("127.0.0.1:50053");
            ip.push_back("127.0.0.1:50054");
            for(auto it : ip)
            {
                s = request.add_locations();
                *s = it;
            }
         write_Data(request,&reply);
    }



private:
    bool GetOneData(const ReadChunkRequest& filename_,ReadChunkReply* read_reply)
    {
        ClientContext context;
        Status status=stub_->ReadChunk(&context,filename_,read_reply);
        if(!status.ok())
        {
            cout<<"GetData rpc failed."<<endl;
            return false;
        }
        if(read_reply->data().empty())
        {
            cout<<"message empty."<<endl;
            return false;
        }
        else
        {
            cout<<"MsgReply:"<<read_reply->data()<<endl;
        }
        return true;
    }

    bool write_Data(const WriteChunkRequest& write_request,WriteChunkReply* wirte_reply)
    {
        ClientContext context;
        Status status=stub_->WriteChunk(&context,write_request,wirte_reply);
        if(!status.ok())
        {
            cout<<"GetData rpc failed."<<"error code:"<<status.error_code()<<endl;
            return false;
        }
       
            cout<<"writen lenth:"<<wirte_reply->bytes_read()<<endl;
        return true;
    }

    std::unique_ptr<Rpc::Cli2ChunkServer::Stub> stub_;
};

//参数说明
// ./test 
//第一个参数（选择服务器chunkserver,0(127.0.0.1:50051) 1(127.0.0.1:50052) 2(127.0.0.1:50053))
//第二个参数 0:read  1:write
//第三个参数 chunkhandle
//第四个参数 offset
//第五个参数 如果是读：length,如果是写：data
int main(int argc,char** argv)
{

    // create a gRPC channel for our stub
   // grpc::CreateChannel("locakhost:50051",grpc::InsecureChannelCredentials());
    int chunk_server=atoi(argv[1]);//0(127.0.0.1:50051) 1(127.0.0.1:50052) 2(127.0.0.1:50053)   
    int type=atoi(argv[2]);


    if(chunk_server==0)
    {
            Test1Client client1(grpc::CreateChannel("localhost:50052",grpc::InsecureChannelCredentials()));
              cout<<"====================="<<endl;
            if(type==0)//read
            {
                //std::string read_path=argv[2];
                int chunkhandle=atoi(argv[3]);
                int offset=atoi(argv[4]);
                int lenth=atoi(argv[5]);
                client1.GetReplyMsg(chunkhandle,offset,lenth);
            

            }
            else{ //write

                //std::string write_path=argv[2];
                int chunkhandle=atoi(argv[3]);
                int offset=atoi(argv[4]);
                std::string content=argv[5];
                client1.write_chunk_test(chunkhandle,offset,content);
        
            }
        
        
    }
    else if(chunk_server==1){
            Test1Client client2(grpc::CreateChannel("localhost:50053",grpc::InsecureChannelCredentials()));
              cout<<"====================="<<endl;
            if(type==0)//read
            {
                //std::string read_path=argv[2];
                int chunkhandle=atoi(argv[3]);
                int offset=atoi(argv[4]);
                int lenth=atoi(argv[5]);
                client2.GetReplyMsg(chunkhandle,offset,lenth);
              
            }
            else{ //write

                //std::string write_path=argv[2];
                int chunkhandle=atoi(argv[3]);
                int offset=atoi(argv[4]);
                std::string content=argv[5];
                client2.write_chunk_test(chunkhandle,offset,content);
    

            }


    }
        
    else
        {
            Test1Client client3(grpc::CreateChannel("localhost:50054",grpc::InsecureChannelCredentials()));
            cout<<"====================="<<endl;
              if(type==0)//read
            {
                //std::string read_path=argv[2];
                int chunkhandle=atoi(argv[3]);
                int offset=atoi(argv[4]);
                int lenth=atoi(argv[5]);
                client3.GetReplyMsg(chunkhandle,offset,lenth);
              
            }
            else{ //write

                //std::string write_path=argv[2];
                int chunkhandle=atoi(argv[3]);
                int offset=atoi(argv[4]);
                std::string content=argv[5];
                client3.write_chunk_test(chunkhandle,offset,content);
    
            }
            

        }


    // if(type==0)//read
    // {
    //      //std::string read_path=argv[2];
    //     int chunkhandle=atoi(argv[3]);
    //     int offset=atoi(argv[4]);
    //     int lenth=atoi(argv[5]);
    //     if(chunk_server==0)
    //         client1.GetReplyMsg(chunkhandle,offset,lenth);
    //     else if(chunk_server==1)
    //         client2.GetReplyMsg(chunkhandle,offset,lenth);
    //     else
    //         client3.GetReplyMsg(chunkhandle,offset,lenth);

    // }
    // else{ //write

    //     //std::string write_path=argv[2];
    //     int chunkhandle=atoi(argv[3]);
    //     int offset=atoi(argv[4]);
    //     std::string content=argv[5];
    //     if(chunk_server==0)
    //         client1.write_chunk_test(chunkhandle,offset,content);
    //     else if(chunk_server==1)
    //         client2.write_chunk_test(chunkhandle,offset,content);
    //     else    
    //         client3.write_chunk_test(chunkhandle,offset,content);


    // }

    return 0;
}