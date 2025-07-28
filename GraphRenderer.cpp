//GraphRenderer.cpp
#include "GraphRenderer.h"
#include <cmath>
#include <stdexcept>    // For std::runtime_error
#include <iomanip>      // For setting precision
#include <sstream>      // For std::ostringstream


// Constructor: initializes the scale and sets the default origin
GraphRenderer::GraphRenderer()
    : scale(50.0f), origin(0, 0) {}

// Zooms in or out by scaling the graph
void GraphRenderer::zoom(float factor) {
    scale *= factor;
}

void GraphRenderer::setFont(const sf::Font& f) {
    font = &f;
}

// Draws the graph of all user-defined functions and axes
void GraphRenderer::draw(sf::RenderWindow& window, const std::vector<UserDefinedFunction>& functions) {
    // Set the origin to the center of the window
    origin = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);

    // Draw grid before anything
    drawGrid(window, window.getSize());
    // Draw X and Y axes
    drawAxes(window);
    // Draw axis numbers
    if (font)
        drawAxisLabels(window, window.getSize());

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

// Draws a grid with lines spaced evenly across the screen
void GraphRenderer::drawGrid(sf::RenderWindow& window, const sf::Vector2u& size) {
    sf::Color gridColor(220, 220, 220, 120); // Light gray
    float labelStep = computeLabelStep();
    float pixelSpacing = scale * labelStep;

    int cols = size.x / pixelSpacing + 2;
    int rows = size.y / pixelSpacing + 2;

    sf::VertexArray lines(sf::Lines);

    // Vertical grid lines
    for (int i = -cols; i < cols; ++i) {
        float x = origin.x + i * pixelSpacing;
        lines.append(sf::Vertex(sf::Vector2f(x, 0), gridColor));
        lines.append(sf::Vertex(sf::Vector2f(x, size.y), gridColor));
    }

    // Horizontal grid lines
    for (int j = -rows; j < rows; ++j) {
        float y = origin.y + j * pixelSpacing;
        lines.append(sf::Vertex(sf::Vector2f(0, y), gridColor));
        lines.append(sf::Vertex(sf::Vector2f(size.x, y), gridColor));
    }

    window.draw(lines);
}

// Draws numeric labels on the X and Y axes
void GraphRenderer::drawAxisLabels(sf::RenderWindow& window, const sf::Vector2u& size) {
    if (!font) return; //avoid null pointer crash

    float labelStep = computeLabelStep();
    float pixelSpacing = scale * labelStep;

    int cols = size.x / pixelSpacing + 2;
    int rows = size.y / pixelSpacing + 2;

    sf::Text label;
    label.setFont(*font);
    label.setCharacterSize(12);
    label.setFillColor(sf::Color::Black);

    // Don't draw labels if zoom is too small
    if (pixelSpacing < 25.f)
        return;

    // X-axis labels
    for (int i = -cols; i < cols; ++i) {
        float value = i * labelStep;
        float x = origin.x + value * scale;
        if (std::abs(value) < 1e-3) continue;

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << value;
        label.setString(ss.str());

        label.setPosition(x + 2, origin.y + 4);
        window.draw(label);
    }

    // Y-axis labels
    for (int j = -rows; j < rows; ++j) {
        float value = -j * labelStep;
        float y = origin.y + value * scale;
        if (std::abs(value) < 1e-3) continue;

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << value;
        label.setString(ss.str());

        label.setPosition(origin.x + 4, y - 8);
        window.draw(label);
    }
}

// Calculates dynamic spacing between axis labels depending on zoom level
float GraphRenderer::computeLabelStep() const {
    // Minimum pixel spacing between labels to avoid overlapping
    const float minPixelGap = 50.f;

    // Convert min pixel gap to world units based on current scale
    float rawStep = minPixelGap / scale;

    // Round to nearest "nice" number: 0.1, 0.2, 0.5, 1, 2, 5, 10...
    float base = std::pow(10.f, std::floor(std::log10(rawStep)));
    float multipliers[] = { 1.f, 2.f, 5.f };

    for (float m : multipliers) {
        if (base * m >= rawStep)
            return base * m;
    }
    return base * 10.f;
}
