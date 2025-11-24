#include "Noeud.hpp"
#include <iostream>
#include <stack>
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>

Noeud* deep_copy(Noeud* n) {
    if (!n) return nullptr;
    if (n->type == TypeNoeud::STATE) return new Noeud(n->valeur);
    if (n->type == TypeNoeud::VAR) return new Noeud(n->nomVar);
    if (n->type == TypeNoeud::NOT) return new Noeud(TypeNoeud::NOT, deep_copy(n->enfantG));
    return new Noeud(n->type, deep_copy(n->enfantG), deep_copy(n->enfantD));
}

void freeTree(Noeud* n) {
    if (!n) return;
    freeTree(n->enfantG);
    freeTree(n->enfantD);
    delete n;
}

Noeud* parseRPN(const std::string& formula) {
    std::stack<Noeud*> s;
    std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string ops_all = "01!&|^>=";

    for (char c : formula) {
        if (ops_all.find(c) == std::string::npos && letters.find(c) == std::string::npos) {
            throw std::invalid_argument(std::string("Caractère invalide: ") + c);
        }

        if (c == '0' || c == '1') {
            s.push(new Noeud(c == '1'));
        } else if (letters.find(c) != std::string::npos) {
            s.push(new Noeud(c));
        } else if (c == '!') {
            if (s.size() < 1) throw std::invalid_argument("Opérateur '!' sans opérande");
            Noeud* e = s.top(); s.pop();
            s.push(new Noeud(TypeNoeud::NOT, e));
        } else {
            if (s.size() < 2) throw std::invalid_argument(std::string("Opérateur binaire '") + c + "' sans opérandes suffisants");
            Noeud* d = s.top(); s.pop();
            Noeud* g = s.top(); s.pop();
            TypeNoeud t;
            if (c == '&') t = TypeNoeud::AND;
            else if (c == '|') t = TypeNoeud::OR;
            else if (c == '^') t = TypeNoeud::XOR;
            else if (c == '>') t = TypeNoeud::CMP;
            else if (c == '=') t = TypeNoeud::EQ;
            else throw std::invalid_argument(std::string("Opérateur inconnu: ") + c);
            s.push(new Noeud(t, g, d));
        }
    }

    if (s.size() != 1) throw std::invalid_argument("Formule RPN mal formée (pile finale != 1)");
    return s.top();
}

Noeud* rewrite_primitives(Noeud* n) {
    if (!n) return nullptr;

    n->enfantG = rewrite_primitives(n->enfantG);
    n->enfantD = rewrite_primitives(n->enfantD);

    if (n->type == TypeNoeud::XOR) {
        // (A & !B) | (!A & B)
        Noeud* A = deep_copy(n->enfantG);
        Noeud* B = deep_copy(n->enfantD);
        Noeud* notB = new Noeud(TypeNoeud::NOT, B);
        Noeud* notA = new Noeud(TypeNoeud::NOT, deep_copy(n->enfantG));
        Noeud* left = new Noeud(TypeNoeud::AND, A, notB);
        Noeud* right = new Noeud(TypeNoeud::AND, notA, deep_copy(n->enfantD));
        freeTree(n);
        return new Noeud(TypeNoeud::OR, left, right);
    }

    if (n->type == TypeNoeud::CMP) {
        // (!A) | B
        Noeud* notA = new Noeud(TypeNoeud::NOT, deep_copy(n->enfantG));
        Noeud* B = deep_copy(n->enfantD);
        freeTree(n);
        return new Noeud(TypeNoeud::OR, notA, B);
    }

    if (n->type == TypeNoeud::EQ) {
        // ((!A)|B) & ((!B)|A)
        Noeud* A = deep_copy(n->enfantG);
        Noeud* B = deep_copy(n->enfantD);
        Noeud* notA = new Noeud(TypeNoeud::NOT, deep_copy(n->enfantG));
        Noeud* notB = new Noeud(TypeNoeud::NOT, deep_copy(n->enfantD));
        Noeud* left = new Noeud(TypeNoeud::OR, notA, B);
        Noeud* right = new Noeud(TypeNoeud::OR, notB, A);
        freeTree(n);
        return new Noeud(TypeNoeud::AND, left, right);
    }

    return n;
}

