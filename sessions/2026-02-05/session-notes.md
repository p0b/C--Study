# 2026-02-05 学习笔记：现代 C++ 核心特性 (智能指针、移动语义、Lambda)

## 1. 课程内容安排
### 第一部分：智能指针详解
- **1.1 为什么需要智能指针**
    - 内存泄漏问题 (Manual memory management issues)
    - 多线程共享对象的传播与释放问题
    - **实战代码**：`exercises/smart_ptr_intro.cpp` (RAII vs Raw Pointer)
- **1.2 shared_ptr 原理与使用**
    - 基本结构 (Control block + raw pointer)
    - 引用计数机制 (Reference counting)
    - **实战代码**：`exercises/shared_ptr_demo.cpp`
    - 线程安全性：计数原子操作，但对象读写需加锁
- **1.3 shared_ptr 的常见问题**
    - 裸指针直接赋值的危险
    - 循环引用问题 (Circular references)
- **1.4 weak_ptr 的作用与使用**
    - 打破循环引用
    - 观察者模式 (`lock()`, `expired()`)
    - **实战代码**：`exercises/weak_ptr_demo.cpp`
- **1.5 unique_ptr 的特点**
    - 独占所有权
    - 移动语义 (`std::move`)
    - **实战代码**：`exercises/unique_ptr_demo.cpp`

### 第二部分：右值引用与移动语义
- **2.1 左值与右值定义**
    - 左值 (Lvalue)：有名字，有地址，生命周期长。
    - 右值 (Rvalue)：无名字 (临时对象)，无地址 (或者即将销毁)，生命周期短。
    - **实战代码**：`exercises/lvalue_rvalue_demo.cpp` (含 std::move 分析)
- **2.2 左值引用与右值引用**
- **2.3 移动构造函数与性能优化**
    - 深拷贝 vs 移动 (Stealing resources)
    - **核心逻辑**：抢夺指针 + 置空原指针 (Prevent double free)
    - **实战代码**：`exercises/move_semantics_perf.cpp` (**性能提升 13 万倍**)
- **2.4 std::move 的作用**
    - 强制类型转换 (Cast to rvalue)：把“有身份证的人”变成“黑户”，允许被抢夺。
- **2.5 完美转发 (Perfect Forwarding)**
    - `T&&` 与 `std::forward`
    - **实战代码**：`exercises/perfect_forwarding.cpp`
- **2.6 成员初始化列表 (Member Initialization List)**
    - 写法：`Constructor(int a) : member(a) {}`
    - 优势：直接初始化，避免“默认构造 + 赋值”的双重开销。
    - **实战代码**：`exercises/init_list_demo.cpp`

### 第三部分：Lambda 表达式
- **3.1 基本语法**
- **3.2 捕获列表 (Capture List)**
    - 值捕获 `[=]` vs 引用捕获 `[&]`
    - `mutable` 关键字
    - **实战代码**：`exercises/lambda_demo.cpp`
- **3.3 实际应用**
    - STL 算法配合 (`std::sort`, `std::for_each`)

### 第四部分：C++ 标准库简介 & 正则表达式
- **4.1 STL 容器**
    - `vector`: 动态数组，随机访问快，尾插快。
    - `map`: 红黑树，有序，Key-Value。
    - `unordered_map`: 哈希表，无序，查找极快 O(1)。
    - **实战代码**：`exercises/stl_containers.cpp`
- **4.2 正则表达式**
    - `regex_match`: 全文匹配
    - `regex_search`: 搜索子串
    - `regex_replace`: 替换
    - `R"(...)"`: 原始字符串字面量 (Raw String Literal)，避免转义反斜杠。
    - **捕获组 (Capture Groups)**: 使用 `()` 包裹子串，通过 `std::smatch[n]` 提取。
    - **实战代码**：`exercises/regex_demo.cpp` (应用), `exercises/regex_explain.cpp` (原理)

## 2. 核心知识点深度总结

### 2.1 智能指针核心原理

**RAII (Resource Acquisition Is Initialization)**
- 本质：把资源生命周期绑定到栈对象上。
- 优势：栈对象销毁时自动调用析构函数，无论是正常返回还是异常退出。
- 对比：裸指针在异常发生时无法自动释放，导致内存泄漏。

**智能指针选择口诀**：
- 默认选 `unique_ptr`（独占，零开销，性能最优）。
- 需要共享选 `shared_ptr`（引用计数，有原子操作开销）。
- 破除循环选 `weak_ptr`（弱引用，不增加计数）。

**循环引用问题**：
- 场景：A 持有 shared_ptr 指向 B，B 持有 shared_ptr 指向 A。
- 后果：双方引用计数永远 ≥ 1，永远不会释放，造成内存泄漏。
- 解决：将其中一方改为 weak_ptr（通常是子节点指向父节点）。

### 2.2 移动语义核心机制

**左值 vs 右值的本质区别**：
- **左值 (Lvalue)**：有名字，有地址，生命周期长（如变量 `int a`）。
- **右值 (Rvalue)**：无名字，临时对象，生命周期短（如字面量 `10`，表达式结果 `a+1`）。
- **关键反直觉点**：右值引用类型的变量本身是左值（因为它有名字）！

**移动构造函数的核心逻辑**：
1. **窃取资源**：`this->buffer = other.buffer;`（拿走对方的指针）
2. **置空原指针**：`other.buffer = nullptr;`（防止对方析构时释放资源）
3. **为什么必须置空**：如果不置空，当 `other` 析构时会 `delete buffer`，导致你刚拿到的指针变成野指针。

**std::move 的本质**：
- 不移动任何东西，只是强制类型转换（左值 → 右值引用）。
- 作用：告诉编译器"这个对象可以被抢夺了"，触发移动构造而非拷贝构造。
- 性能提升：实测从深拷贝到移动构造，性能提升 **13 万倍**（100MB 数据）。

