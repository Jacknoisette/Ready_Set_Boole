#include "Noeud.hpp"
#include <stdexcept>

void checkArbre(Noeud* n) {
    if (!n) throw std::invalid_argument("Noeud null");
    
    switch(n->type) {
        case TypeNoeud::STATE:
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

Noeud* construireArbreRPN(const std::string& formula) {
    std::stack<Noeud*> s;
    std::string symbole = "01!&|^>=";
    for (char c : formula) {
        if (symbole.find(c) == std::string::npos)
            throw std::invalid_argument("Caractère invalide dans la formule");
        if (c == '0' || c == '1') {
            s.push(new Noeud(c == '1'));
        } else if (c == '!') {
            if (s.empty()) throw std::invalid_argument("Formule Incorrect");
            Noeud* e = s.top(); s.pop();
            s.push(new Noeud(TypeNoeud::NOT, e));
        } else {
            if (s.empty()) throw std::invalid_argument("Formule Incorrect");
            Noeud* d = s.top(); s.pop();
            if (s.empty()) throw std::invalid_argument("Formule Incorrect");
            Noeud* g = s.top(); s.pop();
            TypeNoeud t;
            if (c == '&') t = TypeNoeud::AND;
            else if (c == '|') t = TypeNoeud::OR;
            else if (c == '^') t = TypeNoeud::XOR;
            else if (c == '>') t = TypeNoeud::CMP;
            else if (c == '=') t = TypeNoeud::EQ;
            else throw std::invalid_argument("Valeur Incorrect");
            s.push(new Noeud(t, g, d));
        }
    }
    if (s.size() != 1)
        throw std::invalid_argument("Formule Incorrect");
    return s.top();
}

bool eval(Noeud* n) {
    switch(n->type) {
        case TypeNoeud::STATE: return n->valeur;
        case TypeNoeud::NOT: return !eval(n->enfantG);
        case TypeNoeud::AND: return eval(n->enfantG) && eval(n->enfantD);
        case TypeNoeud::OR:  return eval(n->enfantG) || eval(n->enfantD);
        case TypeNoeud::XOR: return eval(n->enfantG) ^ eval(n->enfantD);
        case TypeNoeud::CMP: return !eval(n->enfantG) || eval(n->enfantD);
        case TypeNoeud::EQ: return eval(n->enfantG) == eval(n->enfantD);
    }
    return false;
}

bool eval_formula(std::string str){
    if (str.length() == 0)
        throw std::invalid_argument("Empty String");
    Noeud* tree = nullptr;
    try {
        tree = construireArbreRPN(str);
        if (tree == nullptr)
            throw std::invalid_argument("Incorrect value");
        checkArbre(tree);
        return (eval(tree));
    } catch (const std::invalid_argument& e){
        throw e;
    }
}

int main (void){
    try {
        std::cout << eval_formula("10&") << std::endl;
        std::cout << eval_formula("10|") << std::endl;
        std::cout << eval_formula("11>") << std::endl;
        std::cout << eval_formula("10=") << std::endl;
        std::cout << eval_formula("1011||=") << std::endl;
        std::cout << eval_formula("11^|") << std::endl;
    } catch (const std::invalid_argument& e){
        std::cout << "Erreur : " << e.what() << std::endl;
    }
}