#include <iostream>
#include <memory>
#include <string>

// 一个简单的类，用来观察构造和析构
class Component {
public:
    std::string name;
    Component(std::string n) : name(n) {
        std::cout << "  [构造] Component " << name << " 创建了" << std::endl;
    }
    ~Component() {
        std::cout << "  [析构] Component " << name << " 销毁了 (资源释放)" << std::endl;
    }
    void doWork() {
        std::cout << "  -> " << name << " 正在工作..." << std::endl;
    }
};

void sharedPtrDemo() {
    std::cout << "--- shared_ptr 引用计数演示 ---" << std::endl;

    // 1. 创建 shared_ptr
    // 推荐使用 std::make_shared，因为它更高效（内存分配一次完成）且异常安全
    std::shared_ptr<Component> p1 = std::make_shared<Component>("核心引擎");
    
    std::cout << "p1 创建后，引用计数: " << p1.use_count() << std::endl; // 应该是 1

    {
        // 2. 拷贝 shared_ptr (共享所有权)
        std::shared_ptr<Component> p2 = p1;
        std::cout << "p2 = p1 后，引用计数: " << p1.use_count() << std::endl; // 应该是 2 (p1和p2都指向它)
        
        p2->doWork();
        
        // 3. p2 离开作用域
        std::cout << "p2 即将离开作用域..." << std::endl;
    }
    
    // 此时 p2 死了，计数 -1
    std::cout << "p2 离开后，引用计数: " << p1.use_count() << std::endl; // 应该是 1

    // 4. 重置 p1
    std::cout << "准备重置 p1..." << std::endl;
    p1.reset(); // 主动放弃所有权
    
    std::cout << "p1 重置后，引用计数: " << p1.use_count() << std::endl; // 应该是 0
    std::cout << "Demo 结束" << std::endl;
}

int main() {
    sharedPtrDemo();
    return 0;
}
