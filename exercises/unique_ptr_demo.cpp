#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Resource {
public:
    std::string data;
    Resource(std::string d) : data(d) { std::cout << "Resource " << data << " created" << std::endl; }
    ~Resource() { std::cout << "Resource " << data << " destroyed" << std::endl; }
};

void processResource(std::unique_ptr<Resource> res) {
    // res 的所有权已经转移到了这个函数内部
    std::cout << "Processing: " << res->data << std::endl;
    // 函数结束时，res 销毁，资源自动释放
}

void uniquePtrDemo() {
    std::cout << "--- unique_ptr 移动语义演示 ---" << std::endl;

    // 1. 创建 unique_ptr
    std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>("Document1");

    // 2. 尝试复制 (编译错误)
    // std::unique_ptr<Resource> ptr2 = ptr1; // ❌ 错误！unique_ptr 禁止复制构造
    
    // 3. 移动所有权 (Move)
    std::cout << "移动所有权给 ptr2..." << std::endl;
    std::unique_ptr<Resource> ptr2 = std::move(ptr1); // ✅ 必须显式使用 std::move

    if (!ptr1) {
        std::cout << "ptr1 现在是空的 (nullptr)" << std::endl;
    }
    std::cout << "ptr2 拥有资源: " << ptr2->data << std::endl;

    // 4. 作为函数参数传递 (必须 Move)
    std::cout << "传递所有权给函数..." << std::endl;
    processResource(std::move(ptr2)); 

    if (!ptr2) {
        std::cout << "ptr2 现在也是空的，资源已经在 processResource 中释放了" << std::endl;
    }
    
    // 5. 放在容器中
    std::cout << "\n--- 容器中的 unique_ptr ---" << std::endl;
    std::vector<std::unique_ptr<Resource>> vec;
    vec.push_back(std::make_unique<Resource>("Item1"));
    vec.push_back(std::make_unique<Resource>("Item2"));
    
    // vec.push_back(ptr1); // ❌ 错误：不能放入左值（除非 move）
    
    std::cout << "容器大小: " << vec.size() << std::endl;
    std::cout << "容器销毁中..." << std::endl;
}

int main() {
    uniquePtrDemo();
    return 0;
}
