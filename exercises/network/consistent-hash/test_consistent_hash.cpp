#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <map>
#include <iomanip>
#include "consistent_hash.h"

// 辅助函数：生成随机字符串 Key
std::string generate_random_key(int length) {
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    std::string key;
    key.reserve(length);
    for (int i = 0; i < length; ++i) {
        key += charset[dist(rng)];
    }
    return key;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   分布式一致性哈希 - 扩容迁移测试" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 初始化哈希环 (每节点 150 个虚拟节点)
    int v_nodes = 150;
    ConsistentHash ring(v_nodes);
    
    // 2. 初始集群：5 台服务器
    int initial_servers = 5;
    std::cout << "\n[Step 1] 初始化 " << initial_servers << " 台服务器..." << std::endl;
    for (int i = 0; i < initial_servers; ++i) {
        ring.add_node("192.168.1." + std::to_string(i));
    }

    // 3. 生成 10 万个测试 Key
    int key_count = 100000;
    std::cout << "\n[Step 2] 生成 " << key_count << " 个测试 Key 并分配..." << std::endl;
    std::vector<std::string> keys;
    // 记录每个 Key 初始归属的服务器 IP
    std::map<std::string, std::string> initial_placement;
    
    for (int i = 0; i < key_count; ++i) {
        std::string key = "user_" + std::to_string(i); // 简单起见，用顺序 Key
        keys.push_back(key);
        initial_placement[key] = ring.get_node(key);
    }
    
    // 统计初始分布情况
    std::map<std::string, int> server_load;
    for (const auto& pair : initial_placement) {
        server_load[pair.second]++;
    }
    
    std::cout << "  - 初始分布情况 (理想平均值: " << key_count / initial_servers << "):" << std::endl;
    for (const auto& pair : server_load) {
        std::cout << "    " << pair.first << ": " << pair.second << " keys" << std::endl;
    }

    // 4. 扩容：增加 5 台服务器 (总共 10 台)
    int new_servers = 5;
    std::cout << "\n[Step 3] 扩容！增加 " << new_servers << " 台新服务器..." << std::endl;
    for (int i = initial_servers; i < initial_servers + new_servers; ++i) {
        ring.add_node("192.168.1." + std::to_string(i));
    }

    // 5. 统计迁移量
    std::cout << "\n[Step 4] 重新计算 Key 归属，统计迁移量..." << std::endl;
    int migrated_keys = 0;
    for (const auto& key : keys) {
        std::string new_node = ring.get_node(key);
        if (new_node != initial_placement[key]) {
            migrated_keys++;
        }
    }

    double migration_rate = (double)migrated_keys / key_count * 100.0;
    std::cout << "  - 迁移 Key 数量: " << migrated_keys << std::endl;
    std::cout << "  - 迁移率: " << std::fixed << std::setprecision(2) << migration_rate << "%" << std::endl;
    
    // 理论迁移率计算：(N2 - N1) / N2 = (10 - 5) / 10 = 50%
    // 如果用取模法 hash % N：迁移率 = 1 - (N1/N2) (仅当N2是N1倍数时较低，否则接近 100%)
    // 实际上取模法扩容通常会导致接近 100% 失效，除非是成倍扩容且算法特殊
    // 对于一致性哈希，理论迁移率就是 1 - (旧节点数/新节点数) = 1 - 5/10 = 50%
    
    std::cout << "  - 理论预期迁移率: 50.00%" << std::endl;
    
    if (std::abs(migration_rate - 50.0) < 5.0) {
        std::cout << "\n[PASS] 结果符合一致性哈希特性！" << std::endl;
    } else {
        std::cout << "\n[WARN] 迁移率偏差较大，请检查虚拟节点数量是否足够多。" << std::endl;
    }

    return 0;
}