Noeud* toNNF(Noeud* n, bool neg) {
    if (!n) return nullptr;

    switch (n->type) {
        case TypeNoeud::STATE: {
            bool val = n->valeur;
            if (neg) val = !val;
            return new Noeud(val);
        }
        case TypeNoeud::VAR: {
            if (!neg) return new Noeud(n->nomVar);
            return new Noeud(TypeNoeud::NOT, new Noeud(n->nomVar));
        }
        case TypeNoeud::NOT:
            return toNNF(n->enfantG, !neg);

        case TypeNoeud::AND: {
            if (!neg) {
                Noeud* g = toNNF(n->enfantG, false);
                Noeud* d = toNNF(n->enfantD, false);
                return new Noeud(TypeNoeud::AND, g, d);
            } else {
                Noeud* g = toNNF(n->enfantG, true);
                Noeud* d = toNNF(n->enfantD, true);
                return new Noeud(TypeNoeud::OR, g, d);
            }
        }

        case TypeNoeud::OR: {
            if (!neg) {
                Noeud* g = toNNF(n->enfantG, false);
                Noeud* d = toNNF(n->enfantD, false);
                return new Noeud(TypeNoeud::OR, g, d);
            } else {
                Noeud* g = toNNF(n->enfantG, true);
                Noeud* d = toNNF(n->enfantD, true);
                return new Noeud(TypeNoeud::AND, g, d);
            }
        }

        default:
            if (n->enfantG || n->enfantD) {
                Noeud* g = toNNF(n->enfantG, neg);
                Noeud* d = toNNF(n->enfantD, neg);
                return new Noeud(n->type, g, d);
            } else {
                return nullptr;
            }
    }
}

Noeud* distribute_or(Noeud* a, Noeud* b) {
    if (!a) return b;
    if (!b) return a;

    if (a->type == TypeNoeud::STATE) {
        bool va = a->valeur;
        freeTree(a);
        if (va) {
            freeTree(b);
            return new Noeud(true);
        } else {
            return b;
        }
    }
    if (b->type == TypeNoeud::STATE) {
        bool vb = b->valeur;
        freeTree(b);
        if (vb) {
            freeTree(a);
            return new Noeud(true);
        } else {
            return a;
        }
    }

    if (a->type == TypeNoeud::AND) {
        Noeud* ag = a->enfantG;
        Noeud* ad = a->enfantD;
        a->enfantG = a->enfantD = nullptr;
        freeTree(a);
        Noeud* b_copy = deep_copy(b);
        Noeud* left = distribute_or(ag, b);
        Noeud* right = distribute_or(ad, b_copy);
        return new Noeud(TypeNoeud::AND, left, right);
    }

    if (b->type == TypeNoeud::AND) {
        Noeud* bg = b->enfantG;
        Noeud* bd = b->enfantD;
        b->enfantG = b->enfantD = nullptr;
        freeTree(b);
        Noeud* a_copy = deep_copy(a);
        Noeud* left = distribute_or(a, bg);
        Noeud* right = distribute_or(a_copy, bd);
        return new Noeud(TypeNoeud::AND, left, right);
    }

    return new Noeud(TypeNoeud::OR, a, b);
}

Noeud *toCNF(Noeud* n){
    if (!n) return nullptr;

    switch (n->type) {
        case TypeNoeud::STATE: {
            Noeud *newNode = deep_copy(n);
            freeTree(n);
            return newNode;
        }
        case TypeNoeud::VAR: {
            Noeud *newNode = deep_copy(n);
            freeTree(n);
            return newNode;
        }
        case TypeNoeud::NOT: {
            Noeud *newNode = deep_copy(n);
            freeTree(n);
            return newNode;
        }
        case TypeNoeud::AND: {
            Noeud* g = toCNF(n->enfantG);
            Noeud* d = toCNF(n->enfantD);
            n->enfantG = n->enfantD = nullptr;
            Noeud *newNode = new Noeud(TypeNoeud::AND, g, d);
            freeTree(n);
            return newNode;
        }

        case TypeNoeud::OR: {
            Noeud* g = toCNF(n->enfantG);
            Noeud* d = toCNF(n->enfantD);
            n->enfantG = n->enfantD = nullptr;
            Noeud* res = distribute_or(g, d);
            freeTree(n);
            return res;
        }

        default: {
            Noeud *newNode = deep_copy(n);
            freeTree(n);
            return newNode;
        }
    }
}

