#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip> // for std::fixed, std::setprecision
#include "bloom_filter.h"

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   布隆过滤器基础实现 - 验证测试" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // 1. 设置测试参数
    int n = 1000;              // 预期插入 1000 个元素
    double P = 0.05;           // 允许 5% 的误判率 (0.05)
    
    std::cout << "[Step 1] 初始化布隆过滤器..." << std::endl;
    std::cout << "  - 预期元素数 (n): " << n << std::endl;
    std::cout << "  - 目标误判率 (P): " << (P * 100) << "%" << std::endl;
    
    BloomFilter bf(n, P);
    
    // 2. 插入数据
    std::cout << "\n[Step 2] 插入 " << n << " 个元素..." << std::endl;
    std::vector<std::string> stored_keys;
    for (int i = 0; i < n; ++i) {
        std::string key = "user_id_" + std::to_string(i);
        stored_keys.push_back(key);
        bf.add(key);
    }
    std::cout << "  - 插入完成。" << std::endl;
    
    // 3. 验证已存在的元素（必须全部返回 true）
    std::cout << "\n[Step 3] 验证已插入的元素 (应该全部存在)..." << std::endl;
    int false_negatives = 0;
    for (const auto& key : stored_keys) {
        if (!bf.might_contain(key)) {
            false_negatives++;
        }
    }
    
    if (false_negatives == 0) {
        std::cout << "  - [PASS] 所有已插入元素均被检测到 (无假阴性)。" << std::endl;
    } else {
        std::cout << "  - [FAIL] 发现 " << false_negatives << " 个假阴性! (布隆过滤器不应出现假阴性)" << std::endl;
    }
    
    // 4. 验证不存在的元素（统计误判率）
    std::cout << "\n[Step 4] 验证不存在的元素 (统计误判率)..." << std::endl;
    int test_count = 10000; // 测试 10000 个未插入的元素
    int false_positives = 0;
    
    for (int i = 0; i < test_count; ++i) {
        // 生成一个肯定没插入过的 key
        std::string key = "non_existent_key_" + std::to_string(i);
        if (bf.might_contain(key)) {
            false_positives++;
        }
    }
    
    double actual_rate = static_cast<double>(false_positives) / test_count;
    
    std::cout << "  - 测试样本数: " << test_count << std::endl;
    std::cout << "  - 误判数量: " << false_positives << std::endl;
    std::cout << "  - 实际误判率: " << std::fixed << std::setprecision(4) << (actual_rate * 100) << "%" << std::endl;
    std::cout << "  - 目标误判率: " << (P * 100) << "%" << std::endl;
    
    if (actual_rate <= P * 1.5) { // 允许一定范围的波动
        std::cout << "  - [PASS] 误判率符合预期。" << std::endl;
    } else {
        std::cout << "  - [WARN] 误判率略高于预期，可能哈希函数分布不够均匀。" << std::endl;
    }
    
    std::cout << "\n========================================" << std::endl;
    return 0;
}
