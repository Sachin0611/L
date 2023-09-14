#include <cmath>//added #1
#include "l/l.hpp"
#include <iostream>//זה כבר היה פה
int main(int argc, char** argv) {
    UNUSED(argc) UNUSED(argv)

    LExpression<bigf> expr = LExpression<bigf>("pi");

    std::cout << "Eval: " << expr.evaluate() << "\n";
    return 0;
}
