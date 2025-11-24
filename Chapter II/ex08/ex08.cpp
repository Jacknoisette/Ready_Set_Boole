#include <vector>
#include <iostream>
#include <algorithm>

std::vector<std::vector<int>> powerset(const std::vector<int>& set){
    std::vector<std::vector<int>> result;
    size_t powerset_size = 1 << set.size();
    for (size_t pos = 0; pos < powerset_size; ++pos){
        std::vector<int> tmp;
        for (size_t i = 0; i < set.size(); ++i) {
            if ((pos >> i) & 1){
                tmp.push_back(set[i]);
            }
        }
        result.push_back(tmp);
    }
    return result;
}

int main(void) {
    std::vector<int> set = {1, 2, 3, 4, 5};
    std::vector<std::vector<int>> result = powerset(set);
    std::sort(result.begin(), result.end(), 
        [](const std::vector<int>& a, const std::vector<int>& b) {
            if (a.size() != b.size()) {
                return a.size() < b.size();
            }
            return a < b;
        });

    std::cout << "Powerset de {1, 2, 3} :\n";
    std::cout << "{\n";
    for (const auto& subset : result) {
        std::cout << "  {";
        for (size_t i = 0; i < subset.size(); ++i) {
            std::cout << subset[i];
            if (i < subset.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "}\n";
    }
    std::cout << "}\n";

    return 0;
}