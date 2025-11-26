#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <sstream>
#include <iomanip>

uint32_t getbinary(uint16_t x, uint16_t y){
    uint32_t res = 0;
    for (int i = 0; i < 16; ++i){
        uint32_t x_bit = (x >> i) & 1;
        uint32_t y_bit = (y >> i) & 1;
        res |= (x_bit << (2 * i));
        res |= (y_bit << (2 * i + 1));
    }
    return res;
}

double map(uint16_t x, uint16_t y){
    uint32_t index = getbinary(x, y);
    std::cout << "Binary with : ("<< x << "," << y << ")" << std::endl;
    std::cout << "After Morton Code : " << index << std::endl;
    double total_points = 4294967296.0;
    
    return static_cast<double>(index) / total_points;
}

void test(uint16_t x, uint16_t y){
    std::cout << "____________________" << std::endl;
    std::stringstream msgs;
    msgs << "For a map of " << sizeof(uint16_t) * 8 << " * " << sizeof(uint16_t) * 8 << " and value coordinate (" << x << "," << y << ") the result with Morton Code is : ";
    double res = map(x, y);
    std::string msg = msgs.str();
    std::cout << msg;
    std::cout << std::fixed << std::setprecision(10) << res << std::endl;
    std::cout << std::endl;
}

int main(void){
    test(65535, 65535);
    test(0, 0);
    test(10, 12);
}