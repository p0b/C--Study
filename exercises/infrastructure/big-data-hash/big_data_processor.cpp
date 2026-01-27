#include "big_data_processor.h"
#include <functional>
#include <queue>
#include <random>
#include <filesystem>
#include <unordered_map> // 修复: 添加缺失的头文件

// 构造函数
BigDataProcessor::BigDataProcessor(const std::string& input, const std::string& temp, int shards)
    : input_file(input), temp_dir(temp), num_shards(shards) {
    // 确保临时目录存在
    // std::filesystem::create_directories(temp_dir); // C++17 特性
    // 为了兼容旧编译器，这里假设目录已存在或通过脚本创建
}

// 析构函数：unique_ptr 会自动关闭文件流，这里不需要手动 close
BigDataProcessor::~BigDataProcessor() {}

// 辅助：获取分片索引
int BigDataProcessor::get_shard_index(const std::string& word) const {
    std::hash<std::string> hasher;
    // size_t 是无符号的，直接取模是安全的
    return hasher(word) % num_shards;
}

// 阶段 1：哈希分流
void BigDataProcessor::split_file() {
    std::cout << "[Step 1] 开始分流大文件..." << std::endl;
    std::ifstream infile(input_file);
    if (!infile.is_open()) {
        std::cerr << "无法打开输入文件: " << input_file << std::endl;
        return;
    }

    // 1. 初始化所有分片文件的写入器
    // 清空之前的 writers
    shard_writers.clear(); 
    for (int i = 0; i < num_shards; ++i) {
        std::string name = temp_dir + "/shard_" + std::to_string(i) + ".txt";
        // 使用 make_unique 创建智能指针，管理文件流
        shard_writers.push_back(std::make_unique<std::ofstream>(name));
    }

    // 2. 逐词读取并分流
    std::string word;
    long long word_count = 0;
    while (infile >> word) {
        int idx = get_shard_index(word);
        // *shard_writers[idx] 解引用智能指针，得到 ofstream&
        *shard_writers[idx] << word << "\n"; // 每个单词一行，方便读取
        word_count++;
        
        if (word_count % 100000 == 0) {
            std::cout << "  已处理 " << word_count << " 个单词..." << "\r" << std::flush;
        }
    }
    std::cout << "  分流完成！总共处理 " << word_count << " 个单词。" << std::endl;

    // 3. 关闭所有分片文件 (虽然析构也会关，但手动清空是个好习惯)
    shard_writers.clear();
}

// 阶段 2：统计与归并
std::vector<WordCount> BigDataProcessor::get_top_k(int k) {
    std::cout << "\n[Step 2] 开始局部统计与归并 (Top " << k << ")..." << std::endl;
    
    // 全局最小堆，维护 Top K
    // 为什么用最小堆？因为如果新元素比堆顶(最小值)大，就替换堆顶，这样堆里剩下的永远是最大的K个
    // C++ priority_queue 默认是最大堆，所以 WordCount 重载了 < 运算符为 > 逻辑
    std::priority_queue<WordCount> min_heap;
    
    // 遍历所有分片
    for (int i = 0; i < num_shards; ++i) {
        std::string name = temp_dir + "/shard_" + std::to_string(i) + ".txt";
        std::ifstream shard_in(name);
        
        if (!shard_in.is_open()) continue;

        // 局部统计 (HashMap)
        std::unordered_map<std::string, int> local_counts;
        std::string word;
        while (shard_in >> word) {
            local_counts[word]++;
        }
        
        // 将局部结果汇入全局堆
        for (const auto& pair : local_counts) {
            WordCount wc{pair.first, pair.second};
            
            if (min_heap.size() < k) {
                min_heap.push(wc);
            } else if (wc.count > min_heap.top().count) {
                min_heap.pop();
                min_heap.push(wc);
            }
        }
        
        std::cout << "  分片 " << i << " 处理完毕。" << std::endl;
        shard_in.close();
        
        // 可选：处理完一个分片就删除，释放磁盘空间
        // std::remove(name.c_str());
    }

    // 导出结果 (堆里是无序的，且是最小堆，需要倒序输出)
    std::vector<WordCount> result;
    while (!min_heap.empty()) {
        result.push_back(min_heap.top());
        min_heap.pop();
    }
    // 翻转，让频率最高的排前面
    std::reverse(result.begin(), result.end());
    
    return result;
}

// 辅助：生成测试文件
void BigDataProcessor::generate_test_file(const std::string& filename, int total_words) {
    std::ofstream outfile(filename);
    std::vector<std::string> vocab = {"apple", "banana", "cherry", "date", "elderberry", "fig", "grape", "hadoop", "spark", "linux"};
    
    // 制造数据倾斜：前几个词出现频率特别高
    std::mt19937 rng(12345); // 固定种子
    std::exponential_distribution<> dist(3.5); // 指数分布
    
    for (int i = 0; i < total_words; ++i) {
        int idx = static_cast<int>(dist(rng) * 10) % vocab.size();
        if (idx < 0) idx = 0;
        outfile << vocab[idx] << " ";
    }
    std::cout << "测试文件已生成: " << filename << std::endl;
}
