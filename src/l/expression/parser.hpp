#pragma once

#include <string>
#include <stack>
#include <unordered_map>

#include "../utils/utils.hpp"
#include "../math/math.hpp"

#if !defined(L_INCLUDED) && !defined(L_PRIVATE_INCLUDE)
#error L header file included directly, Include l/l.hpp. 
#endif // !defined(L_INCLUDED) && !defined(L_PRIVATE_INCLUDE)

template <typename T>
class LExpression {
    static_assert(std::is_floating_point_v<T>, "LExpression numerical type T is non-floating point.");
private:
    std::stack<char> operator_stack = std::stack<char>();
    std::stack<T>    value_stack    = std::stack<T>();
public:
    void evaluate_top_operator();
    T evaluate();

    LExpression(const std::string& expr);
};

