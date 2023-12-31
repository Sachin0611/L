#include <string>

#define L_PRIVATE_INCLUDE
#include "parser.hpp"
#include "../utils/utils.hpp"
#include "../math/math.hpp"
#include "tables.hpp"

L_PRIVATE inline bool is_function(std::string x) {
    for (auto function: functions)
        if (function.second == x) return true;

    return false;
}

L_PRIVATE inline bool is_operator(char op) {
    return precedence.contains(op);
}

template <typename T>
L_PRIVATE inline bool is_constant(std::string op) {
    return constants<T>.contains(op);
}

template <typename T>
LExpression<T>::LExpression(const std::string& expr) {
    bool last_one_was_paren = false;
    size_t index = 0;

    while (index < expr.length()) {
        char token = expr[index];

        if (std::isspace(token)) {
            index++; continue;
        }

        if (std::isalpha(token)) {
            std::string buffer;

            while (std::isalpha(expr[index]))
                buffer += expr[index++];

            if (is_function(buffer)) {
                operator_stack.push(buffer[0]);
                continue;
            }

            else if (is_constant<T>(buffer)) {
                value_stack.push(constants<T>.at(buffer));
                continue;
            }
            
            else
                throw std::runtime_error("Invalid function: " + buffer);
        }

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
            throw std::runtime_error("Invalid charachter in expression: '" + std::string(1, token) + "'");
    }
}

template class LExpression<f32>;
template class LExpression<f64>;
#if (__SIZEOF_LONG_DOUBLE__ != __SIZEOF_DOUBLE__) || defined(QUADMATH_USED)
template class LExpression<bigf>;
#endif