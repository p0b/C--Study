#include <iostream>
#include <string>
#include <regex>

void regexSyntaxExplanation() {
    std::cout << "--- 正则表达式语法深度解析 ---" << std::endl;

    // 1. R"()" 原始字符串字面量 (Raw String Literal)
    // ----------------------------------------------------
    // 在普通 C++ 字符串中，反斜杠 \ 是转义字符。
    // 如果要表示 "数字" 的正则 \d，在 C++ 里必须写成 "\\d"。
    // 如果正则复杂一点，反斜杠会多到让你怀疑人生。
    // R"(...)" 告诉编译器：括号里的东西，我写啥就是啥，别转义！
    
    std::string normal_str = "\\d+"; // 普通写法：必须双斜杠
    std::string raw_str = R"(\d+)";  // 原始写法：单斜杠即可，所见即所得
    
    std::cout << "1. 原始字符串对比:" << std::endl;
    std::cout << "   普通写法打印出来: " << normal_str << std::endl;
    std::cout << "   Raw 写法打印出来: " << raw_str << std::endl;

    // 2. 捕获组 (Capture Groups)与 std::smatch
    // ----------------------------------------------------
    std::string log = "ID: 9527, Name: 007";
    
    // 正则：ID: (\d+), Name: (\d+)
    // 括号 () 的作用就是 "划重点"。
    // 编译器会把括号里匹配到的内容，单独存下来。
    std::regex pattern(R"(ID: (\d+), Name: (\d+))");
    
    // std::smatch (String Match) 是一个容器，专门存匹配结果
    std::smatch matches;

    if (std::regex_search(log, matches, pattern)) {
        std::cout << "\n2. 捕获组解析:" << std::endl;
        // matches[0] 永远是 "整个字符串匹配到的部分"
        std::cout << "   matches[0] (全匹配): " << matches[0] << std::endl;
        
        // matches[1] 是第一个括号 (\d+) 抓到的内容
        std::cout << "   matches[1] (ID):     " << matches[1] << std::endl;
        
        // matches[2] 是第二个括号 (\d+) 抓到的内容
        std::cout << "   matches[2] (Name):   " << matches[2] << std::endl;
    }

    // 3. 替换逻辑 (regex_replace)
    // ----------------------------------------------------
    std::string text = "Order 12345";
    std::regex digit_pattern(R"(\d+)"); // \d+ 意思是一个或多个数字
    
    // 逻辑：找到 text 中所有符合 digit_pattern 的部分，把它们变成 "###"
    std::string result = std::regex_replace(text, digit_pattern, "###");
    
    std::cout << "\n3. 替换逻辑:" << std::endl;
    std::cout << "   原文: " << text << std::endl;
    std::cout << "   结果: " << result << std::endl;
}

int main() {
    regexSyntaxExplanation();
    return 0;
}
