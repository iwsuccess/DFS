# NameNode
## 相关api
### GetNode
通过filepath路径，查找目录树，获得元数据信息
### FindAddr 
通过目录树，查找文件名所分成的chunk_handle和chunk_server的location信息
### GetWriteAddr
上传文件到目录树中，分割文件为对应chunk，初始化元数据信息，返回chunk_server的location
### Stat 
获取文件的元数据信息
### Rename
将文件在目录树中的路径重命名
### Delete
删除传入路径的文件
### Mkdir
在该路径下创建目录
### GetSubDirNames
获取该目录下的文件元数据信息

### 编译
执行
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

g++ test1.c -o test1  NameNode.grpc.pb.cc NameNode.pb.cc easylogging++.cc   -std=c++11 -I. `pkg-config --cflags protobuf grpc`     `pkg-config --libs protobuf grpc++ grpc`     -pthread -Wl,--no-as-needed -lgrpc++_reflection -W 

g++ test2.c -o test2  NameNode.grpc.pb.cc NameNode.pb.cc easylogging++.cc   -std=c++11 -I. `pkg-config --cflags protobuf grpc`     `pkg-config --libs protobuf grpc++ grpc`     -pthread -Wl,--no-as-needed -lgrpc++_reflection -W 

