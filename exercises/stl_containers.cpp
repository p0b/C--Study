#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

void vectorDemo() {
    std::cout << "--- 1. std::vector (动态数组) ---" << std::endl;
    // 就像一个可以自动伸缩的数组
    std::vector<int> v = {1, 2, 3};
    v.push_back(4); // 尾部追加
    
    std::cout << "Vector 内容: ";
    for (int x : v) { // 范围 for 循环 (Range-based for loop)
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << "第二个元素: " << v[1] << std::endl; // 随机访问 O(1)
}

void mapDemo() {
    std::cout << "\n--- 2. std::map (红黑树 / 有序) ---" << std::endl;
    // Key -> Value，按 Key 自动排序
    std::map<std::string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 80;
    scores["Charlie"] = 100; // 会自动插到中间去

    std::cout << "Map 内容 (自动按名字排序了):" << std::endl;
    for (const auto& pair : scores) {
        std::cout << "  " << pair.first << ": " << pair.second << std::endl;
    }
}

void unorderedMapDemo() {
    std::cout << "\n--- 3. std::unordered_map (哈希表 / 无序) ---" << std::endl;
    // Key -> Value，顺序随机，但速度最快
    std::unordered_map<std::string, std::string> config;
    config["IP"] = "127.0.0.1";
    config["Port"] = "8080";
    config["Name"] = "MyServer";

    std::cout << "Unordered Map 内容 (乱序):" << std::endl;
    for (const auto& pair : config) {
        std::cout << "  " << pair.first << ": " << pair.second << std::endl;
    }
    
    // 查找
    if (config.find("IP") != config.end()) {
        std::cout << "找到了 IP 配置: " << config["IP"] << std::endl;
    }
}

int main() {
    vectorDemo();
    mapDemo();
    unorderedMapDemo();
    return 0;
}
