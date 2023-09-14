#include <stdexcept>
#include <cmath>
#define L_PRIVATE_INCLUDE
#include "parser.hpp"
#include "../utils/utils.hpp"
#include "../math/math.hpp"
#include "tables.hpp"

template <typename T>
void LExpression<T>::evaluate_top_operator() {
    char op = operator_stack.top();
    operator_stack.pop();

    if (std::isalpha(op)) {
        if (value_stack.size() < 1)
            throw std::runtime_error("Not enough arguments supplied for: " + functions.at(op));

        T a = value_stack.top(); value_stack.pop();
        value_stack.push(mathematical_function_functions<T>.at(op)(a));

        return;
    }
    
    if (op == '-' && value_stack.size() >= 1) {
        T operand = value_stack.top(); value_stack.pop();
        value_stack.push(-operand);
        return;
    }
    
    if (value_stack.size() < 2)
        throw std::runtime_error("Not enough arguments supplied for: " + std::string(1, op));
    
    T b = value_stack.top();
    value_stack.pop();
    T a = value_stack.top();
    value_stack.pop();
    
    T result = operator_functions<T>.at(op)(a, b);
    value_stack.push(result);
}

template <typename T>
T LExpression<T>::evaluate() {
    while (!operator_stack.empty())
        evaluate_top_operator();

    while (!value_stack.empty()) {
        if (value_stack.size() == 1)
            break;

        value_stack.pop();
    }

    if (value_stack.size() != 1)
        throw std::runtime_error("Invalid expression");

    return value_stack.top();
}

template class LExpression<f32>;
template class LExpression<f64>;
#if (__SIZEOF_LONG_DOUBLE__ != __SIZEOF_DOUBLE__) || defined(QUADMATH_USED)
template class LExpression<bigf>;
#endif
