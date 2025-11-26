#include <iostream>

unsigned int gray_code(unsigned int n){
    int tmp = n >> 1;
    n = n ^ tmp;
    return n;
}

int main (void){
    for (unsigned int n = 0; n < 20; n++){
        std::cout << "n " << n << " : " << gray_code(n) << std::endl;
    }
}