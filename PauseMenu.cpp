#include "PauseMenu.h"
#include <iostream>

PauseMenu::PauseMenu()
    : selectedItem(0),
      normalColor(sf::Color::White),
      selectedColor(sf::Color::Yellow),
      titleColor(sf::Color::Cyan),
      currentFont(nullptr) {
    createMenuItems();
}

void PauseMenu::createMenuItems() {
    menuItems.clear();
    menuItems.append("Resume Game");
    menuItems.append("Save Game");
    menuItems.append("Quit Game");
}

void PauseMenu::initialize(sf::Font& font) {
    currentFont = &font;
    
    // Create title
    titleText.setFont(font);
    titleText.setString("GAME PAUSED");
    titleText.setCharacterSize(60);
    titleText.setFillColor(titleColor);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(600 - titleText.getLocalBounds().width / 2, 100);
    
    // Create subtitle
    subtitleText.setFont(font);
    subtitleText.setString("Press ENTER to select or UP/DOWN to navigate");
    subtitleText.setCharacterSize(16);
    subtitleText.setFillColor(sf::Color::Green);
    subtitleText.setPosition(600 - subtitleText.getLocalBounds().width / 2, 180);
    
    // Create semi-transparent overlay
    overlay.setSize(sf::Vector2f(1200, 630));
    overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
    overlay.setPosition(0, 0);
    
    updateVisualElements(font);
}

void PauseMenu::updateVisualElements(const sf::Font& font) {
    menuTexts.clear();
    
    int startY = 280;
    int spacing = 60;
    
    for (int i = 0; i < menuItems.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(menuItems.get(i));
        text.setCharacterSize(32);
        text.setFillColor(i == selectedItem ? selectedColor : normalColor);
        
        int textX = 600 - text.getLocalBounds().width / 2;
        text.setPosition(textX, startY + (i * spacing));
        
        menuTexts.append(text);
    }
}

void PauseMenu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedItem = (selectedItem - 1 + menuItems.size()) % menuItems.size();
            if (currentFont) {
                updateVisualElements(*currentFont);
            }
        } else if (event.key.code == sf::Keyboard::Down) {
            selectedItem = (selectedItem + 1) % menuItems.size();
            if (currentFont) {
                updateVisualElements(*currentFont);
            }
        }
    }
}

void PauseMenu::draw(sf::RenderWindow& window) {
    // Draw overlay
    window.draw(overlay);
    
    // Draw title and subtitle
    window.draw(titleText);
    window.draw(subtitleText);
    
    // Draw menu items
    for (int i = 0; i < menuTexts.size(); ++i) {
        window.draw(menuTexts.get(i));
    }
}

int PauseMenu::getSelectedAction() const {
    return selectedItem;
}

void PauseMenu::resetSelection() {
    selectedItem = 0;
}
