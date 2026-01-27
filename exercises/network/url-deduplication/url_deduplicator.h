#ifndef URL_DEDUPLICATOR_H
#define URL_DEDUPLICATOR_H

#include "bloom_filter.h"
#include <string>
#include <unordered_set>

class UrlDeduplicator {
private:
    BloomFilter* bloom_filter;           // 第一道防线：布隆过滤器
    std::unordered_set<std::string> db;  // 第二道防线：哈希表 (模拟数据库)
    
    // 统计数据 (看看保安大叔拦了多少人)
    int bloom_hits = 0;   // 布隆过滤器拦截次数
    int db_checks = 0;    // 数据库查询次数
    int true_duplicates = 0; // 真实重复次数

public:
    // 构造函数
    UrlDeduplicator(int expected_urls, double error_rate);

    // 析构函数 (负责任地释放资源)
    ~UrlDeduplicator();

    // 核心功能：检查并添加 URL
    // 返回 true: 是新 URL，需要爬取
    // 返回 false: 是重复 URL，跳过
    bool check_and_add(const std::string& url);
    
    // 打印统计报告
    void print_stats() const;
};

#endif // URL_DEDUPLICATOR_H
