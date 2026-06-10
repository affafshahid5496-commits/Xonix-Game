#include "ProfileScreen.h"
#include <iostream>

ProfileScreen::ProfileScreen() 
    : player(nullptr), selectedOption(0) {}

ProfileScreen::ProfileScreen(Player* p) 
    : player(p), selectedOption(0) {}

void ProfileScreen::setPlayer(Player* p) {
    player = p;
    selectedOption = 0;
}

void ProfileScreen::draw(sf::RenderWindow& window, sf::Font& font) {
    if (!player) {
        sf::Text noPlayer("No player selected", font, 32);
        noPlayer.setFillColor(sf::Color::Red);
        noPlayer.setPosition(400, 300);
        window.draw(noPlayer);
        return;
    }
    
    // Draw title
    sf::Text title("PLAYER PROFILE", font, 48);
    title.setFillColor(sf::Color::Cyan);
    title.setStyle(sf::Text::Bold);
    float titleX = (window.getSize().x - title.getLocalBounds().width) / 2;
    title.setPosition(titleX, 20);
    window.draw(title);
    
    // Draw separator line
    sf::RectangleShape separator(sf::Vector2f(window.getSize().x - 40, 2));
    separator.setFillColor(sf::Color::Cyan);
    separator.setPosition(20, 85);
    window.draw(separator);
    
    // Profile section - Left side
    int leftX = 60;
    int topY = 120;
    int lineHeight = 35;
    
    // Username
    sf::Text usernameLabel("Username:", font, 26);
    usernameLabel.setFillColor(sf::Color::Yellow);
    usernameLabel.setPosition(leftX, topY);
    window.draw(usernameLabel);
    
    sf::Text usernameValue(player->getUsername(), font, 26);
    usernameValue.setFillColor(sf::Color::White);
    usernameValue.setPosition(leftX + 200, topY);
    window.draw(usernameValue);
    
    // Nickname
    topY += lineHeight;
    sf::Text nicknameLabel("Nickname:", font, 26);
    nicknameLabel.setFillColor(sf::Color::Yellow);
    nicknameLabel.setPosition(leftX, topY);
    window.draw(nicknameLabel);
    
    sf::Text nicknameValue(player->getNickname(), font, 26);
    nicknameValue.setFillColor(sf::Color::White);
    nicknameValue.setPosition(leftX + 200, topY);
    window.draw(nicknameValue);
    
    // Email
    topY += lineHeight;
    sf::Text emailLabel("Email:", font, 26);
    emailLabel.setFillColor(sf::Color::Yellow);
    emailLabel.setPosition(leftX, topY);
    window.draw(emailLabel);
    
    std::string emailDisplay = player->getEmail().empty() ? "Not set" : player->getEmail();
    sf::Text emailValue(emailDisplay, font, 26);
    emailValue.setFillColor(sf::Color::White);
    emailValue.setPosition(leftX + 200, topY);
    window.draw(emailValue);
    
    // Player ID
    topY += lineHeight;
    sf::Text idLabel("Player ID:", font, 26);
    idLabel.setFillColor(sf::Color::Yellow);
    idLabel.setPosition(leftX, topY);
    window.draw(idLabel);
    
    sf::Text idValue(std::to_string(player->getPlayerId()), font, 26);
    idValue.setFillColor(sf::Color::White);
    idValue.setPosition(leftX + 200, topY);
    window.draw(idValue);
    
    // Statistics section - Right side
    int rightX = 650;
    topY = 120;
    
    // Total Score
    sf::Text totalScoreLabel("Total Score:", font, 26);
    totalScoreLabel.setFillColor(sf::Color::Magenta);
    totalScoreLabel.setPosition(rightX, topY);
    window.draw(totalScoreLabel);
    
    sf::Text totalScoreValue(std::to_string(player->getTotalScore()), font, 26);
    totalScoreValue.setFillColor(sf::Color::Cyan);
    totalScoreValue.setPosition(rightX + 220, topY);
    window.draw(totalScoreValue);
    
    // Highest Score
    topY += lineHeight;
    sf::Text highestLabel("Highest Score:", font, 26);
    highestLabel.setFillColor(sf::Color::Magenta);
    highestLabel.setPosition(rightX, topY);
    window.draw(highestLabel);
    
    sf::Text highestValue(std::to_string(player->getHighestScore()), font, 26);
    highestValue.setFillColor(sf::Color::Cyan);
    highestValue.setPosition(rightX + 220, topY);
    window.draw(highestValue);
    
    // Games Played
    topY += lineHeight;
    sf::Text gamesLabel("Games Played:", font, 26);
    gamesLabel.setFillColor(sf::Color::Magenta);
    gamesLabel.setPosition(rightX, topY);
    window.draw(gamesLabel);
    
    sf::Text gamesValue(std::to_string(player->getGamesPlayed()), font, 26);
    gamesValue.setFillColor(sf::Color::Cyan);
    gamesValue.setPosition(rightX + 220, topY);
    window.draw(gamesValue);
    
    // Games Won
    topY += lineHeight;
    sf::Text winsLabel("Games Won:", font, 26);
    winsLabel.setFillColor(sf::Color::Magenta);
    winsLabel.setPosition(rightX, topY);
    window.draw(winsLabel);
    
    sf::Text winsValue(std::to_string(player->getGamesWon()), font, 26);
    winsValue.setFillColor(sf::Color::Cyan);
    winsValue.setPosition(rightX + 220, topY);
    window.draw(winsValue);
    
    // Win Rate
    topY += lineHeight;
    sf::Text winRateLabel("Win Rate:", font, 26);
    winRateLabel.setFillColor(sf::Color::Magenta);
    winRateLabel.setPosition(rightX, topY);
    window.draw(winRateLabel);
    
    std::string winRateStr = std::to_string((int)player->getWinRate()) + "%";
    sf::Text winRateValue(winRateStr, font, 26);
    winRateValue.setFillColor(sf::Color::Cyan);
    winRateValue.setPosition(rightX + 220, topY);
    window.draw(winRateValue);
    
    // Current Theme
    topY += lineHeight;
    sf::Text themeLabel("Current Theme ID:", font, 26);
    themeLabel.setFillColor(sf::Color::Magenta);
    themeLabel.setPosition(rightX, topY);
    window.draw(themeLabel);
    
    sf::Text themeValue(std::to_string(player->getCurrentThemeId()), font, 26);
    themeValue.setFillColor(sf::Color::Cyan);
    themeValue.setPosition(rightX + 220, topY);
    window.draw(themeValue);
    
    // Bottom section - Actions
    int bottomY = 480;
    
    sf::RectangleShape actionBox(sf::Vector2f(window.getSize().x - 40, 120));
    actionBox.setFillColor(sf::Color(30, 30, 60, 200));
    actionBox.setOutlineThickness(2);
    actionBox.setOutlineColor(sf::Color::Cyan);
    actionBox.setPosition(20, bottomY);
    window.draw(actionBox);
    
    // Action header
    sf::Text actionsHeader("OPTIONS:", font, 24);
    actionsHeader.setFillColor(sf::Color::Yellow);
    actionsHeader.setPosition(40, bottomY + 5);
    window.draw(actionsHeader);
    
    // Action options
    topY = bottomY + 35;
    std::string actions[] = {
        "[1] Edit Nickname",
        "[2] Edit Email",
        "[3] Change Password",
        "[ESC] Back"
    };
    
    for (int i = 0; i < 4; i++) {
        sf::Text action(actions[i], font, 20);
        // Highlight selected option with different color
        if (i == selectedOption && i < 3) {
            action.setFillColor(sf::Color::Magenta);
            action.setStyle(sf::Text::Bold);
        } else if (i == 3) {
            action.setFillColor(sf::Color::Red);
        } else {
            action.setFillColor(sf::Color::White);
        }
        action.setPosition(40 + (i % 2) * 300, topY + (i / 2) * 30);
        window.draw(action);
        
        // Draw selection indicator
        if (i == selectedOption && i < 3) {
            sf::Text indicator(">>", font, 20);
            indicator.setFillColor(sf::Color::Yellow);
            indicator.setPosition(20, topY + (i / 2) * 30);
            window.draw(indicator);
        }
    }
    
    // Instructions
    sf::Text instructions("Use UP/DOWN to navigate | ENTER to select | ESC to go back", font, 16);
    instructions.setFillColor(sf::Color::Green);
    instructions.setPosition(600 - instructions.getLocalBounds().width / 2, 610);
    window.draw(instructions);
}

void ProfileScreen::handleInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) {
            // Move left between options (from right to left)
            if (selectedOption % 2 == 1) {
                selectedOption--; // Move from position 1/3 to 0/2
            }
        } else if (event.key.code == sf::Keyboard::Right) {
            // Move right between options (from left to right)
            if (selectedOption % 2 == 0 && selectedOption < 2) {
                selectedOption++; // Move from position 0/2 to 1/3
            }
        } else if (event.key.code == sf::Keyboard::Up) {
            // Move up (from bottom row to top row)
            if (selectedOption > 1) {
                selectedOption -= 2;
            }
        } else if (event.key.code == sf::Keyboard::Down) {
            // Move down (from top row to bottom row)
            if (selectedOption < 2) {
                selectedOption += 2;
            }
        } else if (event.key.code == sf::Keyboard::Num1) {
            selectedOption = 0; // Edit Nickname
        } else if (event.key.code == sf::Keyboard::Num2) {
            selectedOption = 1; // Edit Email
        } else if (event.key.code == sf::Keyboard::Num3) {
            selectedOption = 2; // Change Password
        }
    }
}

int ProfileScreen::getSelectedAction() const {
    return selectedOption;
}

void ProfileScreen::resetSelection() {
    selectedOption = 0;
}
