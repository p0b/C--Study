#include <iostream>
#include <utility> // for std::forward

// 目标函数：重载了左值和右值版本
void process(int& i) {
    std::cout << "  [Target] 处理左值引用 (LValue Ref): " << i << std::endl;
}

void process(int&& i) {
    std::cout << "  [Target] 处理右值引用 (RValue Ref): " << i << std::endl;
}

// 1. 普通转发 (失败案例)
// 问题：无论 arg 原来是什么，在函数体内它都有名字，所以它永远是左值！
template<typename T>
void badWrapper(T&& arg) {
    std::cout << "Bad Wrapper 接收到参数..." << std::endl;
    process(arg); // ❌ 永远调用左值版本
}

// 2. 完美转发 (成功案例)
// 配合 std::forward，如果 T 是右值，它就把 arg 再次转回右值
template<typename T>
void perfectWrapper(T&& arg) {
    std::cout << "Perfect Wrapper 接收到参数..." << std::endl;
    // std::forward<T>(arg) 会根据 T 的类型决定是否 move
    process(std::forward<T>(arg)); 
}

void forwardingDemo() {
    int x = 10;
    
    std::cout << "--- 测试 1: 传递左值 (int x = 10) ---" << std::endl;
    std::cout << ">> Bad Wrapper:" << std::endl;
    badWrapper(x);
    std::cout << ">> Perfect Wrapper:" << std::endl;
    perfectWrapper(x);
    
    std::cout << "\n--- 测试 2: 传递右值 (临时变量 20) ---" << std::endl;
    std::cout << ">> Bad Wrapper:" << std::endl;
    badWrapper(20); // 本来传的是右值，但在 wrapper 里变成了左值
    std::cout << ">> Perfect Wrapper:" << std::endl;
    perfectWrapper(20); // 完美保持了右值属性
}

int main() {
    forwardingDemo();
    return 0;
}
