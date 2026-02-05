#include <iostream>
#include <memory>
#include <string>

// 这是一个演示循环引用的经典案例：Parent 和 Child 互相持有
struct Child;
struct Parent;

struct Parent {
    std::string name;
    // ⚠️ 错误写法：使用 shared_ptr 持有 Child
    std::shared_ptr<Child> child; 
    
    Parent(std::string n) : name(n) { std::cout << "Parent " << name << " created" << std::endl; }
    ~Parent() { std::cout << "Parent " << name << " destroyed" << std::endl; }
};

struct Child {
    std::string name;
    // ⚠️ 错误写法：使用 shared_ptr 反向持有 Parent，形成闭环
    std::shared_ptr<Parent> parent;
    
    Child(std::string n) : name(n) { std::cout << "Child " << name << " created" << std::endl; }
    ~Child() { std::cout << "Child " << name << " destroyed" << std::endl; }
};

void circularReferenceLeak() {
    std::cout << "\n--- 1. 循环引用导致内存泄漏 ---" << std::endl;
    {
        auto father = std::make_shared<Parent>("Father");
        auto son = std::make_shared<Child>("Son");
        
        // 建立相互引用
        father->child = son;
        son->parent = father;
        
        std::cout << "Father use_count: " << father.use_count() << std::endl; // 应该是 2 (local + son)
        std::cout << "Son use_count: " << son.use_count() << std::endl;       // 应该是 2 (local + father)
    }
    std::cout << "函数作用域结束，但没有看到析构函数打印？因为内存泄漏了！" << std::endl;
}

// ==========================================

// 正确写法：使用 weak_ptr 打破循环
struct GoodChild;
struct GoodParent {
    std::string name;
    std::shared_ptr<GoodChild> child; // 父节点拥有子节点（强引用）
    
    GoodParent(std::string n) : name(n) { std::cout << "GoodParent " << name << " created" << std::endl; }
    ~GoodParent() { std::cout << "GoodParent " << name << " destroyed" << std::endl; }
};

struct GoodChild {
    std::string name;
    // ✅ 正确写法：使用 weak_ptr 观察 Parent，不增加引用计数
    std::weak_ptr<GoodParent> parent;
    
    GoodChild(std::string n) : name(n) { std::cout << "GoodChild " << name << " created" << std::endl; }
    ~GoodChild() { std::cout << "GoodChild " << name << " destroyed" << std::endl; }
    
    void sayHiToDad() {
        // 使用 weak_ptr 必须先 lock() 升级为 shared_ptr 才能访问
        if (auto p = parent.lock()) {
            std::cout << name << " says: Hi " << p->name << "!" << std::endl;
        } else {
            std::cout << name << " says: My parent is gone..." << std::endl;
        }
    }
};

void weakPtrFix() {
    std::cout << "\n--- 2. 使用 weak_ptr 解决循环引用 ---" << std::endl;
    {
        auto father = std::make_shared<GoodParent>("GoodFather");
        auto son = std::make_shared<GoodChild>("GoodSon");
        
        father->child = son;
        son->parent = father; // 只是弱引用，不增加计数
        
        std::cout << "Father use_count: " << father.use_count() << std::endl; // 应该是 1 (只被局部变量持有)
        std::cout << "Son use_count: " << son.use_count() << std::endl;       // 应该是 2 (局部 + father)
        
        son->sayHiToDad();
    }
    std::cout << "函数作用域结束，应该看到析构函数被调用！" << std::endl;
}

int main() {
    circularReferenceLeak();
    weakPtrFix();
    return 0;
}
