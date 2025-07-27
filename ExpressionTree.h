#pragma once

#include "Function.h"
#include <string>
#include <cmath>
#include <stdexcept>

class ExpressionTree : public Function {
public:
    ExpressionTree(const std::string& expression);

    float evaluate(float x) const override;

private:
    std::string expr;

    float simpleEval(const std::string& e, float x) const;
};
