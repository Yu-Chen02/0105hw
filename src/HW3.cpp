#include <iostream>
#include <cmath>
using namespace std;

// Polynomial 類別：用來表示與操作多項式
class Polynomial {
private:
    // 節點結構：表示多項式中的一個項
    struct Node {
        int coef;   // 項的係數 (coefficient)
        int exp;    // 項的指數 (exponent)
        Node* link; // 指向下一個節點的指標
    };
    Node* head; // 頭節點，用於形成循環鏈結串列

public:
    // 預設建構函式：初始化空的循環鏈結串列
    Polynomial() {
        head = new Node{0, 0, nullptr}; // 建立一個虛擬節點
        head->link = head; // 將虛擬節點連結回自身，形成循環
    }

    // 解構函式：釋放動態記憶體，避免記憶體洩漏
    ~Polynomial() {
        clear();      // 清空鏈結串列
        delete head;  // 刪除頭節點
    }

    // 清空多項式：刪除所有節點，保留頭節點
    void clear() {
        Node* current = head->link;
        while (current != head) { // 遍歷所有節點
            Node* temp = current; // 暫存當前節點
            current = current->link; // 移動到下一個節點
            delete temp; // 釋放記憶體
        }
        head->link = head; // 恢復循環鏈結串列
    }

    // 複製建構函式：用於複製另一個 Polynomial 的內容
    Polynomial(const Polynomial& other) {
        head = new Node{0, 0, nullptr};
        head->link = head;
        Node* current = other.head->link;
        while (current != other.head) { // 遍歷另一個多項式的節點
            addTerm(current->coef, current->exp); // 添加到當前多項式
            current = current->link;
        }
    }

    // 賦值運算子：將另一個多項式的內容複製到當前多項式
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) { // 避免自我賦值
            clear(); // 清空當前多項式
            Node* current = other.head->link;
            while (current != other.head) { // 遍歷另一個多項式的節點
                addTerm(current->coef, current->exp); // 添加到當前多項式
                current = current->link;
            }
        }
        return *this;
    }

    // 添加一個項到多項式
    void addTerm(int coef, int exp) {
        if (coef == 0) return; // 係數為 0 的項忽略

        Node* prev = head;
        Node* current = head->link;

        // 找到插入位置：確保指數按降序排列
        while (current != head && current->exp > exp) {
            prev = current;
            current = current->link;
        }

        if (current != head && current->exp == exp) {
            // 若已有相同指數的項，則合併係數
            current->coef += coef;
            if (current->coef == 0) { // 若係數加總後為 0，刪除該項
                prev->link = current->link;
                delete current;
            }
        } else {
            // 否則，插入新項
            Node* newNode = new Node{coef, exp, current};
            prev->link = newNode;
        }
    }

    // 評估多項式值：計算 P(x)
    float Evaluate(float x) const {
        float result = 0;
        Node* current = head->link;
        while (current != head) { // 遍歷所有節點
            result += current->coef * pow(x, current->exp); // coef * x^exp
            current = current->link;
        }
        return result;
    }

    // 重載輸入運算子：從輸入流讀取多項式
    friend istream& operator>>(istream& is, Polynomial& x) {
        int n;
        is >> n; // 讀取多項式項數
        for (int i = 0; i < n; ++i) {
            int coef, exp;
            is >> coef >> exp; // 讀取每一項的係數與指數
            x.addTerm(coef, exp); // 添加該項
        }
        return is;
    }

    // 重載輸出運算子：將多項式格式化輸出
    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        Node* current = x.head->link;
        bool first = true; // 控制是否添加 "+" 號
        while (current != x.head) {
            if (!first && current->coef > 0) os << " + ";
            if (current->coef < 0) os << " - ";
            os << abs(current->coef);
            if (current->exp != 0) os << "x^" << current->exp;
            first = false;
            current = current->link;
        }
        return os;
    }

    // 重載加法運算子
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = head->link;
        Node* p2 = b.head->link;

        // 合併兩個多項式
        while (p1 != head || p2 != b.head) {
            if (p2 == b.head || (p1 != head && p1->exp > p2->exp)) {
                result.addTerm(p1->coef, p1->exp);
                p1 = p1->link;
            } else if (p1 == head || (p2 != b.head && p2->exp > p1->exp)) {
                result.addTerm(p2->coef, p2->exp);
                p2 = p2->link;
            } else {
                result.addTerm(p1->coef + p2->coef, p1->exp);
                p1 = p1->link;
                p2 = p2->link;
            }
        }

        return result;
    }

    // 重載減法運算子
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* p2 = b.head->link;
        while (p2 != b.head) {
            result.addTerm(-p2->coef, p2->exp); // 取反後添加
            p2 = p2->link;
        }
        return *this + result;
    }

    // 重載乘法運算子
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = head->link;

        while (p1 != head) {
            Node* p2 = b.head->link;
            while (p2 != b.head) {
                result.addTerm(p1->coef * p2->coef, p1->exp + p2->exp);
                p2 = p2->link;
            }
            p1 = p1->link;
        }

        return result;
    }
};

// 主程式：執行多項式操作
int main() {
    Polynomial p1, p2;

    // 輸入多項式
    cout << "請輸入第一個多項式 (格式: n coef1 exp1 coef2 exp2 ...): ";
    cin >> p1;
    cout << "請輸入第二個多項式 (格式: n coef1 exp1 coef2 exp2 ...): ";
    cin >> p2;

    // 輸出多項式
    cout << "P1: " << p1 << endl;
    cout << "P2: " << p2 << endl;

    // 多項式加法
    Polynomial sum = p1 + p2;
    cout << "P1 + P2: " << sum << endl;

    // 多項式減法
    Polynomial diff = p1 - p2;
    cout << "P1 - P2: " << diff << endl;

    // 多項式乘法
    Polynomial prod = p1 * p2;
    cout << "P1 * P2: " << prod << endl;

    // 評估多項式值
    float x;
    cout << "請輸入 x 的值來評估 P1(x): ";
    cin >> x;
    cout << "P1(" << x << "): " << p1.Evaluate(x) << endl;

    cout << "請輸入 x 的值來評估 P2(x): ";
    cin >> x;
    cout << "P2(" << x << "): " << p2.Evaluate(x) << endl;

    return 0;
}
