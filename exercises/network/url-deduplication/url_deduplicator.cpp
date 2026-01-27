#include "url_deduplicator.h"
#include <iostream>

UrlDeduplicator::UrlDeduplicator(int expected_urls, double error_rate) {
    bloom_filter = new BloomFilter(expected_urls, error_rate);
}

UrlDeduplicator::~UrlDeduplicator() {
    // 释放保安大叔，防止内存泄漏
    delete bloom_filter;
}

bool UrlDeduplicator::check_and_add(const std::string& url) {
    // TODO: 实现双层防御逻辑
    
    // 第一步：问布隆过滤器 (Bloom Filter)
    if (!bloom_filter->might_contain(url)) {
        // 既然保安说没见过，那就是真没见过 (无假阴性)
        // 1. 把这个新面孔告诉保安 (加入 Bloom Filter)
        bloom_filter->add(url);
        
        // 2. 登记在册 (加入 DB)
        db.insert(url);
        
        return true; // 是新的！
    }
    
    // 到了这里，说明保安说"好像见过" (可能是真的，也可能是误判)
    bloom_hits++; // 记录一次拦截
    
    // 第二步：问精准数据库 (Hash Table)
    db_checks++; // 只要翻了花名册，就要记上一笔！
    
    if (db.count(url) > 0) {
        // 确实在数据库里找到了，说明是真的重复
        true_duplicates++;
        return false;
    } else {
        // 数据库里没有，说明是布隆过滤器的误判 (False Positive)
        // 这是一个新 URL，赶紧补录！
        
        // 1. 补录到布隆过滤器 (防止下次再误判...虽然位已经置了，但语义上应该add)
        // 其实这里 add 不 add 位数组可能没变化，但为了逻辑完整性建议调用
        bloom_filter->add(url); 
        
        // 2. 录入数据库
        db.insert(url);
        
        return true; // 是新的！
    } 
}

void UrlDeduplicator::print_stats() const {
    std::cout << "\n=== 去重系统统计报告 ===" << std::endl;
    std::cout << "布隆过滤器拦截次数: " << bloom_hits << std::endl;
    std::cout << "数据库二次确认次数: " << db_checks << std::endl;
    std::cout << "实际存储 URL 数量:  " << db.size() << std::endl;
}
