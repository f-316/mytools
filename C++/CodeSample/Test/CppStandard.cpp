#include <iostream>
#include <algorithm>
#include <vector>
#include "CppStandard.h"

CppStandard::CppStandard()
{
}

CppStandard::~CppStandard()
{
}

void CppStandard::Test()
{
	printf("Hello!\n");

    std::vector<int> v = { 3, 1, 4, 2, 5 };

    // ¸‡‚É•À‚×‘Ö‚¦‚é
    std::sort(v.begin(), v.end());

    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << x << std::endl;
        });
    std::cout << std::endl;

    // ~‡‚É•À‚×‘Ö‚¦‚é
    std::sort(v.begin(), v.end(), [](int a, int b) {
        return a > b;
        });
    // ‚±‚¿‚ç‚Å‚à‚æ‚¢
    // std::sort(v.begin(), v.end(), std::greater<int>{});

    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << x << std::endl;
        });
}
