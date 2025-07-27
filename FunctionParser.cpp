#include "FunctionParser.h"
#include "ExpressionTree.h"
#include <stdexcept>

// This is a stub for phase 2; full parser comes in phase 3
std::shared_ptr<Function> FunctionParser::parse(const std::string& expression) {
    return parseExpression(expression);
}

std::shared_ptr<Function> FunctionParser::parseExpression(const std::string& expr) {
    // Phase 2: only accept expressions like sin(x), cos(x), x^2, x, x+2
    return std::make_shared<ExpressionTree>(expr);
}
