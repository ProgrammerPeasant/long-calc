#include "InfFloat.cpp"
#include <iostream>

int main()
{
	auto a = InfFloat("12465.423");
	a.SetPrecision(20);
	auto b = InfFloat("83.4812");
	b.SetPrecision(20);
	auto expected = InfFloat("0.2");

	auto res = a / b;
	res.SetPrecision(40);
	std::cout << res << std::endl;
	res = a * b * -1;
	std::cout << res << std::endl;
	res = a + b;
	std::cout << res << std::endl;
	res = a - 100;
	std::cout << res << std::endl;
	return 0;
}