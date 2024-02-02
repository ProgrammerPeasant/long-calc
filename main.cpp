#include "lib/InfFloat.cpp"
#include <iostream>

int main() {
    auto a = InfFloat("45.02");
    a.SetPrecision(20);
    auto b = InfFloat("612.335");
    std::cout << a / b << std::endl << a * b << std::endl << a + b << std::endl << "3.9999"_inf;
}