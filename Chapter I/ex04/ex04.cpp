#include "Noeud.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>

void checkArbre(Noeud* n) {
    if (!n) throw std::invalid_argument("Noeud null");
    
    switch(n->type) {
        case TypeNoeud::STATE:
            if (n->enfantG || n->enfantD)
                throw std::logic_error("Feuille ne doit pas avoir d'enfants");
            break;
        case TypeNoeud::VAR:
            if (n->enfantG || n->enfantD)
                throw std::logic_error("Feuille ne doit pas avoir d'enfants");
            break;
        case TypeNoeud::NOT:
            if (!n->enfantG)
                throw std::logic_error("Operateur unaire sans enfant");
            checkArbre(n->enfantG);
            break;
        case TypeNoeud::AND:
        case TypeNoeud::OR:
        case TypeNoeud::XOR:
        case TypeNoeud::CMP:
        case TypeNoeud::EQ:
            if (!n->enfantG || !n->enfantD)
                throw std::logic_error("Operateur binaire sans enfants");
            checkArbre(n->enfantG);
            checkArbre(n->enfantD);
            break;
    }
}

Noeud* construireArbreRPN(const std::string& formula,
                          std::unordered_map<char, bool>& variables)
{
    std::stack<Noeud*> s;
    std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string ops = "!&|^>=";

    for (char c : formula) {

        if (c == '0' || c == '1') {
            s.push(new Noeud(c == '1'));
        }

        else if (letters.find(c) != std::string::npos) {
            if (variables.find(c) == variables.end())
                variables[c] = false;
            s.push(new Noeud(c));
        }

        else if (c == '!') {
            if (s.size() < 1) throw std::invalid_argument("Manque opérande !");
            Noeud* e = s.top(); s.pop();
            s.push(new Noeud(TypeNoeud::NOT, e));
        }

        else {
            if (s.size() < 2) throw std::invalid_argument("Manque opérandes");
            Noeud* d = s.top(); s.pop();
            Noeud* g = s.top(); s.pop();

            TypeNoeud t;
            if (c == '&') t = TypeNoeud::AND;
            else if (c == '|') t = TypeNoeud::OR;
            else if (c == '^') t = TypeNoeud::XOR;
            else if (c == '>') t = TypeNoeud::CMP;
            else if (c == '=') t = TypeNoeud::EQ;
            else throw std::invalid_argument("Opérateur inconnu");

            s.push(new Noeud(t, g, d));
        }
    }

    if (s.size() != 1)
        throw std::invalid_argument("Formule incorrecte");

    return s.top();
}


bool eval(Noeud* n, const std::unordered_map<char,bool>& vars)
{
    switch(n->type) {
        case TypeNoeud::STATE: return n->valeur;
            case TypeNoeud::VAR: return vars.at(n->nomVar);
            case TypeNoeud::NOT: return !eval(n->enfantG, vars);
            case TypeNoeud::AND: return eval(n->enfantG, vars) && eval(n->enfantD, vars);
            case TypeNoeud::OR: return eval(n->enfantG, vars) || eval(n->enfantD, vars);
            case TypeNoeud::XOR: return eval(n->enfantG, vars) ^ eval(n->enfantD, vars);
            case TypeNoeud::CMP: return !eval(n->enfantG, vars) || eval(n->enfantD, vars);
            case TypeNoeud::EQ: return eval(n->enfantG, vars) == eval(n->enfantD, vars);
    }
    return false;
}

void print_truth_table(const std::string& formula)
{
    std::cout << formula << std::endl;
    std::unordered_map<char, bool> variables;

    Noeud* root = construireArbreRPN(formula, variables);

    std::vector<char> varList;
    for (auto& p : variables)
        varList.push_back(p.first);

    std::sort(varList.begin(), varList.end());

    size_t n = varList.size();
    size_t rows = 1ull << n;

    std::cout << "| ";
    for (char c : varList) std::cout << c << " | ";
    std::cout << "= |\n";

    std::cout << "|";
    for (size_t i = 0; i < n; i++) std::cout << "---|";
    std::cout << "---|\n";

    for (size_t i = 0; i < rows; i++) {
        for (size_t b = 0; b < n; b++) {
            bool value = (i >> (n - b - 1)) & 1;
            variables[varList[b]] = value;
        }
        std::cout << "| ";
        for (char c : varList)
            std::cout << variables[c] << " | ";

        std::cout << eval(root, variables) << " |\n";
    }
    std::cout<< "__________________________________" << std::endl;
}

int main (void){
    try {
        print_truth_table("AB&");
        print_truth_table("AB|");
        print_truth_table("AB^");
        print_truth_table("A!B&");
        print_truth_table("AB>BA>=");
        print_truth_table("AB&C|");
        print_truth_table("AB>C>");  
        print_truth_table("AB|CD|&EF|&");
    } catch (const std::invalid_argument& e){
        std::cout << "Erreur : " << e.what() << std::endl;
    }
}