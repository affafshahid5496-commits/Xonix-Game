#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "DArray.h"

class PauseMenu {
private:
    DArray<std::string> menuItems;
    int selectedItem;
    
    // Visual elements
    sf::Text titleText;
    sf::Text subtitleText;
    DArray<sf::Text> menuTexts;
    sf::RectangleShape overlay;
    
    // Colors
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color titleColor;
    
    // Font reference
    sf::Font* currentFont;
    
public:
    PauseMenu();
    
    void initialize(sf::Font& font);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    int getSelectedAction() const;
    void resetSelection();
    
    // Menu actions
    enum PauseAction {
        NO_ACTION = -1,
        RESUME_GAME = 0,
        SAVE_GAME = 1,
        QUIT_GAME = 2
    };
    
private:
    void updateVisualElements(const sf::Font& font);
    void createMenuItems();
};
