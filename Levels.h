#pragma once
#include <SFML/Graphics.hpp>

class Levels {
public:
    enum Difficulty {
        EASY = 0,
        MEDIUM = 1,
        HARD = 2
    };

    // Actions
    enum Action {
        NO_ACTION = -1,
        SELECT_EASY = 0,
        SELECT_MEDIUM = 1,
        SELECT_HARD = 2,
        BACK_TO_MENU = 3
    };

    Levels();
    void initialize(sf::Font& font);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    int getSelectedAction() const;
    void resetSelection();
    Difficulty getSelectedDifficulty() const;
    
    // Difficulty settings
    static int getEnemyCount(Difficulty difficulty);
    static float getPlayerSpeed(Difficulty difficulty);
    static float getEnemySpeed(Difficulty difficulty);
    static int getInitialPowerUps(Difficulty difficulty);

private:
    void updateVisualElements(const sf::Font& font);
    bool isMouseOverText(const sf::RenderWindow& window, int index) const;
    
    int selectedItem;
    static const int MAX_ITEMS = 4;
    sf::Text levelTexts[MAX_ITEMS];
    
    sf::Text titleText;
    sf::Text descriptionText;
    
    // Colors
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color backgroundColor;
};

