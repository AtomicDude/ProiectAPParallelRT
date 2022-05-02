#include "vec3.h"
#include <iostream>

int main()
{
    rt::vec3 v1(1, 2, 3);
    rt::vec3 v2(2, 2, 2);
    v1 += v2;

    std::cout << v1;
}
