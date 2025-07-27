#include "UserDefinedFunction.h"

UserDefinedFunction::UserDefinedFunction(std::shared_ptr<Function> f, sf::Color color)
    : func(f), drawColor(color) {}

float UserDefinedFunction::evaluate(float x) const {
    return func->evaluate(x);
}

sf::Color UserDefinedFunction::getColor() const {
    return drawColor;
}
