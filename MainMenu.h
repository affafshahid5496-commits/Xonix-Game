#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "DArray.h"
#include "authentication.h"

class MainMenu {
private:
    DArray<std::string> menuItems;
    DArray<std::string> guestMenuItems;
    int selectedItem;
    bool isLoggedIn;
    Player* currentPlayer;
    
    // Visual elements
    sf::RectangleShape background;
    sf::RectangleShape menuBox;
    DArray<sf::RectangleShape> buttons;
    DArray<sf::Text> menuTexts;
    sf::Text titleText;
    sf::Text userInfoText;
    
    // Background image
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::string backgroundImagePath;
    bool backgroundLoaded;
    
    // Colors
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color backgroundColor;
    sf::Color boxColor;
    
    // Font reference
    sf::Font* currentFont;
    
public:
    MainMenu();
    
    void initialize(sf::Font& font);
    void setPlayerStatus(bool loggedIn, Player* player = nullptr);
    void setBackgroundImage(const std::string& imagePath);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    int getSelectedAction() const;
    void resetSelection();
    
    // Menu actions
    enum MenuAction {
        NO_ACTION = -1,
        START_GAME = 0,
        LOGIN = 1,
        REGISTER = 2,
        MULTIPLAYER = 3,
        LEADERBOARD = 4,
        PROFILE = 5,
        FRIENDS = 6,
        THEMES = 7,
        SAVED_GAMES = 8,
        MATCHMAKING = 9,
        LOGOUT = 10,
        EXIT = 11
    };

private:
    void createMenuItems();
    void updateVisualElements(const sf::Font& font);
    bool isMouseOverButton(const sf::RenderWindow& window, int index) const;
    void updateUserInfo();
};

