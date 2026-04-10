#include <iostream>

class A {
    unsigned long long value;
    A* next;
    bool isNumber;

public:
    A(): next(nullptr) {
        if (std::cin >> value) {
            isNumber = true;
            next = new A();
        }
    }

    ~A() {
        if (isNumber){
            delete next;
            std::cout << value << " ";

        }
    }
};

int main() {
    A a;
    return 0;
}
