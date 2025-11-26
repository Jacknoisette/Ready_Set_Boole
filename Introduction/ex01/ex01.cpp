#include <iostream>

unsigned int adder(unsigned int a, unsigned int b){
    while (b != 0){
        int tmp = (a & b) << 1;
        a = a ^ b;
        b = tmp;
    }
    return a;
}

unsigned int multiplier(unsigned int a, unsigned int b){
    unsigned int product = 0;
    while (b != 0){
        if (b & 1){
            product = adder(product, a);
        }
        a <<= 1;
        b >>= 1;
    }
    return product;
}

void bit_print(std::string name, unsigned int n){
    int start = 3;
    std::cout << name << " " << n << " : ";
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

unsigned int multiplier_with_print(unsigned int a, unsigned int b){
    std::cout << "Multiplication with :\n";
    bit_print("a", a);
    bit_print("b", b);
    int step = 0;
    unsigned int product = 0;
    while (b != 0){
        step++;
        if (b & 1){
            product = adder(product, a);
        }
        a <<= 1;
        b >>= 1;
        std::cout << "Mutliplication Step " << step << "/" << b << std::endl;
        bit_print("a", a);
        bit_print("b", b);
        bit_print("product", product);
    }
    return product;
}

int main(){
    int a = 11;
    int b = 20;
    bit_print("a", a);
    bit_print("b", b);
    std::cout << multiplier(a,b) << std::endl;
    multiplier_with_print(a, b);
}