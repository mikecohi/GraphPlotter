// ExpressionNode.cpp
#include "ExpressionNode.h"
#include <cmath>
#include <stdexcept>

// ConstantNode: holds a constant value
float ConstantNode::evaluate(float /*x*/) const {
    return value;
}

// VariableNode: returns the value of variable x
float VariableNode::evaluate(float x) const {
    return x;
}

// BinaryOpNode: evaluates binary operators +, -, *, /, ^
float BinaryOpNode::evaluate(float x) const {
    float leftVal = left->evaluate(x);
    float rightVal = right->evaluate(x);

    switch (op) {
    case '+': return leftVal + rightVal;
    case '-': return leftVal - rightVal;
    case '*': return leftVal * rightVal;
    case '/':
        if (rightVal == 0.0f) throw std::runtime_error("Division by zero");
        return leftVal / rightVal;
    case '^': return std::pow(leftVal, rightVal);
    default: throw std::runtime_error("Unknown binary operator");
    }
}

// UnaryFuncNode: evaluates functions like sin, cos, etc.
float UnaryFuncNode::evaluate(float x) const {
    float val = operand->evaluate(x);

    if (func == "sin") return std::sin(val);
    if (func == "cos") return std::cos(val);
    if (func == "tan") return std::tan(val);
    if (func == "log") {
        if (val <= 0.0f) throw std::runtime_error("log domain error");
        return std::log(val);
    }
    if (func == "exp") return std::exp(val);
    if (func == "sqrt") {
        if (val < 0.0f) throw std::runtime_error("sqrt domain error");
        return std::sqrt(val);
    }
    if (func == "abs") return std::fabs(val);

    throw std::runtime_error("Unknown function: " + func);
}
