# leveldb数据库安装下载
### （1）git clone --recurse-submodules https://gitee.com/qcc-2017904375/leveldb.git
### （2）cd leveldb
### （3）mkdir build && cd build
### （4）cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
### （5）make
### （6）sudo make install

###  参考博客：https://blog.csdn.net/joelcat/article/details/89240584

# datanode编译运行，在datanode文件夹下
###  （1）./generate_grpc_file.sh
###  （2） mkdir build
###  （3） cd build
###  （4） make

# 运行datanode服务
### 在build文件夹下执行命令：`. go_on.sh`
### 会在后台运行5台chunk_server服务：47.97.218.168:50052 47.97.218.168:50053 47.97.218.168:50054 47.97.218.168:50055 47.97.218.168:50056 
### 然后进入log文件夹查看运行状态


