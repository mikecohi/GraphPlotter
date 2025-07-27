#pragma once

#include <SFML/Graphics.hpp>
#include "UserDefinedFunction.h"
#include <vector>

class GraphRenderer {
public:
    GraphRenderer();

    void draw(sf::RenderWindow& window, const std::vector<UserDefinedFunction>& functions);
    void zoom(float factor);

private:
    float scale;
    sf::Vector2f origin;

    sf::Vector2f worldToScreen(float x, float y, const sf::RenderWindow& window) const;
    void drawAxes(sf::RenderWindow& window);
};
