
#include "NameNode.grpc.pb.h"
#include"NameNode.pb.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <iostream>
#include<mutex>


using namespace Rpc;

using namespace std;
std::shared_ptr<grpc::Channel> channel1 =grpc::CreateChannel("101.35.110.87:9000", grpc::InsecureChannelCredentials());
std::shared_ptr<grpc::Channel> channel2 =grpc::CreateChannel("101.35.110.87:9001", grpc::InsecureChannelCredentials());
std::unique_ptr<Rpc::ReportDataNodeSerivce::Stub> stub1 = Rpc::ReportDataNodeSerivce::NewStub(channel1);
std::unique_ptr<Rpc::ReportDataNodeSerivce::Stub> stub2 = Rpc::ReportDataNodeSerivce::NewStub(channel2);
// class Test1Client final:public GFS::Service{
// public:
//      Status ReportDataNode(ServerContext* context,const ReportDataNodeRequest *request,ReportDataNodeReply *reply){

//             //auto& new_datanode = request.datanode();
//             std::string location_me=request->location();
//             int chunk_size=request->available_size();
//             auto chunkhandles=request->handles();
//             std::cout<<"ip:port:"<<location_me<<std::endl;
//             std::cout<<"chunk_size:"<<chunk_size<<std::endl;
//             for(auto it:chunkhandles){
//                 std::cout<<"chunk_handle:"<<it<<std::endl;
//             }
//             //sleep(2);
//             return Status::OK;


//     }

// };

void test()
{

    grpc::ClientContext context;
    ReportDataNodeRequest request; 
    ReportDataNodeReply  reply; 
                    //ip:port

                  //拥有的chunkhandle


    auto status=stub1->ReportDataNode(&context,request,&reply);
     if(status.ok()){
            cout<<"ok"<<endl;
        }
    cout<<status.error_code()<<endl;
                  
}
int main(int argc,char** argv)
{

    
    test();
    return 0;
}