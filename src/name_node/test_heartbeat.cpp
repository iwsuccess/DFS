#include <iostream>
#include <chrono>
#include <namenode_datanode_service.h>
#include <datanode_manager.h>

int main() {
    auto& datanode_manager = DataNodeManager::GetInstance();
    Rpc::DataNode datanode_1;
    datanode_1.set_location("localhost:50050");
    datanode_manager.RegisterDataNode(std::make_shared<Rpc::DataNode>(datanode_1));
    Rpc::DataNode datanode_2;
    datanode_2.set_location("localhost:50051");
    datanode_manager.RegisterDataNode(std::make_shared<Rpc::DataNode>(datanode_2));
    Rpc::DataNode datanode_3;
    datanode_3.set_location("localhost:50052");;
    datanode_manager.RegisterDataNode(std::make_shared<Rpc::DataNode>(datanode_3));
    // start heartbeat
    datanode_manager.StartHeartbeat();
    std::this_thread::sleep_for(std::chrono::seconds(20));
}
