#pragma once
#include <string>
#include <memory>
#include <stack>
#include <queue>
#include <sstream>
#include <map>
#include "ExpressionNode.h"

class ExpressionParser {
public:
    std::unique_ptr<ExpressionNode> parse(const std::string& expression);

private:
    int getPrecedence(const std::string& op);
    bool isLeftAssociative(const std::string& op);
    bool isOperator(const std::string& token);
    bool isFunction(const std::string& token);
    bool isNumber(const std::string& token);
    std::vector<std::string> tokenize(const std::string& expr);
    std::vector<std::string> toRPN(const std::vector<std::string>& tokens);
    std::unique_ptr<ExpressionNode> buildAST(const std::vector<std::string>& rpn);
};
