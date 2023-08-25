#include <l/expression/expression.hpp>
#include <iostream>

#include "utils.hpp"

int main(int argc, char** argv) {
    UNUSED(argc) UNUSED(argv)

    LExpression<bigf> expr = LExpression<bigf>("-((1 + 2)(2 + 3)^2)");

    std::cout << "Eval: " << expr.evaluate() << "\n";
    return 0;
}