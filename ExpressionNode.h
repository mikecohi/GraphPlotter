// ExpressionNode.h
#pragma once
#include <memory>
#include <string>

class ExpressionNode {
public:
    virtual ~ExpressionNode() = default;
    virtual float evaluate(float x) const = 0;
};

class ConstantNode : public ExpressionNode {
    float value;
public:
    ConstantNode(float val) : value(val) {}
    float evaluate(float x) const override;
};

class VariableNode : public ExpressionNode {
public:
    float evaluate(float x) const override;
};

class BinaryOpNode : public ExpressionNode {
    char op;
    std::unique_ptr<ExpressionNode> left, right;
public:
    BinaryOpNode(char o, std::unique_ptr<ExpressionNode> l, std::unique_ptr<ExpressionNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
    float evaluate(float x) const override;
};

class UnaryFuncNode : public ExpressionNode {
    std::string func;
    std::unique_ptr<ExpressionNode> operand;
public:
    UnaryFuncNode(const std::string& f, std::unique_ptr<ExpressionNode> op)
        : func(f), operand(std::move(op)) {}
    float evaluate(float x) const override;
};
