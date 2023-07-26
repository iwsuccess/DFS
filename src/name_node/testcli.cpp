#include "NameNode.grpc.pb.h"
#include"NameNode.pb.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include <iostream>
#include<mutex>
using namespace Rpc;

using namespace std;


#include<string>

std::shared_ptr<grpc::Channel> channel1 =grpc::CreateChannel("101.35.110.87:9000", grpc::InsecureChannelCredentials());
std::shared_ptr<grpc::Channel> channel2 =grpc::CreateChannel("101.35.110.87:9001", grpc::InsecureChannelCredentials());
std::shared_ptr<grpc::Channel> channel3 =grpc::CreateChannel("localhost:9002", grpc::InsecureChannelCredentials());
       // 创建一个stub
std::unique_ptr<Rpc::Cli2Master::Stub> stub1 = Rpc::Cli2Master::NewStub(channel1);
std::unique_ptr<Rpc::Cli2Master::Stub> stub2 = Rpc::Cli2Master::NewStub(channel2);
std::unique_ptr<Rpc::Cli2Master::Stub> stub3 = Rpc::Cli2Master::NewStub(channel3);
void mkdirtest()
{
      MkdirRequest request;
           // 创建一个响应对象，用于解包响要接收的应数据
      MkdirReply   reply;
           
           // 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为                               
      grpc::ClientContext context1;
      request.set_new_file_path("/test");
     stub1->Mkdir(&context1,request,&reply);

      grpc::ClientContext context2;
     stub2->Mkdir(&context2,request,&reply);

}
void mkdirtest2()
{
      MkdirRequest request;
           // 创建一个响应对象，用于解包响要接收的应数据
      MkdirReply   reply;
           
           // 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为                               
      grpc::ClientContext context1;
      request.set_new_file_path("/test/testmulu");
     stub1->Mkdir(&context1,request,&reply);

      grpc::ClientContext context2;
     stub2->Mkdir(&context2,request,&reply);

}
void stat()
{
      StatRequest request;
           // 创建一个响应对象，用于解包响要接收的应数据
      StatReply   reply;
           
           // 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为                               
      grpc::ClientContext context1;
      request.set_filepath("/test/test");
     stub1->Stat(&context1,request,&reply);

      grpc::ClientContext context2;
     stub2->Stat(&context2,request,&reply);
}
void stat2()
{
      StatRequest request;
           // 创建一个响应对象，用于解包响要接收的应数据
      StatReply   reply;
           
           // 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为                               
      grpc::ClientContext context1;
      request.set_filepath("/test/testmulu");
     stub1->Stat(&context1,request,&reply);

      grpc::ClientContext context2;
     stub2->Stat(&context2,request,&reply);
}
void deltest()
{
     Rpc::DeleteRequest request;
           // 创建一个响应对象，用于解包响要接收的应数据
     Rpc::DeleteReply   reply;
           
           // 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为                               
     grpc::ClientContext context1;
     request.set_remote_file_path("/testw");
     stub1->Delete(&context1,request,&reply);

     grpc::ClientContext context2;
     stub2->Delete(&context2,request,&reply);
}
void deltest2()
{
     //删除不存在
       Rpc::DeleteRequest request;
           // 创建一个响应对象，用于解包响要接收的应数据
     Rpc::DeleteReply   reply;
           
           // 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为                               
     grpc::ClientContext context1;
     request.set_remote_file_path("/testm");
     stub1->Delete(&context1,request,&reply);

     grpc::ClientContext context2;
     stub2->Delete(&context2,request,&reply);
}
void getwrite()
{
     Rpc::GetWriteAddrRequest request;
           // 创建一个响应对象，用于解包响要接收的应数据
     Rpc::GetWriteAddrReply   reply;
           
           // 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为                               
     grpc::ClientContext context1;
     request.set_remote_file_path("/test/test");
     request.set_write_data_size(65*1024);
     stub1->GetWriteAddr(&context1,request,&reply);
     // cout<<"r1"<<endl;
     // for(int i=0;i<reply.chunk_handles().size();i++)
     // {
     //      cout<<reply.chunk_handles()[i]<<":"<<reply.addrs()[i]<<endl;
     // }
     grpc::ClientContext context2;
     stub2->GetWriteAddr(&context2,request,&reply);
     //  cout<<"r2"<<endl;
     // for(int i=0;i<reply.chunk_handles().size();i++)
     // {
     //      cout<<reply.chunk_handles()[i]<<":"<<reply.addrs()[i]<<endl;
     // }
}
void rename()
{
     Rpc::RenameRequest request;
           // 创建一个响应对象，用于解包响要接收的应数据
     Rpc::RenameReply  reply;
           
           // 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为                               
     grpc::ClientContext context1;
     request.set_src_path("/testm");
     request.set_des_path("/testmulu");
     stub1->Rename(&context1,request,&reply);

     grpc::ClientContext context2;
     stub2->Rename(&context2,request,&reply);
}
int main()
{
     mkdirtest();
    mkdirtest2();
      stat2();
      getwrite();
      stat();
//     rename();
    return 0;
}