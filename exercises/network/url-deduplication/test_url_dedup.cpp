#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>
#include "url_deduplicator.h"

// 简单的随机字符串生成器 (模拟 URL)
std::string generate_url(int id) {
    return "https://www.example.com/page/" + std::to_string(id) + ".html";
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   URL 去重系统 - 双层防御实战演习" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. 初始化系统
    // 假设我们要处理 10万数据，允许 1% 的误判率
    int estimated_count = 100000;
    double error_rate = 0.01;
    UrlDeduplicator dedup(estimated_count, error_rate);
    
    std::cout << "\n[Step 1] 系统初始化完成" << std::endl;
    std::cout << "  - 预计容量: " << estimated_count << std::endl;
    std::cout << "  - 允许误判率: " << error_rate * 100 << "%" << std::endl;

    // 2. 准备历史数据 (50,000 个)
    int history_count = 50000;
    std::vector<std::string> history_urls;
    std::cout << "\n[Step 2] 正在加载历史数据 (" << history_count << " 条)..." << std::endl;
    
    for (int i = 0; i < history_count; ++i) {
        std::string url = generate_url(i);
        history_urls.push_back(url);
        dedup.check_and_add(url); // 全部录入
    }
    std::cout << "  - 历史数据加载完毕。" << std::endl;

    // 3. 模拟流量洪峰 (50,000 条请求)
    // - 10,000 条是重复的 (来自历史数据)
    // - 40,000 条是全新的
    int total_requests = 50000;
    int duplicate_count = 10000;
    int new_count = total_requests - duplicate_count;
    
    std::cout << "\n[Step 3] 流量洪峰来袭!" << std::endl;
    std::cout << "  - 总请求: " << total_requests << std::endl;
    std::cout << "  - 其中重复: " << duplicate_count << " (应被拦截)" << std::endl;
    std::cout << "  - 其中新增: " << new_count << " (应被放行)" << std::endl;

    std::vector<std::string> requests;
    
    // 混入重复数据
    for (int i = 0; i < duplicate_count; ++i) {
        // 随机取一些历史数据
        requests.push_back(history_urls[i]); 
    }
    
    // 混入新数据 (ID 从 history_count 开始，保证不重复)
    for (int i = 0; i < new_count; ++i) {
        requests.push_back(generate_url(history_count + i));
    }
    
    // 打乱顺序，模拟真实流量
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(requests.begin(), requests.end(), g);
    
    // 4. 开始处理
    std::cout << "  - 正在处理..." << std::endl;
    int accepted = 0;
    int rejected = 0;
    
    for (const auto& url : requests) {
        if (dedup.check_and_add(url)) {
            accepted++;
        } else {
            rejected++;
        }
    }
    
    // 5. 结果分析
    std::cout << "\n[Step 4] 处理结果" << std::endl;
    std::cout << "  - 放行数量: " << accepted << " (预期: " << new_count << ")" << std::endl;
    std::cout << "  - 拦截数量: " << rejected << " (预期: " << duplicate_count << ")" << std::endl;
    
    if (accepted == new_count && rejected == duplicate_count) {
        std::cout << "  - [PASS] 完美！所有重复均被拦截，所有新数据均被放行。" << std::endl;
    } else {
        std::cout << "  - [FAIL] 哎呀，数据有点对不上..." << std::endl;
    }
    
    // 6. 查看内部统计
    dedup.print_stats();
    
    return 0;
}
