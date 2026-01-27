#ifndef BIG_DATA_PROCESSOR_H
#define BIG_DATA_PROCESSOR_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>   // for unique_ptr
#include <iostream>
#include <map>
#include <algorithm>

// 用于存储单词及其频率的简单结构
struct WordCount {
    std::string word;
    int count;
    
    // 重载小于号，用于优先队列 (Top K)
    bool operator<(const WordCount& other) const {
        return count > other.count; // 最小堆，频率小的在上面，方便剔除
    }
};

class BigDataProcessor {
private:
    std::string input_file;
    std::string temp_dir;
    int num_shards;
    
    // 使用 unique_ptr 管理文件流，避免拷贝问题
    std::vector<std::unique_ptr<std::ofstream>> shard_writers;

    // 辅助函数：获取单词的哈希值并取模
    int get_shard_index(const std::string& word) const;

public:
    BigDataProcessor(const std::string& input, const std::string& temp, int shards);
    ~BigDataProcessor();

    // 阶段 1：哈希分流 (Split)
    void split_file();

    // 阶段 2：局部统计 + 归并 (Process & Merge)
    // 返回全局 Top K 结果
    std::vector<WordCount> get_top_k(int k);
    
    // 辅助：生成测试大文件
    static void generate_test_file(const std::string& filename, int total_words);
};

#endif // BIG_DATA_PROCESSOR_H
