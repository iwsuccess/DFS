## 文件说明

cli2ser.proto 文件：定义了 client 与 master(name_node)、chunk_server(data_node) 通信的数据格式。

client.cc：具体 API 相关实现。

## 生成 client 二进制可执行文件

前提：已配置好 grpc 相关环境。

1. 生成代码

   ```shell
   protoc --cpp_out=./ cli2ser.proto 
   protoc --grpc_out=./ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` cli2ser.proto
   ```

2. 编译代码命令

   ```shell
   g++ easylogging++.cc client.cc cli2ser.grpc.pb.cc cli2ser.pb.cc -o client `pkg-config --cflags protobuf grpc` `pkg-config --libs protobuf grpc++ grpc`  -pthread -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -I ../../include/

   ```

3. 执行 client 程序

   ```shell
   ./client
   ```

   