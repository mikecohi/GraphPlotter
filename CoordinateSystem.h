#pragma once
#include <SFML/Graphics.hpp>

class CoordinateSystem
{
public:
    CoordinateSystem();

    void draw(sf::RenderWindow& window) const;
    sf::Vector2f toScreen(float x, float y) const;
    float getScale() const;

private:
    sf::Vector2f origin;
    float scale; // pixels per unit
};
