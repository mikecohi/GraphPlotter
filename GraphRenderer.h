//GraphRenderer.h
#pragma once

#include <SFML/Graphics.hpp>
#include "UserDefinedFunction.h"
#include <vector>

class GraphRenderer {
public:
    GraphRenderer();

    void draw(sf::RenderWindow& window, const std::vector<UserDefinedFunction>& functions);
    void zoom(float factor);

    // Optional: set font externally to draw labels
    void setFont(const sf::Font& font);

private:
    float scale;              // Zoom level (pixels per unit)
    sf::Vector2f origin;      // Origin point in screen coordinates
    float gridSpacing = 1.0f; // Grid spacing in world units
    float computeLabelStep() const; // Calculate space to draw axis number


    const sf::Font* font = nullptr; // Font for axis labels (can be null)

    sf::Vector2f worldToScreen(float x, float y, const sf::RenderWindow& window) const;
    
    void drawAxes(sf::RenderWindow& window);
    void drawGrid(sf::RenderWindow& window, const sf::Vector2u& size);
    void drawAxisLabels(sf::RenderWindow& window, const sf::Vector2u& size);
};
