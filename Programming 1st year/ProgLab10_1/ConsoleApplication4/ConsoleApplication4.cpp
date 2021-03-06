#include <iostream>

using namespace std;

extern "C" int __fastcall sumFast(int, int);
extern "C" int __stdcall sumStd(int, int, int);
extern "C" int __cdecl sumCd(int, int, int);

int main() {
	int x = 2, a = 4, b = 3;
	std::cout <<"a+b= "<< sumFast(a, b)<<std::endl;
	std::cout << "a*x+b= " << sumStd(a, b, x) << std::endl;
	std::cout << "a*x+b= " << sumCd(a, b, x) << std::endl;
	system("pause");
	return 0;
}