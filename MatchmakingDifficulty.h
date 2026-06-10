#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class MatchmakingDifficulty {
private:
    sf::Text titleText;
    sf::Text difficultyTexts[3];  // EASY, MEDIUM, HARD
    int selectedDifficulty;
    sf::Font* currentFont;

public:
    enum Difficulty {
        EASY = 0,
        MEDIUM = 1,
        HARD = 2,
        BACK_TO_MENU = 3
    };

    MatchmakingDifficulty();
    
    void initialize(sf::Font& font);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    
    int getSelectedDifficulty() const { return selectedDifficulty; }
    void resetSelection();
    
    // Action helpers
    enum MenuAction {
        SELECT_EASY = 0,
        SELECT_MEDIUM = 1,
        SELECT_HARD = 2,
        BACK_TO_MATCHMAKING = 3,
        NO_ACTION = -1
    };
    
    int getSelectedAction() const;
    
private:
    void updateVisualElements();
};
