#include <iostream>


unsigned int adder(unsigned int a, unsigned int b){
    while (b != 0){
        int tmp = (a & b) << 1;
        a = a ^ b;
        b = tmp;
    }
    return a;
}

void bit_print(std::string name, unsigned int n){
    int start = 3;
    std::cout << name << " : ";
    bool started = false;
    for (int i = 31; i >= 0; i--) {
        int bit = (n >> i) & 1;
        if (bit) started = true;
        if (i <= start) started = true;
        if (started) std::cout << bit;
    }
    if (!started) std::cout << 0;
    std::cout << std::endl;
}

unsigned int adder_with_print(unsigned int a, unsigned int b){
    std::cout << "Addition with :\n";
    bit_print("a", a);
    bit_print("b", b);
    int step = 0;
    while (b != 0){
        step++;
        int tmp = (a & b) << 1;
        a = a ^ b;
        b = tmp;
        std::cout << "Step " << step << std::endl;
        bit_print("result", a);
        bit_print("carry", b);
    }
    bit_print("final result", a);
    return a;
}

int main(){
    int a = 6;
    int b = 7;
    bit_print("a", a);
    bit_print("b", b);
    std::cout << adder(a,b) << std::endl;
    adder_with_print(a, b);
}