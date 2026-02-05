#include <iostream>
#include <string>
#include <regex>

void regexDemo() {
    std::cout << "--- C++11 Regex 演示 ---" << std::endl;

    // 1. 验证邮箱格式 (regex_match)
    // 原始字符串 (Raw String) R"(...)" 避免转义地狱
    // 意思是：以字母开头，后面跟着字母数字，然后是 @，然后是域名
    std::regex email_pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    
    std::string email1 = "test.user@example.com";
    std::string email2 = "invalid-email";

    std::cout << "验证 " << email1 << ": " 
              << (std::regex_match(email1, email_pattern) ? "✅ 通过" : "❌ 失败") << std::endl;
    std::cout << "验证 " << email2 << ": " 
              << (std::regex_match(email2, email_pattern) ? "✅ 通过" : "❌ 失败") << std::endl;

    // 2. 提取数据 (regex_search)
    // 假设我们要从日志里提取时间戳和错误码
    std::string log = "[2026-02-05 14:30:00] ERROR: Code 404 not found";
    // 捕获组：(\d{4}-\d{2}-\d{2}) 捕获日期，Code (\d+) 捕获错误码
    std::regex log_pattern(R"(\[(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})\] ERROR: Code (\d+))");
    std::smatch matches;

    if (std::regex_search(log, matches, log_pattern)) {
        std::cout << "\n日志解析成功:" << std::endl;
        std::cout << "  完整匹配: " << matches[0] << std::endl;
        std::cout << "  时间戳:   " << matches[1] << std::endl; // 第一个括号里的内容
        std::cout << "  错误码:   " << matches[2] << std::endl; // 第二个括号里的内容
    }

    // 3. 替换 (regex_replace)
    // 把所有的数字替换成 #
    std::string text = "Order 12345, Qty 99";
    std::regex digit_pattern(R"(\d+)");
    std::string masked = std::regex_replace(text, digit_pattern, "###");
    
    std::cout << "\n脱敏结果: " << masked << std::endl;
}

int main() {
    regexDemo();
    return 0;
}
