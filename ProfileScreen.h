#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class ProfileScreen {
private:
    Player* player;
    int selectedOption;
    
public:
    enum ProfileAction {
        NO_ACTION = -1,
        BACK = 0,
        EDIT_NICKNAME = 1,
        EDIT_EMAIL = 2,
        CHANGE_PASSWORD = 3
    };
    
    ProfileScreen();
    ProfileScreen(Player* p);
    
    void draw(sf::RenderWindow& window, sf::Font& font);
    void handleInput(sf::Event& event);
    int getSelectedAction() const;
    void resetSelection();
    void setPlayer(Player* p);
};
