
data_node:
	g++ ../datanode.cc ../eazylog/src/easylogging++.cc ../gen_code/final.grpc.pb.cc ../gen_code/final.pb.cc -o data_node `pkg-config --cflags protobuf grpc` `pkg-config --libs protobuf grpc++ grpc`  -pthread -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -I ../gen_code/ -I ../eazylog/src/ -lleveldb
	#g++ ../report.cc ../eazylog/src/easylogging++.cc ../gen_code/gfs.grpc.pb.cc ../gen_code/namenode.pb.cc ../gen_code/namenode.grpc.pb.cc ../gen_code/gfs.pb.cc -o test_report `pkg-config --cflags protobuf grpc` `pkg-config --libs protobuf grpc++ grpc`  -pthread -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -I ../gen_code/ -I ../eazylog/src/ -lleveldb
#test:
	#g++ ../test.cc ../gen_code/final.grpc.pb.cc ../gen_code/final.pb.cc -o test `pkg-config --cflags protobuf grpc` `pkg-config --libs protobuf grpc++ grpc`  -pthread -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -I ../gen_code/ -I ../eazylog/src/ -lleveldb
