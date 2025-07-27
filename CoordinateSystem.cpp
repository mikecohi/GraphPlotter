#include "CoordinateSystem.h"

CoordinateSystem::CoordinateSystem()
{
    origin = sf::Vector2f(400, 300); // Center of window
    scale = 30.0f; // 1 unit = 30 pixels
}

// Draw the X and Y axis
void CoordinateSystem::draw(sf::RenderWindow& window) const
{
    sf::Vertex xAxis[] =
    {
        sf::Vertex(sf::Vector2f(0, origin.y), sf::Color::Black),
        sf::Vertex(sf::Vector2f(800, origin.y), sf::Color::Black)
    };

    sf::Vertex yAxis[] =
    {
        sf::Vertex(sf::Vector2f(origin.x, 0), sf::Color::Black),
        sf::Vertex(sf::Vector2f(origin.x, 600), sf::Color::Black)
    };

    window.draw(xAxis, 2, sf::Lines);
    window.draw(yAxis, 2, sf::Lines);
}

// Convert mathematical coordinate to screen coordinate
sf::Vector2f CoordinateSystem::toScreen(float x, float y) const
{
    return sf::Vector2f(origin.x + x * scale, origin.y - y * scale);
}

float CoordinateSystem::getScale() const
{
    return scale;
}
