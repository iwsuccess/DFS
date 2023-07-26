#include <iostream>
#include <fstream>
#include <memory>
#include <inttypes.h>
#include <csignal>


#include <grpc++/grpc++.h>
#include <google/protobuf/timestamp.pb.h>
#include "./gen_code/gfs.grpc.pb.h"
#include "./gen_code/namenode.grpc.pb.h"
#include "datanode.h"
using namespace std;
int main()
{
    string master_address = "101.35.110.87:9001";
    cout<<"sdafas"<<endl;
    std::unique_ptr<Rpc::ReportDataNodeSerivce::Stub> stub_master ;
    stub_master= Rpc::ReportDataNodeSerivce::NewStub(grpc::CreateChannel(master_address,grpc::InsecureChannelCredentials()));

    Rpc::ReportDataNodeRequest request;
    Rpc::ReportDataNodeReply reply;
    request.set_location("127.0.0.1:10000");
    request.set_available_size(100);
    while(true){
        ClientContext context;
        grpc::Status status = stub_master->ReportDataNode(&context, request, &reply);
        if(status.ok()){
            cout<<"ok"<<endl;
        }
        else{
            cout<<"false message= "<<status.error_code()<<endl;
        }
        sleep(3);
    }
    

}
