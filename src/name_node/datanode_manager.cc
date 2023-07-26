#include "datanode_manager.h"

void DataNodeManager::StartHeartbeat()
{
    // create the background thread to run the hearbeat task
    LOG(INFO) << "create background thread for heartbeat";
    heartbeat_thread_ = std::unique_ptr<std::thread>(
        new std::thread(&DataNodeManager::HeartbeatTask, this));
}

void DataNodeManager::HeartbeatTask()
{
    LOG(INFO) << "heartbeat task is now runnning in the background thread";
    while (true)
    {
        // send heartbeat request

        auto tmp_datanode_map = datanode_map_;
        for (auto &datanode : tmp_datanode_map)
        {

            Rpc::HeartbeatRequest request;
            auto client = GetOrCreateDataNodeControlClient(datanode.first);
            // LOG(INFO) << "Sending heartbeat message to datanode :"
            //           << datanode.first;

            grpc::Status reply;
            for (int attempt = 1; attempt <= max_heartbeat_attempts; ++attempt)
            {
                reply = client->SendRequest(request);
                if (reply.ok())
                {
                    // LOG(INFO) << "Received heartbeat reply from datanode "
                    //           << datanode.first;
                    break;
                }
                else
                {
                    LOG(INFO) << "Failed to receive heartbeat from datanode: "
                              << datanode.first << " after " << attempt
                              << " attempt(s).";
                }
            }
            // after max attempts of try, if this datanode still not reply
            // we unregister this datanode
            if (!reply.ok())
            {
                LOG(INFO) << "Unregister datanode "
                          << datanode.first;
                UnRegisterDataNode(datanode.first);
            }
        }

        // LOG(INFO) << "heartbeat task is going to sleep for 3 second";

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

Rpc::Chunk DataNodeManager::AllocateDataNode(const int64_t &chunk_handle, const ushort &datanode_count,int64_t chunk_size)
{
    // check if we have allocated datanode for this previously
    if (chunk_map_.count(chunk_handle))
    {
        LOG(INFO) << "DataNodes have been previous allocated for chunk:" << chunk_handle;
        return chunk_map_[chunk_handle];
    }
    Rpc::Chunk new_chunk;
    new_chunk.set_chunk_handle(chunk_handle);

    ushort allocated_datanode_count = 0;
    for (auto iter = datanode_priority_list_.cbegin();
         iter != datanode_priority_list_.cend() && allocated_datanode_count < datanode_count;
         ++iter, ++allocated_datanode_count)
    {
        auto datanode = *iter;
        // here size means mb
        // should communicate to get a con consensus
        int32_t new_available_size = datanode->available_size() - 64;
        if (new_available_size <= 0)
        {
            // break iter
            // can't find any datanode with larger space
            break;
        }
        new_chunk.add_location(datanode->location());
        datanode->set_available_size(new_available_size);
        
        datanode->add_stored_chunk_handles(chunk_handle);
    }

    if (allocated_datanode_count > 0)
    {
        // we did some allocations and changed some datanodes available space
        // so should reorder them
        datanode_priority_list_.sort(
            DataNodeAvailableSizeGreaterCompare());
    }
    if (allocated_datanode_count < datanode_count)
    {
        LOG(INFO) << "available datanode did not satisfy our requirement" << std::endl
                  << "allocated datanode count: " << allocated_datanode_count << std::endl
                  << "requirement datanode count: " << datanode_count;
    }
     new_chunk.set_size(chunk_size);
    chunk_map_.insert({chunk_handle, new_chunk});
    return new_chunk;
}

bool DataNodeManager::RegisterDataNode(const std::shared_ptr<Rpc::DataNode> datanode)
{
    auto result = datanode_map_.insert({datanode->location(), datanode});
    // if successfully inserted, means not previously registered
    if (result.second)
    {
        // update the chunk map for the chunks owned by this datanode
        for (int i = 0; i < datanode->stored_chunk_handles_size(); ++i)
        {
            chunk_map_[datanode->stored_chunk_handles(i)].add_location(datanode->location());
        }
        datanode_priority_list_.push_back(datanode);
        // keep priority list order
        datanode_priority_list_.sort(
            DataNodeAvailableSizeGreaterCompare());
        LOG(INFO) << "Register new datanode " << datanode->location();
    }
    return result.second;
}

void DataNodeManager::UpdateDataNode(const Rpc::DataNode &new_datanode)
{
    auto location = new_datanode.location();
    if (!datanode_map_.count(location))
    {
        // if not found, return
        return;
    }
    // LOG(INFO) << "Updating datanode " << location;
    // update available size
    auto &update_datanode = datanode_map_[location];

    update_datanode->set_available_size(new_datanode.available_size());

    // for(int i = 0;i < update_datanode->stored_chunk_handles_size();++i) {
    //     // update chunk info
    //     // should think of a smarter way
    // }
    datanode_priority_list_.sort(
        DataNodeAvailableSizeGreaterCompare());
}

void DataNodeManager::UnRegisterDataNode(const std::string datanode_location)
{
    LOG(INFO) << "unregistering " << datanode_location;
    if (!datanode_map_.count(datanode_location))
    {
        // nothing to unregister
        return;
    }
    auto unregistered_datanode = datanode_map_[datanode_location];
    datanode_map_.erase(datanode_location);

    for (auto iter = datanode_priority_list_.cbegin(); iter != datanode_priority_list_.cend(); ++iter)
    {
        if ((*iter)->location() == datanode_location)
        {
            // found and delete
            datanode_priority_list_.erase(iter);
            break;
        }
    }

    for (int i = 0; i < unregistered_datanode->stored_chunk_handles_size(); ++i)
    {
        auto tmp_chunk = chunk_map_[unregistered_datanode->stored_chunk_handles(i)];
        auto &chunk = chunk_map_[unregistered_datanode->stored_chunk_handles(i)];
        chunk.clear_location();
        for (int j = 0; j < tmp_chunk.location_size(); ++j)
        {
            if (tmp_chunk.location(j) != unregistered_datanode->location())
            {
                // not so smart way to remove unregister location from chunk
                chunk.add_location(tmp_chunk.location(j));
            }
        }
        // should send a rpc message to datanode which stored the same handle
        // (here default choose the first index)
        // to tell another datanode (the top element in priority list) to copy this chunk
        auto copy_to_datanode = datanode_priority_list_.cbegin();
        if (chunk.location_size() > 0 && (*copy_to_datanode)->available_size() >= 64)
        {
            auto client = GetOrCreateDataNodeControlClient(chunk.location(0));
            Rpc::CopyChunkRequest request;
            request.set_location(copy_to_datanode->get()->location());
            request.set_chunk_handle(unregistered_datanode->stored_chunk_handles(i));
            if (client->SendRequest(request).ok())
            {
                // successfully make a copy from datanode1 to datanode2
                chunk.add_location((*copy_to_datanode)->location());
                LOG(INFO) << "successfully make a copy chunk handle: " << chunk.chunk_handle()
                          << "from datanode " << chunk.location(0)
                          << "to datanode " << (*copy_to_datanode)->location();
            }
        }
        else
        {
            LOG(INFO) << "this chunk: " << chunk.chunk_handle() << " don't have allocated datanode";
        }
    }
    LOG(INFO) << "unregister " << datanode_location << " successfully";
    return;
}

std::shared_ptr<DataNodeControlerClient> DataNodeManager::GetOrCreateDataNodeControlClient(const std::string &address)
{
    if (datanode_control_clients_.count(address))
    {
        return datanode_control_clients_[address];
    }
    else
    {
        LOG(INFO) << "Establishing new connection to datanode"
                  << address;
        datanode_control_clients_[address] =
            std::make_shared<DataNodeControlerClient>(
                grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
        return datanode_control_clients_[address];
    }
}

const Rpc::DataNode DataNodeManager::GetDataNode(const std::string &location)
{
    Rpc::DataNode datanode;
    if (datanode_map_.count(location))
    {
        datanode = *datanode_map_[location];
        return datanode;
    }
    return datanode;
}

bool DataNodeManager::CheckExistDataNode(const std::string &location)
{
    if (datanode_map_.count(location))
    {
        return true;
    }
    return false;
}
grpc::Status ReportDataNodeServiceImpl::ReportDataNode(grpc::ServerContext *context, const ReportDataNodeRequest *request, ReportDataNodeReply *reply)
{
    
    //  auto& new_datanode = request->datanode();
    //         auto location = request->location();
    //         auto available_size = request->available_size();
    Rpc::DataNode new_datanode;
    new_datanode.set_location(request->location());
    new_datanode.set_available_size(request->available_size());
    for (int i = 0; i < request->stored_chunk_handles_size(); i++){
        new_datanode.add_stored_chunk_handles(request->stored_chunk_handles(i));
    }
    bool is_exist = DataNodeManager().GetInstance().CheckExistDataNode(request->location());
    if (!is_exist){
        // new datanode
        DataNodeManager().GetInstance().RegisterDataNode(std::make_shared<Rpc::DataNode>(new_datanode));
    } else {
        DataNodeManager().GetInstance().UpdateDataNode(new_datanode);
    }

    return grpc::Status::OK;
}
