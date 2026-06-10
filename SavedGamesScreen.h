#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "DArray.h"
#include "SaveGame.h"

struct SavedGameRecord {
    std::string username;
    std::string score;
    std::string difficulty;
    std::string timestamp;
    std::string filename;  // Save file name
};

class SavedGamesScreen {
private:
    DArray<SavedGameRecord> savedGames;
    int selectedItem;
    
    // Visual elements
    sf::RectangleShape background;
    sf::RectangleShape menuBox;
    DArray<sf::RectangleShape> buttons;
    DArray<sf::Text> gameTexts;
    sf::Text titleText;
    sf::Text noGamesText;
    
    // Colors
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color backgroundColor;
    sf::Color boxColor;
    
    // Font reference
    sf::Font* currentFont;
    
    // Records file
    std::string recordsFile;
    
public:
    SavedGamesScreen();
    
    void initialize(sf::Font& font, const std::string& username);
    void loadSavedGamesFromFile();
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    
    SavedGameRecord* getSelectedGame();
    int getSelectedIndex() const { return selectedItem; }
    void resetSelection();
    bool hasSavedGames() const { return !savedGames.isEmpty(); }
    
private:
    void createVisualElements(const sf::Font& font);
    bool isMouseOverButton(const sf::RenderWindow& window, int index) const;
    std::string getRecordsFilePath(const std::string& username) const;
};
