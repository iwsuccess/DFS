
#include"NameNode.cc"

// static void Usage(std::string proc)
// {
//     std::cout << "Usage:\n\t" << proc << " port" << std::endl;;
// }

int main()
 {

    grpc::ServerBuilder builder;
  //  // 添加监听的地址和端口，后一个参数用于设置认证方式，这里选择不认证
    builder.AddListeningPort("0.0.0.0:9000",grpc::InsecureServerCredentials());
  
  // // 创建服务对象
    ServiceImpl service(0);
    DataNodeControlServiceImpl serviced;
     ReportDataNodeServiceImpl servicer;
  // 注册服务
   builder.RegisterService(&service);
   builder.RegisterService(&serviced);
builder.RegisterService(&servicer);
  // 构建服务器
   std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

   // 进入服务处理循环（必须在某处调用server->Shutdown()才会返回）
   server->Wait();

   return 0;
   
 } 