void collect_disjuncts(Noeud* n, std::vector<Noeud*>& out) {
    if (!n) return;
    if (n->type == TypeNoeud::OR) {
        collect_disjuncts(n->enfantG, out);
        collect_disjuncts(n->enfantD, out);
    } else {
        out.push_back(n);
    }
}

void collect_conjuncts(Noeud* n, std::vector<Noeud*>& out) {
    if (!n) return;
    if (n->type == TypeNoeud::AND) {
        collect_conjuncts(n->enfantG, out);
        collect_conjuncts(n->enfantD, out);
    } else {
        out.push_back(n);
    }
}

void toRPN(Noeud* n, std::string& out) {
    if (!n) return;
    
    if (n->type == TypeNoeud::OR) {
        std::vector<Noeud*> parts;
        collect_disjuncts(n, parts);
        for (Noeud* p : parts) toRPN(p, out);
        for (size_t i = 1; i < parts.size(); ++i) out.push_back('|');
        return;
    }
    if (n->type == TypeNoeud::AND) {
        std::vector<Noeud*> parts;
        collect_conjuncts(n, parts);
        for (Noeud* p : parts) toRPN(p, out);
        for (size_t i = 1; i < parts.size(); ++i) out.push_back('&');
        return;
    }

    if (n->enfantG) toRPN(n->enfantG, out);
    if (n->enfantD) toRPN(n->enfantD, out);
    switch (n->type) {
        case TypeNoeud::STATE: out.push_back(n->valeur ? '1' : '0'); break;
        case TypeNoeud::VAR: out.push_back(n->nomVar); break;
        case TypeNoeud::NOT: out.push_back('!'); break;
        // case TypeNoeud::AND: out.push_back('&'); break;
        // case TypeNoeud::OR:  out.push_back('|'); break;
        default:
            throw std::logic_error("Unsupported node in toRPN");
    }
}

std::string conjunctive_normal_form(const std::string& formula) {

    Noeud* root = nullptr;
    try {
        root = parseRPN(formula);
    } catch (...) {
        if (root) freeTree(root);
        throw;
    }

    Noeud* rewritten = nullptr;
    try {
        rewritten = rewrite_primitives(root);
    } catch (...) {
        freeTree(root);
        throw;
    }

    Noeud* nnf = nullptr;
    try {
        nnf = toNNF(rewritten, false);
    } catch (...) {
        freeTree(rewritten);
        throw;
    }

    freeTree(rewritten);

    Noeud* cnf = nullptr;
    try {
        cnf = toCNF(nnf);
    } catch (...) {
        // freeTree(nnf);
        throw;
    }


    std::string out;
    toRPN(cnf, out);

    freeTree(cnf);

    return out;
}

int main() {
    std::vector<std::string> tests = {
        "AB&!",
        "AB|!",
        "AB|C&",
        "AB|C|D|",
        "AB&C&D&",
        "AB&!C!|",
        "AB|!C!&"
    };

    for (const auto& t : tests) {
        try {
            std::string res =  conjunctive_normal_form(t);
            std::cout << t << "  ->  " << res << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Erreur pour \"" << t << "\": " << e.what() << std::endl;
        }
    }

    try {
        std::cout << "A -> " <<  conjunctive_normal_form("A") << std::endl;
        std::cout << "A! -> " <<  conjunctive_normal_form("A!") << std::endl;
        std::cout << "AB^ -> " <<  conjunctive_normal_form("AB^") << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Erreur: " << e.what() << std::endl;
    }

    return 0;
}
