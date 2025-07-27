#include "ExpressionTree.h"
#include "ExpressionParser.h"



ExpressionTree::ExpressionTree(const std::string& expression)
    : expr(expression) 
{
    ExpressionParser parser;
    root = parser.parse(expression); // convert expression into AST tree
}

float ExpressionTree::evaluate(float x) const {
    return root->evaluate(x); // using evaluate of AST tree
}

// Simple evaluator test
//float ExpressionTree::evaluate(float x) const {
//    return simpleEval(expr, x);
//}

//float ExpressionTree::simpleEval(const std::string& e, float x) const {
//    if (e == "x")
//        return x;
//    else if (e == "x^2")
//        return x * x;
//    else if (e == "x^3")
//        return x * x * x;
//    else if (e == "sin(x)")
//        return std::sin(x);
//    else if (e == "cos(x)")
//        return std::cos(x);
//    else if (e == "exp(x)")
//        return std::exp(x);
//    else if (e == "log(x)")
//        return std::log(x);
//    else
//        throw std::runtime_error("Unsupported expression: " + e);
//}
