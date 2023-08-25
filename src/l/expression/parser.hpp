#pragma once

#include <string>
#include <stack>
#include <unordered_map>

#include "../utils/utils.hpp"
#include "../math/math.hpp"

template <typename T>
class LExpression {
    static_assert(std::is_floating_point_v<T>, "LExpression numerical type T is non-floating point.");
private:
    std::stack<char> operator_stack = std::stack<char>();
    std::stack<T> value_stack       = std::stack<T>();
public:
    void evaluate_top_operator();
    T evaluate();

    LExpression(const std::string& expr);
};

