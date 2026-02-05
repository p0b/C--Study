#include <iostream>
#include <vector>
#include <algorithm> // for std::for_each

void lambdaDemo() {
    std::cout << "--- Lambda 表达式演示 ---" << std::endl;

    int x = 10;
    int y = 20;

    // 1. 基本语法 & 值捕获 [=]
    // 捕获 x 和 y 的副本。默认是 const 的，不能修改。
    auto printSum = [=]() {
        // x++; // ❌ 编译错误：read-only
        std::cout << "  [值捕获] x + y = " << x + y << std::endl;
    };
    printSum();

    // 2. 引用捕获 [&]
    // 捕获 x 和 y 的引用。可以修改。
    auto modifyValues = [&]() {
        x += 100;
        y += 100;
        std::cout << "  [引用捕获] 内部修改了 x, y" << std::endl;
    };
    modifyValues();
    std::cout << "  [外部验证] x=" << x << ", y=" << y << " (变了!)" << std::endl;

    // 3. mutable 关键字
    // 允许修改值捕获的“副本”，不影响外部
    auto mutableLambda = [x]() mutable {
        x += 50; // 修改的是 lambda 内部的副本
        std::cout << "  [mutable] 内部副本 x=" << x << std::endl;
    };
    mutableLambda();
    std::cout << "  [外部验证] 外部 x=" << x << " (没变!)" << std::endl;

    // 4. 实战：配合 STL 算法
    std::cout << "\n--- 实战：STL for_each ---" << std::endl;
    std::vector<int> nums = {1, 5, 3, 8, 2};
    
    // 排序
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b; // 降序
    });

    // 遍历打印
    int threshold = 4;
    std::cout << "大于 " << threshold << " 的数: ";
    std::for_each(nums.begin(), nums.end(), [threshold](int n) {
        if (n > threshold) {
            std::cout << n << " ";
        }
    });
    std::cout << std::endl;
}

int main() {
    lambdaDemo();
    return 0;
}
