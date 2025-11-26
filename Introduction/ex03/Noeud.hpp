#ifndef NOEUD_HPP
#define NOEUD_HPP

#include <iostream>
#include <stack>

enum class TypeNoeud {
    STATE, // 0 or 1
    NOT, // !
    AND, // &
    OR, // |
    XOR, // ^
    CMP, // >
    EQ // =
};

class Noeud {
    public :
        TypeNoeud type;
        bool valeur;
        Noeud *enfantG;
        Noeud *enfantD;
        Noeud(bool val) : type(TypeNoeud::STATE), valeur(val), enfantG(nullptr), enfantD(nullptr) {}
        Noeud(TypeNoeud t, Noeud* e) : type(t), enfantG(e), enfantD(nullptr) {}
        Noeud(TypeNoeud t, Noeud* g, Noeud* d) : type(t), enfantG(g), enfantD(d) {}
};

#endif