#include <iostream>
#include <cmath>
using namespace std;

struct Node {
    int coef; // 系數
    int exp;  // 指數
    Node* link; // 指向下一個節點的指標
};

class Polynomial {
private:
    Node* head; // 頭節點

public:
    // 預設建構子
    Polynomial() {
        head = new Node{0, 0, nullptr};
        head->link = head; // 圓形鏈結
    }

    // 複製建構子
    Polynomial(const Polynomial& a) {
        head = new Node{0, 0, nullptr};
        head->link = head;
        Node* temp = a.head->link;
        while (temp != a.head) {
            addTerm(temp->coef, temp->exp);
            temp = temp->link;
        }
    }

    // 解構子
    ~Polynomial() {
        clear();
        delete head;
    }

    // 清除鏈結串列
    void clear() {
        Node* temp = head->link;
        while (temp != head) {
            Node* del = temp;
            temp = temp->link;
            delete del;
        }
        head->link = head;
    }

    // 插入多項式項
    void addTerm(int coef, int exp) {
        Node* prev = head;
        Node* curr = head->link;
        while (curr != head && curr->exp > exp) {
            prev = curr;
            curr = curr->link;
        }
        if (curr != head && curr->exp == exp) {
            curr->coef += coef; // 合併同類項
            if (curr->coef == 0) {
                prev->link = curr->link;
                delete curr;
            }
        } else {
            Node* newNode = new Node{coef, exp, curr};
            prev->link = newNode;
        }
    }

    // 輸入運算子重載
    friend std::istream& operator>>(std::istream& is, Polynomial& x) {
        int n, coef, exp;
        is >> n;
        for (int i = 0; i < n; ++i) {
            is >> coef >> exp;
            x.addTerm(coef, exp);
        }
        return is;
    }

    // 輸出運算子重載
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& x) {
        Node* temp = x.head->link;
        while (temp != x.head) {
            if (temp != x.head->link && temp->coef > 0) os << " + ";
            os << temp->coef << "x^" << temp->exp;
            temp = temp->link;
        }
        return os;
    }

    // 加法運算子重載
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = head->link;
        Node* p2 = b.head->link;
        while (p1 != head || p2 != b.head) {
            if (p1 == head) {
                result.addTerm(p2->coef, p2->exp);
                p2 = p2->link;
            } else if (p2 == b.head || p1->exp > p2->exp) {
                result.addTerm(p1->coef, p1->exp);
                p1 = p1->link;
            } else if (p1->exp < p2->exp) {
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

    // 減法運算子重載
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* temp = b.head->link;
        while (temp != b.head) {
            result.addTerm(-temp->coef, temp->exp);
            temp = temp->link;
        }
        return *this + result;
    }

    // 乘法運算子重載
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Node* temp1 = head->link;
        while (temp1 != head) {
            Node* temp2 = b.head->link;
            while (temp2 != b.head) {
                result.addTerm(temp1->coef * temp2->coef, temp1->exp + temp2->exp);
                temp2 = temp2->link;
            }
            temp1 = temp1->link;
        }
        return result;
    }

    // 賦值運算子重載
    Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            clear();
            Node* temp = a.head->link;
            while (temp != a.head) {
                addTerm(temp->coef, temp->exp);
                temp = temp->link;
            }
        }
        return *this;
    }

    // 多項式計算
    float Evaluate(float x) const {
        float result = 0;
        Node* temp = head->link;
        while (temp != head) {
            result += temp->coef * pow(x, temp->exp);
            temp = temp->link;
        }
        return result;
    }
};

int main() {
    Polynomial p1, p2;
    cout << "Enter first polynomial (n c1 e1 c2 e2 ...): ";
    cin >> p1;
    cout << "Enter second polynomial (n c1 e1 c2 e2 ...): ";
    cin >> p2;

    cout << "P1: " << p1 << endl;
    cout << "P2: " << p2 << endl;

    Polynomial sum = p1 + p2;
    cout << "P1 + P2: " << sum << endl;

    Polynomial diff = p1 - p2;
    cout << "P1 - P2: " << diff << endl;

    Polynomial prod = p1 * p2;
    cout << "P1 * P2: " << prod << endl;

    float x;
    cout << "Enter a value for x to evaluate P1: ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Evaluate(x) << endl;

    return 0;
}