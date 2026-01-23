#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <vector>
#include <string>
#include <cmath>
#include <cstdint>

class BloomFilter {
private:
    std::vector<bool> bits;      // 位数组：存储指纹
    int num_hashes;              // k: 哈希函数个数
    int num_bits;                // m: 位数组长度
    
    // 内部辅助函数：计算一个键的 k 个哈希位置
    // 使用双重哈希策略：hash_i = (hash1 + i * hash2) % m
    std::vector<int> get_hash_indices(const std::string& key) const;
    
public:
    /**
     * 构造函数
     * @param expected_elements (n): 预期要插入的元素数量
     * @param false_positive_rate (P): 允许的误判率 (例如 0.01 表示 1%)
     */
    BloomFilter(int expected_elements, double false_positive_rate);
    
    /**
     * 添加元素
     * @param key: 要添加的字符串键
     */
    void add(const std::string& key);
    
    /**
     * 查询元素是否可能存在
     * @param key: 要查询的字符串键
     * @return true: 可能存在; false: 一定不存在
     */
    bool might_contain(const std::string& key) const;
    
    // 获取当前参数（用于调试）
    int get_num_bits() const { return num_bits; }
    int get_num_hashes() const { return num_hashes; }
};

#endif // BLOOM_FILTER_H
