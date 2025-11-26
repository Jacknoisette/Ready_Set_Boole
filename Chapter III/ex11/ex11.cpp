#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <sstream>
#include <iomanip>

std::pair<uint16_t , uint16_t> getpair(uint32_t index){
    std::pair<uint16_t , uint16_t> res;
    for (int i = 0; i < 32; ++i){
        if (i % 2 == 0){
            uint32_t x_bit = (index >> i) & 1;
            res.first |= (x_bit << (i / 2));
        }
        else{
            uint32_t y_bit = (index >> i) & 1;
            res.second |= (y_bit << (i / 2));
        }
    }
    return res;
}

std::pair<uint16_t , uint16_t> reverse_map(double reel_nbr){
    if (reel_nbr < 0.0 || reel_nbr > 1.0) {
        throw std::logic_error("Error input must be between 0.0 and 1.0");
    }
    double total_points = 4294967296.0;
    uint32_t index;
    if (reel_nbr == 1.0) { 
        index = 0xFFFFFFFF;
    } else {
        index = static_cast<uint32_t>(reel_nbr * total_points);
    }
    std::cout << "Reel_nbr before conversion to uint32_t ";
    std::cout << std::fixed << std::setprecision(10) << reel_nbr << std::endl;
    std::cout << "After conversion uint32_t: " << index << std::endl;
    std::pair<uint16_t , uint16_t> res = getpair(index);
    return res;
}

void test(double reel_nbr){
    std::cout << "____________________" << std::endl;
    try {
        std::stringstream msgs;
        msgs << "For a map of " << sizeof(uint16_t) * 8 << " * " << sizeof(uint16_t) * 8 << " and a reel number of value " << std::fixed << std::setprecision(10) << reel_nbr << " the result of the decoding is : ";
        std::pair<uint16_t , uint16_t> res = reverse_map(reel_nbr);
        std::string msg = msgs.str();
        std::cout << msg;
        std::cout << "(" << res.first << "," << res.second << ")" << std::endl;
    } catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;
}

int main(void){
    test(1);
    test(0);
    test(-1);
    test(2);
    test(0.2);
    test(0.0000000531);
}