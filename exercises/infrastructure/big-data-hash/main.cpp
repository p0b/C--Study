#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "big_data_processor.h"

// 简单的目录创建函数，兼容旧标准
void ensure_directory(const std::string& path) {
    std::string command = "mkdir -p " + path;
    system(command.c_str());
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   大文件单词统计 (哈希分流实战)" << std::endl;
    std::cout << "========================================" << std::endl;

    std::string input_file = "big_data_source.txt";
    std::string temp_dir = "temp_shards";
    int num_shards = 10; // 分成 10 个小文件
    int total_words = 1000000; // 模拟 100 万单词
    
    // 0. 准备环境
    ensure_directory(temp_dir);
    
    // 1. 生成测试数据
    std::cout << "\n[Step 0] 生成测试数据..." << std::endl;
    BigDataProcessor::generate_test_file(input_file, total_words);
    
    // 2. 初始化处理器
    BigDataProcessor processor(input_file, temp_dir, num_shards);
    
    // 3. 第一阶段：分流
    processor.split_file();
    
    // 4. 第二阶段：统计 Top 10
    std::vector<WordCount> top10 = processor.get_top_k(10);
    
    // 5. 展示结果
    std::cout << "\n[Step 3] 统计结果 (Top 10):" << std::endl;
    std::cout << "排名\t单词\t\t频率" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    
    int rank = 1;
    for (const auto& wc : top10) {
        std::cout << rank++ << "\t" << wc.word << "\t\t" << wc.count << std::endl;
    }
    
    std::cout << "\n[Success] 任务完成！" << std::endl;
    
    return 0;
}
