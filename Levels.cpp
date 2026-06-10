#include "Levels.h"
#include <iostream>

Levels::Levels() 
    : selectedItem(0) {
    
    // Initialize colors
    normalColor = sf::Color(200, 200, 200);    // Light gray
    selectedColor = sf::Color(255, 215, 0);    // Gold
    backgroundColor = sf::Color(25, 25, 112);  // Midnight blue
}

void Levels::initialize(sf::Font& font) {
    // Create title
    titleText.setFont(font);
    titleText.setString("SELECT DIFFICULTY");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(600 - titleText.getLocalBounds().width / 2, 80);
    
    // Create description
    descriptionText.setFont(font);
    descriptionText.setCharacterSize(16);
    descriptionText.setFillColor(sf::Color::Yellow);
    descriptionText.setPosition(300, 150);
    
    updateVisualElements(const_cast<const sf::Font&>(font));
}

void Levels::updateVisualElements(const sf::Font& font) {
    // Menu items
    const char* levelItems[MAX_ITEMS] = {
        "EASY",
        "MEDIUM", 
        "HARD",
        "BACK TO MAIN MENU"
    };
    
    int startY = 200;
    int spacing = 50;
    
    for (int i = 0; i < MAX_ITEMS; ++i) {
        levelTexts[i].setFont(font);
        
        if (i == selectedItem) {
            std::string selectedText = "> ";
            selectedText += levelItems[i];
            selectedText += " <";
            levelTexts[i].setString(selectedText);
            levelTexts[i].setCharacterSize(36);
            levelTexts[i].setFillColor(selectedColor);
        } else {
            levelTexts[i].setString(levelItems[i]);
            levelTexts[i].setCharacterSize(30);
            levelTexts[i].setFillColor(normalColor);
        }
        
        sf::FloatRect textBounds = levelTexts[i].getLocalBounds();
        int xPos = 600 - textBounds.width / 2;
        int yPos = startY + (i * spacing);
        levelTexts[i].setPosition(xPos, yPos);
    }
    
    // Update description based on selection
    std::string description;
    switch (selectedItem) {
        case 0: // EASY
            description = "• 2 Enemies\n• Normal Player Speed\n• Slower Enemies\n• 2 Starting Power-ups\n• Perfect for beginners!";
            break;
        case 1: // MEDIUM
            description = "• 4 Enemies\n• Normal Player Speed\n• Normal Enemy Speed\n• 1 Starting Power-up\n• Balanced challenge";
            break;
        case 2: // HARD
            description = "• 6 Enemies\n• Faster Player Speed\n• Faster Enemies\n• No Starting Power-ups\n• For expert players!";
            break;
        case 3: // BACK
            description = "Return to main menu";
            break;
        default:
            description = "Select a difficulty level";
    }
    descriptionText.setString(description);
}

void Levels::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved) {
        // Update selection based on mouse position
        for (int i = 0; i < MAX_ITEMS; ++i) {
            if (isMouseOverText(window, i)) {
                selectedItem = i;
                updateVisualElements(*const_cast<const sf::Font*>(levelTexts[0].getFont()));
                break;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            for (int i = 0; i < MAX_ITEMS; ++i) {
                if (isMouseOverText(window, i)) {
                    selectedItem = i;
                    break;
                }
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedItem = (selectedItem - 1 + MAX_ITEMS) % MAX_ITEMS;
            updateVisualElements(*const_cast<const sf::Font*>(levelTexts[0].getFont()));
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedItem = (selectedItem + 1) % MAX_ITEMS;
            updateVisualElements(*const_cast<const sf::Font*>(levelTexts[0].getFont()));
        }
        else if (event.key.code == sf::Keyboard::Return) {
            // Enter key - selection is handled by getSelectedAction
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            selectedItem = MAX_ITEMS - 1; // Back option
        }
    }
}

bool Levels::isMouseOverText(const sf::RenderWindow& window, int index) const {
    if (index < 0 || index >= MAX_ITEMS) return false;
    
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    const sf::Text& text = levelTexts[index];
    sf::FloatRect textBounds = text.getGlobalBounds();
    
    return textBounds.contains(static_cast<sf::Vector2f>(mousePos));
}

void Levels::draw(sf::RenderWindow& window) {
    // Draw title
    window.draw(titleText);
    
    // Draw description
    window.draw(descriptionText);
    
    // Draw level items
    for (int i = 0; i < MAX_ITEMS; ++i) {
        window.draw(levelTexts[i]);
    }
    
    // Draw navigation hints
    if (MAX_ITEMS > 0) {
        sf::Font* font = (sf::Font*)levelTexts[0].getFont();
        if (font) {
            sf::Text instructions;
            instructions.setFont(*font);
            instructions.setString("UP/DOWN: Navigate | ENTER: Select | ESC: Back");
            instructions.setCharacterSize(14);
            instructions.setFillColor(sf::Color::Green);
            instructions.setPosition(600 - instructions.getLocalBounds().width / 2, 580);
            window.draw(instructions);
        }
    }
}

int Levels::getSelectedAction() const {
    switch (selectedItem) {
        case 0: return SELECT_EASY;      // Easy
        case 1: return SELECT_MEDIUM;    // Medium  
        case 2: return SELECT_HARD;      // Hard
        case 3: return BACK_TO_MENU;     // Back to menu
        default: return NO_ACTION;
    }
}

void Levels::resetSelection() {
    selectedItem = 0;
    if (MAX_ITEMS > 0) {
        updateVisualElements(*const_cast<const sf::Font*>(levelTexts[0].getFont()));
    }
}

Levels::Difficulty Levels::getSelectedDifficulty() const {
    switch (selectedItem) {
        case 0: return EASY;
        case 1: return MEDIUM;
        case 2: return HARD;
        default: return EASY; // Default to easy if somehow invalid
    }
}

// Difficulty settings implementation
int Levels::getEnemyCount(Difficulty difficulty) {
    switch (difficulty) {
        case EASY: return 2;
        case MEDIUM: return 4;
        case HARD: return 6;
        default: return 4;
    }
}

float Levels::getPlayerSpeed(Difficulty difficulty) {
    switch (difficulty) {
        case EASY: return 0.07f;   // Normal speed
        case MEDIUM: return 0.07f; // Normal speed
        case HARD: return 0.05f;   // Faster speed
        default: return 0.07f;
    }
}

float Levels::getEnemySpeed(Difficulty difficulty) {
    switch (difficulty) {
        case EASY: return 3.0f;    // Slower enemies
        case MEDIUM: return 4.0f;  // Normal speed
        case HARD: return 5.0f;    // Faster enemies
        default: return 4.0f;
    }
}

int Levels::getInitialPowerUps(Difficulty difficulty) {
    switch (difficulty) {
        case EASY: return 2;
        case MEDIUM: return 1;
        case HARD: return 0;
        default: return 1;
    }
}