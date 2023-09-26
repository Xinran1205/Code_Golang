#include <iostream>
#include "add.h"

//如果我们加上using namespace std，这样我们cout前面的都可以省略
//https://blog.csdn.net/whc18858/article/details/124588910
//这篇博客写的很好，讲解了namespace

int main(){
    std::cout << add(1, 2) << std::endl;
    std::cout << "Hello World!";
    return 0;
}