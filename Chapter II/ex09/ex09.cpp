#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <bits/stdc++.h>
#include <string>
#include <stack>
#include <unordered_set>
#include <stdexcept>
#include <utility>

std::unordered_set<int> apply_union(const std::unordered_set<int>& a, const std::unordered_set<int>& b) {
    std::unordered_set<int> r = a;
    for (int x : b) r.insert(x);
    return r;
}
std::unordered_set<int> apply_intersection(const std::unordered_set<int>& a, const std::unordered_set<int>& b) {
    std::unordered_set<int> r;
    if (a.size() <= b.size()) {
        for (int x : a) if (b.count(x)) r.insert(x);
    } else {
        for (int x : b) if (a.count(x)) r.insert(x);
    }
    return r;
}
std::unordered_set<int> apply_symdiff(const std::unordered_set<int>& a, const std::unordered_set<int>& b) {
    std::unordered_set<int> r;
    for (int x : a) if (!b.count(x)) r.insert(x);
    for (int x : b) if (!a.count(x)) r.insert(x);
    return r;
}
std::unordered_set<int> apply_complement(const std::unordered_set<int>& a, const std::unordered_set<int>& universe) {
    std::unordered_set<int> r;
    for (int x : universe) if (!a.count(x)) r.insert(x);
    return r;
}
std::unordered_set<int> apply_impl(const std::unordered_set<int>& A, const std::unordered_set<int>& B, const std::unordered_set<int>& universe) {
    std::unordered_set<int> notA = apply_complement(A, universe);
    return apply_union(notA, B);
}
std::unordered_set<int> apply_equiv(const std::unordered_set<int>& A, const std::unordered_set<int>& B, const std::unordered_set<int>& universe) {
    std::unordered_set<int> impl1 = apply_impl(A, B, universe);
    std::unordered_set<int> impl2 = apply_impl(B, A, universe);
    return apply_intersection(impl1, impl2);
}

std::vector<int> eval_set(const std::string& formula, const std::vector<std::vector<int>>& sets) {
    std::unordered_set<int> universe;
    for (const auto& v : sets) for (int x : v) universe.insert(x);

    std::size_t n_sets = sets.size();
    std::stack<std::unordered_set<int>> st;

    for (char c : formula) {
        if (c >= 'A' && c <= 'Z') {
            std::size_t idx = static_cast<std::size_t>(c - 'A');
            if (idx >= n_sets) throw std::runtime_error("Not enough sets provided for variables in formula");
            std::unordered_set<int> s;
            for (int x : sets[idx]) s.insert(x);
            st.push(std::move(s));
        } else if (c == '!') {
            if (st.empty()) throw std::runtime_error("Malformed formula: not enough operands for '!'");
            std::unordered_set<int> a = std::move(st.top()); st.pop();
            st.push(apply_complement(a, universe));
        } else {
            if (st.size() < 2) throw std::runtime_error(std::string("Malformed formula: not enough operands for operator '") + c + "'");
            std::unordered_set<int> right = std::move(st.top()); st.pop();
            std::unordered_set<int> left  = std::move(st.top()); st.pop();
            std::unordered_set<int> res;
            switch (c) {
                case '&': res = apply_intersection(left, right); break;
                case '|': res = apply_union(left, right); break;
                case '^': res = apply_symdiff(left, right); break;
                case '>': res = apply_impl(left, right, universe); break;
                case '=': res = apply_equiv(left, right, universe); break;
                default:
                    throw std::runtime_error(std::string("Unknown operator: ") + c);
            }
            st.push(std::move(res));
        }
    }

    if (st.size() != 1) throw std::runtime_error("Malformed formula: final stack size != 1");
    std::unordered_set<int> result = std::move(st.top());

    std::vector<int> out;
    out.reserve(result.size());
    for (int x : result) out.push_back(x);
    std::sort(out.begin(), out.end());
    return out;
}

void print_vec(const std::vector<int>& v) {
    std::cout << "{";
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ", ";
        std::cout << v[i];
    }
    std::cout << "}";
}

int main() {
    try {
        const std::vector<std::vector<int>> sets1 = {{0,1,2}, {0,3,4}};
        std::vector<int> r1 = eval_set("AB&", sets1);
        print_vec(r1); std::cout << std::endl;

        const std::vector<std::vector<int>> sets2 = {{0,1,2}, {3,4,5}};
        std::vector<int> r2 = eval_set("AB|", sets2);
        print_vec(r2); std::cout << std::endl;

        const std::vector<std::vector<int>> sets3 = {{0,1,2},{4}};
        std::vector<int> r3 = eval_set("A!B&", sets3);
        print_vec(r3); std::cout << std::endl;

        const std::vector<std::vector<int>> sets4 = {{0,1,2}, {1,2,3}};
        std::vector<int> r4 = eval_set("AB^", sets4);
        print_vec(r4); std::cout << std::endl;

        const std::vector<std::vector<int>> sets5 = {{1,2}, {1,3}, {2, 4}};
        std::vector<int> r5 = eval_set("AB>", sets5);
        print_vec(r5); std::cout << std::endl;

        const std::vector<std::vector<int>> sets6 = {{0,2}, {0,2}, {3}};
        std::vector<int> r6 = eval_set("AB=", sets6);
        print_vec(r6); std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 2;
    }
    return 0;
}


