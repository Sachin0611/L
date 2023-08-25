#include <string>

#include "parser.hpp"
#include "../utils/utils.hpp"
#include "../math/math.hpp"

static const std::unordered_map<char, i8> precedence = {
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2},
    {'^', 3},
};

template<typename T>
static const std::unordered_map<char, T(*)(T, T)> operator_functions {
    {'+', &LMath::add<T>},
    {'-', &LMath::sub<T>}, 
    {'*', &LMath::mul<T>},
    {'/', &LMath::div<T>},
    {'^', &LMath::pow<T>},
};

L_PRIVATE inline bool is_operator(char op) {
    return precedence.contains(op);
}

template <typename T>
LExpression<T>::LExpression(const std::string& expr) {
    bool last_one_was_paren = false;
    size_t index = 0;

    while (index < expr.length()) {
        if (std::isspace(expr[index])) {
            index++; continue;
        }

        char token = expr[index];

        if (std::isdigit(token) || (token == '-' && (index == 0 || !std::isdigit(expr[index - 1])))) {
            if (expr.size() > index + 1 && is_operator(expr[index + 1]))
                throw std::runtime_error("Two operators can't be chained together");
            
            // There's probably a smarter solution to this, but eh.
            // Basically, if I encounter a `-` before a parentheses block,
            // I go over the expression, pushing charachters to a buffer
            // before encountering the ending `)`. After that, I make
            // an another LExpression, evaluate it, and then append
            // sign flipped value to value_stack
            
            if (expr.size() > index + 1 && expr[index + 1] == '(') {
                std::string substr; index += 1;

                bool last_rp = false;

                int nop = 0;

                while (true) {
                    printf("index: %zu | nop: %d | expr[index]: %c\n", index, nop, expr[index]);
                    if (expr[index] == '(')
                        nop++;

                    else if (expr[index] == ')') {
                        nop--;

                        if (nop == 0) {
                            substr += expr[index];
                            last_rp = true;
                            break;
                        }
                    }

                    if (index >= expr.size())
                        break;

                    substr += expr[index++];
                }

                if (last_rp == false)
                    throw std::runtime_error("Dangling '('");

                value_stack.push(-(LExpression<T>(substr)).evaluate());

                last_one_was_paren = true;

                continue;
            }
            
            T value = LMath::floating_from_string<T>(expr.substr(index));

            value_stack.push(value);
            while (index < expr.length() && (std::isdigit(expr[index]) || expr[index] == '.'))
                index++;
        } 
        
        else if (is_operator(token)) {
            if (expr.size() > index + 1 && is_operator(expr[index + 1]))
                throw std::runtime_error("Two operators can't be chained together");
            
            while (!operator_stack.empty() && is_operator(operator_stack.top()) && 
                   precedence.at(operator_stack.top()) >= precedence.at(token))
                evaluate_top_operator();

            operator_stack.push(token);
            index++;
        }
        
        else if (token == '(') {
            operator_stack.push(token);
            index++;
        } 
        
        else if (token == ')') {
            while (!operator_stack.empty()) {
                if (operator_stack.top() == '(') {
                    operator_stack.pop(); break;
                }
                evaluate_top_operator();
            }

            if ((expr.size() > index + 1 && expr[index + 1] == '(') || 
                last_one_was_paren) {
                while (!operator_stack.empty() && is_operator(operator_stack.top()) && 
                   precedence.at(operator_stack.top()) >= 2)
                    evaluate_top_operator();

                operator_stack.push('*');

                last_one_was_paren = false;
            }
            
            index++;
        }
        
        else
            throw std::runtime_error("Invalid charachter in expression");
    }
}

template <typename T>
void LExpression<T>::evaluate_top_operator() {
    char op = operator_stack.top();
    operator_stack.pop();

    printf("op: %c\n", op);
    
    if (op == '-' && value_stack.size() >= 1) {
        T operand = value_stack.top();
        value_stack.pop();
        value_stack.push(-operand); // Applying the negative sign
        return;
    }
    
    if (value_stack.size() < 2) {
        // Handle error, insufficient operands
        return;
    }
    
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

    // if (value_stack.size() != 1)
    //     throw std::runtime_error("Invalid expression");

    return value_stack.top();
}

template class LExpression<f32>;
template class LExpression<f64>;
#if (__SIZEOF_LONG_DOUBLE__ != __SIZEOF_DOUBLE__) || defined(QUADMATH_USED)
template class LExpression<bigf>;
#endif