#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class ConsistentHash {
private:
    map<size_t, string> ring;  // 哈希值 -> 服务器名
    size_t virtual_nodes;       // 每个物理节点的虚拟节点数
    
    size_t hash_string(const string& key) {
        // 简单哈希函数（实际应该用更好的算法）
        hash<string> hasher;
        return hasher(key);
    }
    
public:
    ConsistentHash(size_t virtual_nodes_per_server = 150) 
        : virtual_nodes(virtual_nodes_per_server) {}
    
    void add_server(const string& server_name, const string& server_ip) {
        cout << "=== 添加服务器 " << server_name << " (" << server_ip << ") ===" << endl;
        
        // 为每个物理服务器创建多个虚拟节点
        for (size_t i = 0; i < virtual_nodes; i++) {
            string virtual_node_name = server_name + "#" + to_string(i);
            size_t hash_value = hash_string(server_ip + ":" + to_string(i));
            ring[hash_value] = server_name;
            
            if (i < 5) { // 只显示前5个虚拟节点
                cout << "  虚拟节点 " << i << " -> 哈希值: " << hash_value << endl;
            }
        }
        if (virtual_nodes > 5) {
            cout << "  ... 共创建 " << virtual_nodes << " 个虚拟节点" << endl;
        }
        cout << "  总服务器数: " << ring.size() << " 个节点" << endl << endl;
    }
    
    string get_server(const string& key) {
        size_t key_hash = hash_string(key);
        
        cout << "查找 \"" << key << "\" -> 哈希值: " << key_hash << endl;
        
        // 在环上找到第一个 >= key_hash 的服务器
        auto it = ring.lower_bound(key_hash);
        
        if (it == ring.end()) {
            // 如果没找到，回到环的起点
            it = ring.begin();
            cout << "  超过环的末尾，回到起点" << endl;
        }
        
        cout << "  映射到服务器: " << it->second << " (哈希值: " << it->first << ")" << endl;
        return it->second;
    }
    
    void remove_server(const string& server_name, const string& server_ip) {
        cout << "=== 移除服务器 " << server_name << " (" << server_ip << ") ===" << endl;
        
        size_t removed = 0;
        for (size_t i = 0; i < virtual_nodes; i++) {
            string virtual_node_name = server_name + "#" + to_string(i);
            size_t hash_value = hash_string(server_ip + ":" + to_string(i));
            if (ring.erase(hash_value)) {
                removed++;
            }
        }
        cout << "  移除了 " << removed << " 个虚拟节点" << endl;
        cout << "  剩余服务器数: " << ring.size() << " 个节点" << endl << endl;
    }
    
    void show_ring_stats() {
        cout << "=== 环状态统计 ===" << endl;
        cout << "总节点数: " << ring.size() << endl;
        cout << "物理服务器数: " << ring.size() / virtual_nodes << endl;
        cout << endl;
    }
};

int main() {
    ConsistentHash ch(3);  // 每个服务器3个虚拟节点（演示用）
    
    // 初始部署
    ch.add_server("Cache1", "192.168.1.10");
    ch.add_server("Cache2", "192.168.1.11");
    ch.add_server("Cache3", "192.168.1.12");
    
    ch.show_ring_stats();
    
    // 查找一些Key
    cout << "=== 初始映射结果 ===" << endl;
    ch.get_server("user:1001");
    ch.get_server("product:500");
    ch.get_server("order:999");
    ch.get_server("user:2002");
    cout << endl;
    
    // 添加新服务器
    cout << "=== 扩容：添加新服务器 ===" << endl;
    ch.add_server("Cache4", "192.168.1.13");
    
    // 再次查找相同的Key，看看映射变化
    cout << "=== 扩容后的映射结果 ===" << endl;
    ch.get_server("user:1001");
    ch.get_server("product:500");
    ch.get_server("order:999");
    ch.get_server("user:2002");
    cout << endl;
    
    // 移除服务器
    cout << "=== 缩容：移除服务器 ===" << endl;
    ch.remove_server("Cache2", "192.168.1.11");
    
    cout << "=== 缩容后的映射结果 ===" << endl;
    ch.get_server("user:1001");
    ch.get_server("product:500");
    
    return 0;
}