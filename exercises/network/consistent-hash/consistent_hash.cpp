#include "consistent_hash.h"
#include <iostream>
#include <functional> // std::hash
#include <sstream>

ConsistentHash::ConsistentHash(int v_nodes) : virtual_node_count(v_nodes) {}

// 简单的哈希函数实现
uint32_t ConsistentHash::hash(const std::string& key) const {
    std::hash<std::string> hasher;
    // std::hash 返回的是 size_t (通常 64 位)，我们截断为 32 位以模拟 2^32 空间
    return static_cast<uint32_t>(hasher(key));
}

void ConsistentHash::add_node(const std::string& node_ip) {
    // 为每个物理节点生成 virtual_node_count 个虚拟节点
    for (int i = 0; i < virtual_node_count; ++i) {
        // 构造虚拟节点名称，例如: "192.168.1.1#0"
        std::string virtual_node_name = node_ip + "#" + std::to_string(i);
        
        // 计算虚拟节点的哈希值
        uint32_t v_hash = hash(virtual_node_name);
        
        // 放入 map 中，value 存真实的物理节点 IP
        nodes[v_hash] = node_ip;
    }
    
    std::cout << "[ConsistentHash] Added node: " << node_ip 
              << ", Total virtual nodes on ring: " << nodes.size() << std::endl;
}

void ConsistentHash::remove_node(const std::string& node_ip) {
    // 移除该物理节点的所有虚拟节点
    for (int i = 0; i < virtual_node_count; ++i) {
        std::string virtual_node_name = node_ip + "#" + std::to_string(i);
        uint32_t v_hash = hash(virtual_node_name);
        
        // 从 map 中删除
        nodes.erase(v_hash);
    }
    
    std::cout << "[ConsistentHash] Removed node: " << node_ip 
              << ", Total virtual nodes on ring: " << nodes.size() << std::endl;
}

std::string ConsistentHash::get_node(const std::string& key) const {
    if (nodes.empty()) {
        return "";
    }

    uint32_t key_hash = hash(key);
    
    // 1. 在 map 中找到第一个 key >= key_hash 的节点
    // lower_bound 返回指向第一个关键字 >= key_hash 的元素的迭代器
    auto it = nodes.lower_bound(key_hash);
    
    // 2. 如果找到了 (it != end())，那就是它
    if (it != nodes.end()) {
        return it->second;
    }
    
    // 3. 如果没找到 (it == end())，说明 key_hash 比环上所有节点都大
    // 根据环的特性，回绕到起点，取第一个节点
    return nodes.begin()->second;
}
