rm -rf gen_code
mkdir gen_code
protoc -I ./ --grpc_out=./gen_code --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./final.proto
protoc -I ./ --cpp_out=./gen_code ./final.proto