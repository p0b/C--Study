#include <iostream>
#include <string>

// 演示类：用来观察构造和赋值的区别
class StringWrapper {
public:
    StringWrapper() { 
        std::cout << "  [StringWrapper] 默认构造 (Default Constructor)" << std::endl; 
    }
    StringWrapper(const std::string& s) { 
        std::cout << "  [StringWrapper] 带参构造: " << s << std::endl; 
    }
    StringWrapper& operator=(const std::string& s) {
        std::cout << "  [StringWrapper] 赋值操作 (Assignment): " << s << std::endl;
        return *this;
    }
};

// 1. 低效写法：先构造，再赋值
class BadClass {
public:
    StringWrapper member;
    
    // 这里的 member 经历了两次操作：
    // 1. 在进入 { 之前，先调用了 member 的默认构造函数
    // 2. 在 { 里面，又调用了 member 的赋值操作符
    BadClass(std::string s) {
        std::cout << "--- BadClass 进入构造函数体 ---" << std::endl;
        member = s; 
    }
};

// 2. 高效写法：初始化列表
class GoodClass {
public:
    StringWrapper member;

    // 这里的 member 只有一次操作：
    // 1. 直接调用 member 的带参构造函数进行初始化
    // 根本没有产生临时的中间状态
    GoodClass(std::string s) : member(s) {
        std::cout << "--- GoodClass 进入构造函数体 ---" << std::endl;
    }
};

void initListDemo() {
    std::cout << "=== 1. 低效写法 (BadClass) ===" << std::endl;
    BadClass bad("Hello");
    
    std::cout << "\n=== 2. 高效写法 (GoodClass) ===" << std::endl;
    GoodClass good("World");
}

int main() {
    initListDemo();
    return 0;
}
