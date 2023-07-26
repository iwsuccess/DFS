NameNode:main.c NameNode.grpc.pb.cc NameNode.pb.cc
	g++ $^ -o  $@ -std=c++11 -I. `pkg-config --cflags protobuf grpc`     `pkg-config --libs protobuf grpc++ grpc`     -pthread -Wl,--no-as-needed -lgrpc++_reflection -W
.PHONY:clean
clean:
	rm NameNode