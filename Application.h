#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GraphRenderer.h"
#include "FunctionParser.h"
#include "UserDefinedFunction.h"

class Application {
public:
    Application();
    void run();

private:
    sf::RenderWindow window;
    GraphRenderer renderer;
    sf::Font font;

    std::vector<UserDefinedFunction> functions;

    void processInput();
    void render();
};
