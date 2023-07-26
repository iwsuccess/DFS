/* 
1. 首先写数据库
已知服务器A，B
2. 然后给出{1.A中chunk_handle, 2.B的location}
*/
#include <iostream>
#include <fstream>
#include <memory>
#include <inttypes.h>
#include <csignal>
#include <unistd.h>

#include <grpc++/grpc++.h>
#include <google/protobuf/timestamp.pb.h>
#include "./gen_code/gfs.grpc.pb.h"
//#include "datanode.h"

using namespace gfs;
using grpc::ClientContext;
int main()
{
    std::string locations[4] = {"127.0.0.1:50051","127.0.0.1:50054","127.0.0.1:50053","127.0.0.1:50052"};
    std::string A = locations[0];
    std::string B = locations[3];
    std::string data[2] = {"snfjsdkfsd", "shdakjdfhaskjhfj"};
    std::unique_ptr<gfs::GFS::Stub> stub_master;
    for(int j = 0;j < 2;j++ ){
        ClientContext context;
        WriteChunkReply reply;
        WriteChunkRequest request;
        request.set_client_id(j);
        request.set_data(data[j]);
        request.set_chunkhandle(j);
        request.set_offset(0);
        for(int i = 0;i < 3;i++){
            std::string *temp_location = request.add_locations();
            *temp_location = locations[(i+j) % 4];
        }
        stub_master = gfs::GFS::NewStub(grpc::CreateChannel(locations[j], grpc::InsecureChannelCredentials()));
        stub_master -> WriteChunk(&context, request, &reply);
    }

    //chunkhandle == 0，data[0] --> server0,1,2
    //data[1] --> server1,2,3;
    // 将A中的data[0],写入B中
    stub_master = gfs::GFS::NewStub(grpc::CreateChannel(A, grpc::InsecureChannelCredentials()));
    CopyChunkRequest copyrequest;
    CopyChunkReply copyreply;
    ClientContext copycontext;
    copyrequest.set_chunk_handle(0);
    copyrequest.set_location(B);
    stub_master->CopyChunk(&copycontext, copyrequest, &copyreply);  // 原来10003只有chunkhandle == 1， 现在由于同步之后有chunkhandle == 0了
    HeartbeatRequest heartrequest;
    HeartbeatReply heartreply;
    while(true){
        ClientContext heartcontext;
        grpc::Status status_heart = stub_master-> CheckHeartbeat(&heartcontext, heartrequest, &heartreply);
        if(status_heart.ok()){
            std::cout<<"heart ok"<<std::endl;
        }
        else {
            std::cout<<"heart no ok"<<std::endl;
        }
        sleep(2);
    }
   
    return 0;
}