#include "Application.h"
#include <iostream>

Application::Application()
    : window(sf::VideoMode(800, 600), "Graph Plotter") {}

void Application::run() {
    // Example: read function
    std::cout << "Enter a function (e.g., sin(x) + x^2): ";
    std::string expr;
    std::getline(std::cin, expr);

    try {
        FunctionParser parser;
        auto tree = parser.parse(expr);
        functions.emplace_back(tree, sf::Color::Red);
    }
    catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << '\n';
    }

    while (window.isOpen()) {
        processInput();
        render();
    }
}

void Application::processInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        // Zoom with +/-
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Add)
                renderer.zoom(1.1f);
            else if (event.key.code == sf::Keyboard::Subtract)
                renderer.zoom(0.9f);
        }
    }
}

void Application::render() {
    window.clear(sf::Color::White);
    renderer.draw(window, functions);
    window.display();
}
