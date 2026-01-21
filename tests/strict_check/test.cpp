#include <iostream>

consteval int get_value() {
    return 42;
}

int main() {
    constexpr int val = get_value();
    std::cout << "Value: " << val << std::endl;
    return 0;
}
