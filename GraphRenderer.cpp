#include "GraphRenderer.h"
#include <cmath>
#include <stdexcept>  // For std::runtime_error

// Constructor: initializes the scale and sets the default origin
GraphRenderer::GraphRenderer()
    : scale(50.0f), origin(0, 0) {}

// Zooms in or out by scaling the graph
void GraphRenderer::zoom(float factor) {
    scale *= factor;
}

// Draws the graph of all user-defined functions and axes
void GraphRenderer::draw(sf::RenderWindow& window, const std::vector<UserDefinedFunction>& functions) {
    // Set the origin to the center of the window
    origin = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);

    // Draw X and Y axes
    drawAxes(window);

    // Loop over each function and draw its curve
    for (const auto& func : functions) {
        sf::VertexArray curve(sf::LineStrip);  // Line strip for continuous curve
        bool lastValid = false;                // Whether the previous point was valid

        // Sample the function in range [-10, 10] with a step of 0.01
        for (float x = -10.f; x <= 10.f; x += 0.01f) {
            float y;

            // Evaluate function at x and catch any math error (like log(-1), sqrt(-2), etc.)
            try {
                y = func.evaluate(x);

                // Skip point if it's NaN or Inf
                if (std::isnan(y) || std::isinf(y)) {
                    lastValid = false;
                    continue;
                }
            }
            catch (const std::exception& e) {
                // Skip point if evaluation throws an exception
                lastValid = false;
                continue;
            }

            // Convert world coordinates to screen coordinates
            sf::Vector2f screen = worldToScreen(x, y, window);

            if (!lastValid && curve.getVertexCount() > 1) {
                // If previous segment was broken, draw current curve so far
                window.draw(curve);
                curve.clear();
            }

            // Append current valid point to the curve
            curve.append(sf::Vertex(screen, func.getColor()));
            lastValid = true;
        }

        // Draw remaining part of the curve (if any)
        if (curve.getVertexCount() > 1) {
            window.draw(curve);
        }
    }
}

// Converts mathematical (world) coordinates to pixel (screen) coordinates
sf::Vector2f GraphRenderer::worldToScreen(float x, float y, const sf::RenderWindow& window) const {
    return sf::Vector2f(origin.x + x * scale, origin.y - y * scale);
}

// Draws the X and Y axes in black
void GraphRenderer::drawAxes(sf::RenderWindow& window) {
    sf::Vertex xAxis[] = {
        sf::Vertex(sf::Vector2f(0, origin.y), sf::Color::Black),
        sf::Vertex(sf::Vector2f(window.getSize().x, origin.y), sf::Color::Black)
    };
    sf::Vertex yAxis[] = {
        sf::Vertex(sf::Vector2f(origin.x, 0), sf::Color::Black),
        sf::Vertex(sf::Vector2f(origin.x, window.getSize().y), sf::Color::Black)
    };

    window.draw(xAxis, 2, sf::Lines);
    window.draw(yAxis, 2, sf::Lines);
}
