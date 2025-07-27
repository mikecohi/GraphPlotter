#pragma once

#include "Function.h"
#include <string>
#include <cmath>
#include <stdexcept>
#include "ExpressionNode.h"


class ExpressionTree : public Function {
public:
    ExpressionTree(const std::string& expression);
    float evaluate(float x) const;

    //float evaluate(float x) const override;

private:
    std::string expr;
    std::unique_ptr<ExpressionNode> root;

    //float simpleEval(const std::string& e, float x) const;
};
