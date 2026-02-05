#include <iostream>
#include <string>
#include <vector>

void analyze(int& i) {
    std::cout << "  -> 这是一个 [左值] (有名字，在内存里有固定地址)" << std::endl;
}

void analyze(int&& i) {
    std::cout << "  -> 这是一个 [右值] (临时对象，没名字，马上要蒸发)" << std::endl;
}

// 获取一个右值（返回临时对象）
int getTemp() { return 100; }

// 获取一个左值（返回全局变量的引用）
int globalVar = 999;
int& getGlobal() { return globalVar; }

void valueCategoryDemo() {
    std::cout << "--- 左值 vs 右值 大解析 ---" << std::endl;

    // 1. 最简单的左值
    int a = 10; 
    std::cout << "分析 a:";
    analyze(a); // a 有名字，能取地址 (&a 是合法的)

    // 2. 最简单的右值
    std::cout << "分析 10:";
    analyze(10); // 10 是字面量，没地址，没名字

    // 3. 表达式的结果
    std::cout << "分析 a+1:";
    analyze(a + 1); // a+1 的结果是一个临时产生的 int，用完即弃，所以是右值

    // 4. 函数返回值
    std::cout << "分析 getTemp():";
    analyze(getTemp()); // 返回的是临时变量，右值

    std::cout << "分析 getGlobal():";
    analyze(getGlobal()); // 返回的是引用（别名），指向固定的内存，左值

    // 5. 所有的 "变量" 都是左值，哪怕它的类型是右值引用！
    int&& rRef = 20; // rRef 是一个类型为 "右值引用" 的变量
    std::cout << "分析 rRef:";
    analyze(rRef); // ⚠️ 注意！rRef 本身有名字，所以它是左值！
    
    // 6. std::move 的作用
    std::cout << "分析 std::move(a):";
    analyze(std::move(a)); // 强制把左值伪装成右值
}

int main() {
    valueCategoryDemo();
    return 0;
}
