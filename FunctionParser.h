#pragma once

#include <string>
#include <memory>
#include "Function.h"

class FunctionParser {
public:
    std::shared_ptr<Function> parse(const std::string& expression);

private:
    std::shared_ptr<Function> parseExpression(const std::string& expr);
};
