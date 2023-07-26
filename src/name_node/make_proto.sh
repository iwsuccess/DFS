PROTONAME="*.proto"
protoc -I ./ --cpp_out=. ${PROTONAME}
protoc -I ./ --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ${PROTONAME}

