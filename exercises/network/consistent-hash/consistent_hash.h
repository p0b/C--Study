#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H

#include <string>
#include <map>
#include <vector>
#include <cstdint>

class ConsistentHash {
private:
    // 哈希环：虚拟节点哈希值 -> 物理节点名称
    // std::map 底层是红黑树，Key 是有序排列的，非常适合模拟"环"
    std::map<uint32_t, std::string> nodes;
    
    // 每个物理节点对应的虚拟节点数量
    int virtual_node_count;
    
    // 哈希函数：将字符串转换为 32 位无符号整数
    uint32_t hash(const std::string& key) const;

public:
    // 构造函数：指定虚拟节点数量
    ConsistentHash(int v_nodes = 100);
    
    // 添加物理节点 (例如: "192.168.1.1")
    void add_node(const std::string& node_ip);
    
    // 删除物理节点
    void remove_node(const std::string& node_ip);
    
    // 根据 Key 查找它应该存放的节点 (例如: "user_123")
    std::string get_node(const std::string& key) const;
    
    // 获取环上的节点总数 (虚拟节点数)
    size_t get_ring_size() const { return nodes.size(); }
};

#endif // CONSISTENT_HASH_H
