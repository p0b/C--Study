#include "bloom_filter.h"
#include <iostream>
#include <functional> // for std::hash

BloomFilter::BloomFilter(int expected_elements, double false_positive_rate) {
    // 1. 准备变量 (转为 double 以进行浮点计算)
    double n = static_cast<double>(expected_elements);
    double p = false_positive_rate;
    double ln2 = std::log(2.0);

    // 2. 计算 m (位数组大小)
    // 公式: m = - (n * ln(P)) / (ln(2)^2)
    // 注意：C++ 中 std::log() 即为自然对数 ln()
    double m_optimal = - (n * std::log(p)) / (ln2 * ln2);
    
    // m 向上取整，保证空间足够
    this->num_bits = static_cast<int>(std::ceil(m_optimal));

    // 3. 计算 k (哈希函数个数)
    // 公式: k = (m / n) * ln(2)
    double k_optimal = (m_optimal / n) * ln2;
    
    // k 四舍五入
    this->num_hashes = static_cast<int>(std::round(k_optimal));

    // 4. 边界检查 (防止非法参数)
    if (this->num_bits < 1) this->num_bits = 1;
    if (this->num_hashes < 1) this->num_hashes = 1;
    
    // 5. 初始化位数组
    // vector::resize 会自动分配空间并初始化
    this->bits.resize(this->num_bits, false);
    
    std::cout << "[BloomFilter] 参数计算完成:" << std::endl;
    std::cout << "  - 输入: n=" << n << ", P=" << p << std::endl;
    std::cout << "  - 结果: m=" << num_bits << " bits, k=" << num_hashes << " hashes" << std::endl;
}

std::vector<int> BloomFilter::get_hash_indices(const std::string& key) const {
    std::vector<int> indices;
    
    // 1. 准备哈希函数对象
    std::hash<std::string> hasher;
    
    // 2. 计算两个基础哈希值 (Double Hashing 策略)
    // h1: 直接对 key 进行哈希
    size_t h1 = hasher(key);
    
    // h2: 对 key 稍作修改后哈希 (模拟第二个独立的哈希函数)
    // 这里简单的给 key 加上一个后缀再哈希
    size_t h2 = hasher(key + "_salt");
    
    // 3. 生成 k 个哈希位置
    for (int i = 0; i < this->num_hashes; ++i) {
        // 公式: hash_i = (h1 + i * h2) % m
        // 使用 size_t (无符号长整型) 允许自然溢出，这是安全的
        size_t combined_hash = h1 + i * h2;
        
        // 映射到 [0, m-1] 范围
        int index = combined_hash % this->num_bits;
        
        indices.push_back(index);
    }
    
    return indices;
}

void BloomFilter::add(const std::string& key) {
    // 1. 获取所有哈希位置
    std::vector<int> indices = get_hash_indices(key);
    
    // 2. 将对应位置标记为 1 (true)
    for (int index : indices) {
        this->bits[index] = true;
    }
}

bool BloomFilter::might_contain(const std::string& key) const {
    // 1. 获取所有哈希位置
    std::vector<int> indices = get_hash_indices(key);
    
    // 2. 检查所有对应位
    for (int index : indices) {
        // 只要发现有一个位是 0 (false)，则该元素一定不存在
        if (this->bits[index] == false) {
            return false;
        }
    }
    
    // 3. 所有位都是 1，则该元素可能存在 (假阳性)
    return true;
}
