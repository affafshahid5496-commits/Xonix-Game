#include "SavedGamesScreen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

SavedGamesScreen::SavedGamesScreen()
    : selectedItem(0), currentFont(nullptr) {
    
    // Initialize colors
    normalColor = sf::Color(70, 130, 180);     // Steel blue
    selectedColor = sf::Color(255, 215, 0);    // Gold
    backgroundColor = sf::Color(25, 25, 112);  // Midnight blue
    boxColor = sf::Color(30, 30, 90);          // Dark blue
}

std::string SavedGamesScreen::getRecordsFilePath(const std::string& username) const {
    return "saves/" + username + "_records.txt";
}

void SavedGamesScreen::initialize(sf::Font& font, const std::string& username) {
    currentFont = &font;
    recordsFile = getRecordsFilePath(username);
    loadSavedGamesFromFile();
    createVisualElements(font);
}

void SavedGamesScreen::loadSavedGamesFromFile() {
    savedGames.clear();
    std::ifstream file(recordsFile);
    
    if (!file.is_open()) {
        std::cout << "No saved games records found for this user" << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Parse: username|score|difficulty|timestamp|filename
        std::stringstream ss(line);
        std::string username, score, difficulty, timestamp, filename;
        
        if (std::getline(ss, username, '|') &&
            std::getline(ss, score, '|') &&
            std::getline(ss, difficulty, '|') &&
            std::getline(ss, timestamp, '|') &&
            std::getline(ss, filename, '|')) {
            
            SavedGameRecord record;
            record.username = username;
            record.score = score;
            record.difficulty = difficulty;
            record.timestamp = timestamp;
            record.filename = filename;
            
            savedGames.append(record);
        }
    }
    
    file.close();
}

void SavedGamesScreen::createVisualElements(const sf::Font& font) {
    buttons.clear();
    gameTexts.clear();
    
    // Background
    background.setSize(sf::Vector2f(1200, 800));
    background.setFillColor(backgroundColor);
    background.setPosition(0, 0);
    
    // Menu box
    menuBox.setSize(sf::Vector2f(900, 600));
    menuBox.setFillColor(boxColor);
    menuBox.setPosition(150, 100);
    
    // Title
    titleText.setFont(font);
    titleText.setString("SAVED GAMES");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Cyan);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(600 - titleBounds.width / 2, 120);
    
    if (savedGames.isEmpty()) {
        // Show "No saved games" message
        noGamesText.setFont(font);
        noGamesText.setString("No saved games found");
        noGamesText.setCharacterSize(40);
        noGamesText.setFillColor(sf::Color::White);
        sf::FloatRect bounds = noGamesText.getLocalBounds();
        noGamesText.setPosition(600 - bounds.width / 2, 350);
    } else {
        // Display saved games
        int startY = 220;
        int spacing = 50;
        
        for (size_t i = 0; i < savedGames.size(); ++i) {
            sf::Text text;
            text.setFont(font);
            
            // Format: "Score: XXX | Difficulty: MEDIUM | Time: YYYY-MM-DD HH:MM"
            std::string displayStr = "Score: " + savedGames[i].score + 
                                   " | Difficulty: " + savedGames[i].difficulty + 
                                   " | " + savedGames[i].timestamp;
            
            if (i == selectedItem) {
                text.setString("> " + displayStr + " <");
                text.setCharacterSize(30);
                text.setFillColor(selectedColor);
            } else {
                text.setString(displayStr);
                text.setCharacterSize(28);
                text.setFillColor(sf::Color::White);
            }
            
            sf::FloatRect textBounds = text.getLocalBounds();
            int xPos = 600 - textBounds.width / 2;
            int yPos = startY + (i * spacing);
            text.setPosition(xPos, yPos);
            
            gameTexts.append(text);
            
            // Create button rectangle
            sf::RectangleShape button(sf::Vector2f(700, 40));
            button.setPosition(250, yPos - 5);
            button.setFillColor(sf::Color::Transparent);
            buttons.append(button);
        }
    }
}

void SavedGamesScreen::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (savedGames.isEmpty()) return;
    
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedItem = (selectedItem - 1 + savedGames.size()) % savedGames.size();
            if (currentFont) createVisualElements(*currentFont);
        } else if (event.key.code == sf::Keyboard::Down) {
            selectedItem = (selectedItem + 1) % savedGames.size();
            if (currentFont) createVisualElements(*currentFont);
        }
    } else if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        bool foundHover = false;
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                if (selectedItem != (int)i) {
                    selectedItem = i;
                    if (currentFont) createVisualElements(*currentFont);
                }
                foundHover = true;
                break;
            }
        }
    } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            for (size_t i = 0; i < buttons.size(); ++i) {
                if (buttons[i].getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                    selectedItem = i;
                    break;
                }
            }
        }
    }
}

void SavedGamesScreen::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(menuBox);
    window.draw(titleText);
    
    if (savedGames.isEmpty()) {
        window.draw(noGamesText);
    } else {
        for (int i = 0; i < gameTexts.size(); ++i) {
            window.draw(gameTexts.get(i));
        }
    }
}

SavedGameRecord* SavedGamesScreen::getSelectedGame() {
    if (selectedItem >= 0 && selectedItem < savedGames.size()) {
        return &savedGames.at(selectedItem);
    }
    return nullptr;
}

void SavedGamesScreen::resetSelection() {
    selectedItem = 0;
    if (currentFont) createVisualElements(*currentFont);
}
