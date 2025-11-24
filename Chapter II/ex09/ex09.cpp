#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>

void buildRPNTree(const std::string& formula,
                        std::unordered_map<char, std::vector<int>>& var,
                        std::vector<char>& op,
                        const std::vector<std::vector<int>>& sets)
{
    std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string ops = "!&|^>=";

    for (char c : formula) {
        if (letters.find(c) != std::string::npos) {
            if (var.find(c) == var.end()){
                if (sets.size() < var.size() + 1)
                    throw std::logic_error("Number of Sets and Variables unequal");
                var[c] = sets[sets.size() - 1];
            }
        } else if (ops.find(c) != std::string::npos) {
            op.push_back(c);
        } else {
            throw std::invalid_argument("Character Incorrect");
        }
    }

    if (op.size() < 1)
        throw std::invalid_argument("Formula incorrect");

    return ;
}

void apply_negation(){

}

void apply_conjunction(){

}

void apply_disjonction(){

}

void apply_exclusive_disjonction(){

}

void apply_material_condition(){

}

void apply_equivalence(){

}

void executeTree(std::unordered_map<char, std::vector<int>>& var,
                        std::vector<char>& op, std::vector<int>& res){
    while(op.size()){
        char operation = op[op.size() - 1];
        switch (operation){
            case '!' :
                apply_negation();
            case '&' :
                apply_conjunction();
            case '|' :
                apply_disjonction();
            case '^' :
                apply_exclusive_disjonction();
            case '>' :
                apply_material_condition();
            case '=' :
                apply_equivalence();
        }
        
        op.pop_back();
    }
}

std::vector<int> eval_set(const std::string& formula, const std::vector<std::vector<int>>& sets){
    std::unordered_map<char, std::vector<int>> var;
    std::vector<char> op;
    std::vector<int> res;
    try{
        buildRPNTree(formula, var, op, sets);

    } catch (...){
        throw;
    }
}


int main(void) {
    std::vector<int> set = {1, 2, 3, 4, 5};
    std::vector<std::vector<int>> result = 
}