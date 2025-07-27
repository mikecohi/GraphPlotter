#pragma once

#include "Function.h"
#include <SFML/Graphics.hpp>
#include <memory>

class UserDefinedFunction {
public:
    UserDefinedFunction(std::shared_ptr<Function> f, sf::Color color);

    float evaluate(float x) const;
    sf::Color getColor() const;

private:
    std::shared_ptr<Function> func;
    sf::Color drawColor;
};
