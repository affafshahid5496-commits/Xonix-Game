
#pragma once
#include <SFML/Graphics.hpp>
#include "theme_manager.h"
#include "Player.h"

class ThemesScreen {
private:
    ThemeManager* manager;
    Player* currentPlayer;
    int selected;

public:
    ThemesScreen();
    ThemesScreen(ThemeManager* m, Player* p);
    void draw(sf::RenderWindow& window, sf::Font& font);
    void handleInput(sf::Event& event);
};