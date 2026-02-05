#include <iostream>
#include <stdexcept>
#include <memory>

void rawPointerRisk() {
    std::cout << "--- 裸指针风险演示 ---" << std::endl;
    int* p = new int(100);
    std::cout << "分配了内存: " << p << std::endl;

    try {
        // 模拟业务逻辑抛出异常
        if (true) {
            throw std::runtime_error("业务逻辑发生错误！");
        }
        
        // 这行代码永远不会执行
        delete p; 
        std::cout << "释放了内存" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        // 糟糕！delete p 被跳过了，内存泄漏！
        // 在 C++ 中，栈展开（Stack Unwinding）只会销毁栈上的对象，
        // 堆上的对象（new 出来的）如果没有人 delete，就永远丢了。
    }
}

// 这是一个简单的 RAII 包装类，演示智能指针的原理
class IntegerWrapper {
public:
    IntegerWrapper(int v) : ptr(new int(v)) {
        std::cout << "IntegerWrapper 构造: 分配内存 " << ptr << std::endl;
    }
    // 析构函数：这是 RAII 的核心！
    // 无论函数是正常返回还是异常退出，栈对象销毁时一定会调用析构函数
    ~IntegerWrapper() {
        std::cout << "IntegerWrapper 析构: 释放内存 " << ptr << std::endl;
        delete ptr;
    }
    int getValue() const { return *ptr; }
private:
    int* ptr;
};

void raiiDemo() {
    std::cout << "\n--- RAII (智能指针原理) 演示 ---" << std::endl;
    try {
        IntegerWrapper wrapper(200); // 栈上对象
        
        if (true) {
            throw std::runtime_error("业务逻辑又崩了！");
        }
    } catch (const std::exception& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
        // 此时 wrapper 已经被自动销毁，内存已被释放！
    }
}

int main() {
    rawPointerRisk();
    raiiDemo();
    return 0;
}
