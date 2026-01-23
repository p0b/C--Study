#include <iostream>
#include <string>
using namespace std;

// 模拟开放寻址法哈希表
class OpenAddressingHash {
private:
    static const int SIZE = 10;
    string* table;
    bool* deleted;
    
public:
    OpenAddressingHash() {
        table = new string[SIZE];
        deleted = new bool[SIZE];
        for (int i = 0; i < SIZE; i++) {
            table[i] = "";
            deleted[i] = false;
        }
    }
    
    int hash(string key) {
        // 简单哈希：字符串长度
        return key.length() % SIZE;
    }
    
    void insert(string key) {
        int index = hash(key);
        cout << "插入 \"" << key << "\" -> 哈希值: " << index << endl;
        
        // 线性探测找空位
        while (table[index] != "" && !deleted[index]) {
            index = (index + 1) % SIZE;
            cout << "  碰撞！探测下一个位置: " << index << endl;
        }
        
        table[index] = key;
        deleted[index] = false;
        cout << "  成功存入位置: " << index << endl << endl;
    }
    
    bool find(string key) {
        int index = hash(key);
        int startIndex = index;
        
        cout << "查找 \"" << key << "\" -> 从位置 " << index << " 开始" << endl;
        
        while (true) {
            if (table[index] == "" && !deleted[index]) {
                cout << "  位置 " << index << " 为空，停止查找" << endl;
                return false; // 遇到空位，肯定不存在
            }
            
            if (table[index] == key && !deleted[index]) {
                cout << "  在位置 " << index << " 找到了！" << endl;
                return true;
            }
            
            index = (index + 1) % SIZE;
            cout << "  继续探测位置: " << index << endl;
            
            if (index == startIndex) {
                cout << "  回到起点，查找失败" << endl;
                return false;
            }
        }
    }
    
    void remove(string key) {
        int index = hash(key);
        int startIndex = index;
        
        cout << "删除 \"" << key << "\" -> 从位置 " << index << " 开始" << endl;
        
        while (true) {
            if (table[index] == "" && !deleted[index]) {
                cout << "  位置 " << index << " 为空，删除失败" << endl;
                return;
            }
            
            if (table[index] == key && !deleted[index]) {
                table[index] = ""; // 清空
                deleted[index] = true; // 标记为已删除
                cout << "  成功删除位置 " << index << " (标记为DELETED)" << endl;
                return;
            }
            
            index = (index + 1) % SIZE;
            if (index == startIndex) {
                cout << "  回到起点，删除失败" << endl;
                return;
            }
        }
    }
    
    void printState() {
        cout << "当前哈希表状态:" << endl;
        for (int i = 0; i < SIZE; i++) {
            if (table[i] != "") {
                cout << "  [" << i << "]: " << table[i];
                if (deleted[i]) cout << " (DELETED)";
                cout << endl;
            } else if (deleted[i]) {
                cout << "  [" << i << "]: [DELETED]" << endl;
            }
        }
        cout << endl;
    }
};

int main() {
    OpenAddressingHash hash;
    
    cout << "=== 插入阶段 ===" << endl;
    hash.insert("cat");     // hash=3
    hash.insert("dog");     // hash=3  
    hash.insert("fish");    // hash=4
    hash.insert("bird");    // hash=4
    hash.insert("mouse");   // hash=5
    
    hash.printState();
    
    cout << "=== 查找阶段 ===" << endl;
    hash.find("mouse");
    cout << endl;
    
    cout << "=== 删除阶段 ===" << endl;
    hash.remove("fish");
    hash.remove("dog");
    hash.printState();
    
    cout << "=== 删除后查找 ===" << endl;
    hash.find("bird");
    
    return 0;
}