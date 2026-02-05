#include <iostream>
#include <cstring>
#include <vector>
#include <chrono>

class BigMemoryPool {
public:
    char* buffer;
    size_t size;

    // 1. 构造函数
    BigMemoryPool(size_t s) : size(s) {
        buffer = new char[size];
        memset(buffer, 0, size); // 模拟耗时操作
        // std::cout << "  [构造] 分配了 " << size << " 字节" << std::endl;
    }

    // 2. 拷贝构造函数 (深拷贝) - C++98 的笨办法
    // 就像 "重新买个杯子倒水"
    BigMemoryPool(const BigMemoryPool& other) : size(other.size) {
        buffer = new char[size]; // 重新分配内存
        memcpy(buffer, other.buffer, size); // 复制数据
        // std::cout << "  [拷贝] 深拷贝了 " << size << " 字节" << std::endl;
    }

    // 3. 移动构造函数 (浅拷贝/资源窃取) - C++11 的新武器
    // 就像 "直接把杯子递给你，我不要了"
    // 注意参数是 BigMemoryPool&& (右值引用)
    BigMemoryPool(BigMemoryPool&& other) noexcept : buffer(other.buffer), size(other.size) {
        // 关键步骤：把对方置空！防止对方析构时释放这块内存
        other.buffer = nullptr;
        other.size = 0;
        // std::cout << "  [移动] 窃取了资源，耗时几乎为0" << std::endl;
    }

    // 析构函数
    ~BigMemoryPool() {
        if (buffer) {
            delete[] buffer;
            // std::cout << "  [析构] 释放资源" << std::endl;
        }
    }
};

void runPerformanceTest() {
    std::cout << "--- 性能大比拼: Copy vs Move ---" << std::endl;
    
    // 创建一个巨大的数据
    size_t dataSize = 100 * 1024 * 1024; // 100MB
    BigMemoryPool source(dataSize);

    // 测试拷贝性能
    auto start = std::chrono::high_resolution_clock::now();
    BigMemoryPool copy(source); // 触发拷贝构造
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> copyTime = end - start;
    std::cout << "拷贝构造耗时: " << copyTime.count() << " ms" << std::endl;

    // 测试移动性能
    start = std::chrono::high_resolution_clock::now();
    BigMemoryPool moved(std::move(source)); // 触发移动构造
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> moveTime = end - start;
    std::cout << "移动构造耗时: " << moveTime.count() << " ms" << std::endl;
    
    std::cout << "性能提升倍数: " << copyTime.count() / moveTime.count() << " 倍!" << std::endl;
    
    // 此时 source 里的 buffer 已经是 nullptr 了
}

int main() {
    runPerformanceTest();
    return 0;
}
