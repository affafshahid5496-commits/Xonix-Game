#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class GUI {
public:
    static bool initFont(sf::Font& font);
    static sf::Text createText(const sf::Font& font, const std::string& text, int size, 
                              sf::Color color, float x, float y);
    static sf::RectangleShape createButton(float x, float y, float width, float height, 
                                         sf::Color color);
    static bool isMouseOver(const sf::RenderWindow& window, const sf::RectangleShape& button);
    static sf::RectangleShape createInputField(float x, float y, float width, float height, 
                                             sf::Color color);
};

