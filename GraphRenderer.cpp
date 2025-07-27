#include "GraphRenderer.h"

GraphRenderer::GraphRenderer()
    : scale(50.0f), origin(0, 0) {}

void GraphRenderer::zoom(float factor) {
    scale *= factor;
}

void GraphRenderer::draw(sf::RenderWindow& window, const std::vector<UserDefinedFunction>& functions) {
    origin = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);

    drawAxes(window);

    for (const auto& func : functions) {
        sf::VertexArray curve(sf::LineStrip);

        for (float x = -10.f; x <= 10.f; x += 0.01f) {
            float y = func.evaluate(x);
            sf::Vector2f screen = worldToScreen(x, y, window);
            curve.append(sf::Vertex(screen, func.getColor()));
        }

        window.draw(curve);
    }
}

sf::Vector2f GraphRenderer::worldToScreen(float x, float y, const sf::RenderWindow& window) const {
    return sf::Vector2f(origin.x + x * scale, origin.y - y * scale);
}

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
