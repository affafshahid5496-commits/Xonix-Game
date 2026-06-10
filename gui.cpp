#include "gui.h"
#include <iostream>

bool GUI::initFont(sf::Font& font) {
    if (!font.loadFromFile("../arial.ttf")) {
        std::cout << "Warning: Could not load font. Using default." << std::endl;
        return false;
    }
    return true;
}

sf::Text GUI::createText(const sf::Font& font, const std::string& text, int size, 
                        sf::Color color, float x, float y) {
    sf::Text textElement;
    textElement.setFont(font);
    textElement.setString(text);
    textElement.setCharacterSize(size);
    textElement.setFillColor(color);
    textElement.setPosition(x, y);
    return textElement;
}

sf::RectangleShape GUI::createButton(float x, float y, float width, float height, 
                                   sf::Color color) {
    sf::RectangleShape button(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(color);
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::White);
    return button;
}

bool GUI::isMouseOver(const sf::RenderWindow& window, const sf::RectangleShape& button) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f buttonPos = button.getPosition();
    sf::Vector2f buttonSize = button.getSize();
    
    return (mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
            mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y);
}

sf::RectangleShape GUI::createInputField(float x, float y, float width, float height, 
                                       sf::Color color) {
    sf::RectangleShape field(sf::Vector2f(width, height));
    field.setPosition(x, y);
    field.setFillColor(color);
    field.setOutlineThickness(2);
    field.setOutlineColor(sf::Color::White);
    return field;
}