**完美转发 (Perfect Forwarding)**：
- 问题：右值引用参数在函数内部变成了左值（因为有名字）。
- 解决：使用 `std::forward<T>(arg)` 恢复原始属性。
- 应用场景：泛型工厂函数（如 `std::make_shared`, `emplace_back`）。

### 2.3 成员初始化列表

**语法**：`Constructor(int a) : member(a) {}`

**优势**：
- 直接调用成员的带参构造函数，一步到位。
- 避免"默认构造 + 赋值"的双重开销。

**必须使用的场景**：
- 成员变量是 `const` 类型。
- 成员变量是引用类型（`int&`）。
- 成员变量的类型没有默认构造函数。

### 2.4 Lambda 表达式

**捕获列表**：
- `[=]`：值捕获，捕获外部变量的副本（默认 const，需 `mutable` 才能修改）。
- `[&]`：引用捕获，直接引用外部变量（可修改，但要小心悬空引用）。
- `[x, &y]`：混合捕获，`x` 按值，`y` 按引用。

**实际应用**：
- 配合 STL 算法（`std::sort`, `std::for_each`）。
- 简化回调函数编写（不用定义全局函数或函数对象）。

### 2.5 STL 容器选择

| 容器 | 底层实现 | 时间复杂度 | 有序性 | 适用场景 |
|------|----------|------------|--------|----------|
| `vector` | 动态数组 | 随机访问 O(1)，尾插 O(1) | 插入顺序 | 默认首选，频繁随机访问 |
| `map` | 红黑树 | 查找/插入 O(log n) | 按 Key 排序 | 需要有序遍历 |
| `unordered_map` | 哈希表 | 查找/插入 O(1) | 无序 | 追求极致查找性能 |

### 2.6 正则表达式关键语法

**原始字符串 R"(...)"**：
- 问题：正则中的 `\d` 在 C++ 字符串里要写成 `"\\d"`（双斜杠）。
- 解决：`R"(\d)"` 所见即所得，避免转义地狱。

**捕获组 (Capture Groups)**：
- 语法：用 `()` 包裹需要提取的部分。
- 提取：`std::smatch matches; matches[0]` 是全匹配，`matches[1]` 是第一个括号内容。
- 示例：`R"(ID: (\d+))"` 可以从 `"ID: 9527"` 中提取出 `9527`。

## 3. 重要问题与解答记录

### Q1: 循环引用为什么会导致内存泄漏？
**A**: A 和 B 互相持有 `shared_ptr`，双方引用计数都是 2（局部变量 + 对方的成员）。当局部变量销毁后，计数降为 1，但永远不会到 0，因此永远不会释放。

### Q2: 为什么移动构造函数必须把原对象的指针置空？
**A**: 如果不置空，原对象析构时会 `delete` 那块内存，而新对象还在使用它，导致新对象的指针变成野指针（Use After Free）。

### Q3: `unique_ptr` 为什么必须用 `std::move` 传参？
**A**: `unique_ptr` 禁止拷贝（独占所有权）。如果不 `move`，就会发生拷贝，导致两个人同时拥有，违反了"独占"原则。`move` 后，原指针变成 `nullptr`，所有权完全转移。

### Q4: 成员初始化列表和函数体内赋值有什么区别？
**A**: 
- **初始化列表**：直接调用带参构造，一步到位（高效）。
- **函数体赋值**：先调用默认构造，再调用赋值操作符（低效，两步操作）。
- **实测结果**：初始化列表只打印"带参构造"，函数体赋值打印"默认构造 + 赋值"。

### Q5: Lambda 的值捕获和引用捕获有什么风险？
**A**: 
- **值捕获**：捕获的是副本，原变量改变不影响 Lambda。
- **引用捕获**：直接引用原变量，要小心 Lambda 的生命周期长于被捕获变量（悬空引用）。

### Q6: 正则表达式的捕获组是干什么的？
**A**: 捕获组用 `()` 包裹正则的一部分，表示"我要提取这部分内容"。`std::smatch[n]` 可以按顺序获取每个括号里匹配到的内容。

## 4. 性能测试数据

| 测试项 | 数据规模 | 性能结果 | 代码位置 |
|--------|----------|----------|----------|
| 深拷贝 vs 移动 | 100MB 内存 | 移动比拷贝快 **13 万倍** | `move_semantics_perf.cpp` |
| shared_ptr 引用计数 | 单个对象 | 拷贝后计数 +1，销毁后计数 -1 | `shared_ptr_demo.cpp` |
| 循环引用泄漏 | 2 个对象 | 作用域结束后无析构日志（泄漏） | `weak_ptr_demo.cpp` |

## 5. 练习代码索引
- `exercises/smart_ptr_intro.cpp`: RAII 基础演示
- `exercises/shared_ptr_demo.cpp`: shared_ptr 引用计数观察
- `exercises/weak_ptr_demo.cpp`: 循环引用与 weak_ptr 解决方案
- `exercises/unique_ptr_demo.cpp`: unique_ptr 与移动语义
- `exercises/move_semantics_perf.cpp`: 深拷贝 vs 移动构造性能对比
- `exercises/perfect_forwarding.cpp`: 完美转发演示
- `exercises/lambda_demo.cpp`: Lambda 捕获与 STL 应用
- `exercises/init_list_demo.cpp`: 成员初始化列表 vs 赋值
- `exercises/lvalue_rvalue_demo.cpp`: 左值右值详解
- `exercises/stl_containers.cpp`: Vector, Map, UnorderedMap 对比
- `exercises/regex_demo.cpp`: 正则表达式匹配与提取
- `exercises/regex_explain.cpp`: 正则表达式语法与捕获组解析
