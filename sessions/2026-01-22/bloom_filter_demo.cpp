#include <vector>
#include <functional>
#include <cmath>
#include <iostream>
using namespace std;

class BloomFilter {
private:
    vector<bool> bits;           // 位数组
    vector<function<size_t(string)>> hash_functions;  // 哈希函数列表
    
public:
    // 构造函数：根据预期元素数量和误判率自动计算最优参数
    BloomFilter(size_t expected_elements, double false_positive_rate) {
        // 计算最优的位数组长度和哈希函数个数
        double m_double = -((double)expected_elements * log(false_positive_rate)) / pow(log(2), 2);
        size_t m = (size_t)m_double;
        double k_double = (m / (double)expected_elements) * log(2);
        size_t k = (size_t)k_double;
        
        cout << "=== 布隆过滤器参数 ===" << endl;
        cout << "预期元素数: " << expected_elements << endl;
        cout << "误判率: " << false_positive_rate * 100 << "%" << endl;
        cout << "位数组长度: " << m << " 位 (" << m/8/1024/1024.0 << " MB)" << endl;
        cout << "哈希函数个数: " << k << endl;
        cout << endl;
        
        bits.resize(m);
        
        // 创建k个不同的哈希函数
        for (size_t i = 0; i < k; i++) {
            hash_functions.push_back([i, m](const string& key) {
                // 简单的多重哈希实现
                // 实际工程中应该用更复杂的哈希函数
                size_t hash1 = hash<string>{}(key);
                size_t hash2 = hash<string>{}(key + to_string(i));
                return (hash1 + i * hash2) % m;
            });
        }
    }
    
    void add(const string& key) {
        for (auto& hash_func : hash_functions) {
            size_t index = hash_func(key);
            bits[index] = true;
        }
    }
    
    bool might_contain(const string& key) {
        for (auto& hash_func : hash_functions) {
            size_t index = hash_func(key);
            if (!bits[index]) {
                return false;  // 一定不存在
            }
        }
        return true;  // 可能存在
    }
    
    double current_false_positive_rate(size_t inserted_elements) {
        double k = hash_functions.size();
        double m = bits.size();
        double n = inserted_elements;
        return pow(1 - exp(-k * n / m), k);
    }
};

int main() {
    // 示例：垃圾邮件黑名单
    BloomFilter blacklist(100000000, 0.001);  // 1亿邮箱，0.1%误判率
    
    // 添加一些恶意邮箱
    blacklist.add("spam@example.com");
    blacklist.add("phisher@scam.com");
    blacklist.add("virus@malware.net");
    
    // 测试
    cout << "=== 测试结果 ===" << endl;
    cout << "spam@example.com 存在吗? " << (blacklist.might_contain("spam@example.com") ? "是" : "否") << endl;
    cout << "phisher@scam.com 存在吗? " << (blacklist.might_contain("phisher@scam.com") ? "是" : "否") << endl;
    cout << "good@legit.com 存在吗? " << (blacklist.might_contain("good@legit.com") ? "可能" : "肯定不是") << endl;
    
    return 0;
}