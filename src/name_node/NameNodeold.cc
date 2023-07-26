
#include "NameNode.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <memory>
#include<algorithm>
#include <iostream>
#include<string>
#include"NameNode.h"
#include"common.h"

using namespace grpc;
//本机
ServiceImpl::ServiceImpl(int index)
{
   for(int i=8083;i<8200;i++)
   {
      vport.push_back(i);
   }
   GF=new GetFiles("/");
   //master三副本
   for(int i=0;i<3;i++)
   {
      std::shared_ptr<grpc::Channel> channel =grpc::CreateChannel("localhost:900"+to_string(i), grpc::InsecureChannelCredentials());
       // 创建一个stub
      std::shared_ptr<Rpc::Cli2Master::Stub> stub = Rpc::Cli2Master::NewStub(channel);
      _Stub.push_back(stub);
   }

}
ServiceImpl::~ServiceImpl()
{
   delete GF;
   
}

int ihash(string s)
{
   uint64_t ret=0;
   for(auto& x:s)
   {
      ret=ret*31+x-'a';
   }
   return ret&0x7fffffff;

}

FileTree* ServiceImpl::GetNode(string filepath)
{

   //GF->Get_FileMetadata("/A/B",node);
   FileTree node;
   if(GF->Get_FileMetadata(filepath,node)==false)
   {
      //没找到
       std::cout<<"没找到node"<<std::endl;
       return nullptr;
   }
   FileTree* ret=new FileTree(node);
   return ret;
}

Status ServiceImpl::FindAddr(grpc::ServerContext*  context,const Rpc::FindAddrRequest* request,Rpc::FindAddrReply* response)
{
  std::lock_guard<std::mutex> lg(_mtx);
   std::string filepath=request->filename();
    FileTree* node=GetNode(filepath);
   if(node==nullptr)
   {
      return Status(NOT_FOUND,"没有找到文件");
   }

   auto chunk_handles=node->chunk_handles;
   for(size_t  i=0;i<chunk_handles.size();i++)
   {
      auto chunk_handle=chunk_handles[i];
      auto& chunk=_chunks[chunk_handle];//chunk*

      response->add_chunk_handles(chunk_handle);
      response->add_addrs(chunk->location[0].location);
   }
   return Status::OK;
}

//上传文件 PUT 文件
 Status ServiceImpl::GetWriteAddr(grpc::ServerContext*  context,const Rpc::GetWriteAddrRequest* request,Rpc::GetWriteAddrReply* response)
 {
  std::lock_guard<std::mutex> lg(_mtx);
    std::string filepath =request->remote_file_path();
    FileTree* node=GetNode(filepath);
    if(node!=nullptr)
    {
      return Status(ALREADY_EXISTS,"文件已经存在");
    }

    int64_t write_size=request->write_data_size();

    std::string filename=GetFileName(filepath);

    FileTree file=FileTree(filename,filepath,write_size,0);
     GF->Add_Path(filepath,file);
    auto& chandle=node->chunk_handles;
    //64M
    for(int i=0;write_size>0;i++)
    {
        auto size=min(write_size,(int64_t)CHUNK_SIZE);
        chandle[i]=hanlde;
        Chunk* c=new Chunk();//chunk
        c->chunk_size=size;
        std::string host="localhost:"+to_string(ihash(filepath+to_string(i))%vport.size());
        c->location[0].location=host;//primary 只分配了
         _chunks[chandle[i]]=c;
         write_size-=size;
         response->add_chunk_handles(hanlde);
         response->add_addrs(host);
         hanlde++;
    }
    return Status::OK;
 }

Status ServiceImpl::Stat(grpc::ServerContext*  context,const Rpc::StatRequest* request,Rpc::StatReply* response)
{
   std::lock_guard<std::mutex> lg(_mtx);
   std::string filepath=request->filename();
   FileTree* node=GetNode(filepath);
   if(node==nullptr)
   {
      response->set_existed(false);
      return Status(NOT_FOUND,"没有找到文件");
   }
   
   response->set_existed(true);
   response->set_file_path(node->file_name);
   response->set_file_size(node->size);
   return Status::OK;
}
Status ServiceImpl::Rename(grpc::ServerContext*  context,const Rpc::RenameRequest* request,Rpc::RenameReply* response)
{
   auto dest=request->des_path();
   std::string src=request->src_path();
   std::lock_guard<std::mutex> lg(_mtx);
   FileTree* node=GetNode(src);

   if(node==nullptr)
   {
       return Status(NOT_FOUND,"没有找到文件");
   }
   //rename
   if(GF->Rename_Path(src,dest)==false)
   {
       return Status(NOT_FOUND,"RENAME失败");
   }

   return Status::OK;
}
Status ServiceImpl::Delete(grpc::ServerContext*  context,const Rpc::DeleteRequest* request,Rpc::DeleteReply* response)
{
   auto filepath=request->remote_file_path();
   std::lock_guard<std::mutex> lg(_mtx);
   if(GF->Del_Path(filepath)==false)
   {
      return  Status(NOT_FOUND,"没有找到删除的文件");
   }
   return Status::OK;
}

Status ServiceImpl::Mkdir(grpc::ServerContext*  context,const Rpc::MkdirRequest* request,Rpc::MkdirReply* response)
{
   auto& path=request->new_file_path();
   //目录不用创建chunk
   std::lock_guard<std::mutex> lg(_mtx);
   if(GF->Add_Path(path,FileTree(GetFileName(path),path,1,1))==false)
   {
      return Status(ALREADY_EXISTS,"已经有该目录了");
   }
   return Status::OK;
}
//list
Status ServiceImpl::GetSubDirNames(grpc::ServerContext*  context,const Rpc::GetSubDirNamesRequest* request,Rpc::GetSubDirNamesReply* response)
{
   // /A  /B
   std::lock_guard<std::mutex> lg(_mtx);
   auto dirpath=request->file_path();
   FileTree* node=GetNode(dirpath);

   if(node==nullptr)
   {
       return Status(NOT_FOUND,"没有找到目录");
   }
   
   std::vector<FileTree> ret=GF->Get_Metadata(dirpath);
   for(auto& x:ret)
   {
       response->add_subdir_file_names(x.file_name);
   }
   //返回信息
   return Status::OK;
}
