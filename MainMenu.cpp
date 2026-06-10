#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu() 
    : selectedItem(0), isLoggedIn(false), currentPlayer(nullptr), currentFont(nullptr),
      backgroundLoaded(false) {
    
    // Initialize colors
    normalColor = sf::Color(70, 130, 180);    // Steel blue
    selectedColor = sf::Color(255, 215, 0);   // Gold
    backgroundColor = sf::Color(25, 25, 112);  // Midnight blue
    boxColor = sf::Color(47, 79, 79, 200);    // Dark slate gray with transparency
    
    createMenuItems();
}

void MainMenu::createMenuItems() {
    // Menu items for logged-in users
    menuItems.append("START GAME");
    menuItems.append("MULTIPLAYER");
    menuItems.append("LEADERBOARD");
    menuItems.append("PROFILE");
    menuItems.append("FRIENDS");
    menuItems.append("THEMES");
    menuItems.append("SAVED GAMES");
    menuItems.append("MATCHMAKING");
    menuItems.append("LOGOUT");
    menuItems.append("EXIT");
    
    // Menu items for guests
    guestMenuItems.append("START GAME");
    guestMenuItems.append("LOGIN");
    guestMenuItems.append("REGISTER");
    guestMenuItems.append("EXIT");
}

void MainMenu::initialize(sf::Font& font) {
    currentFont = &font;
    
    // Create title
    titleText.setFont(font);
    titleText.setString("XONIX MAIN MENU");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(600 - titleText.getLocalBounds().width / 2, 50);
    
    // Create user info text
    userInfoText.setFont(font);
    userInfoText.setCharacterSize(18);
    userInfoText.setFillColor(sf::Color::Yellow);
    userInfoText.setPosition(250, 130);
    
    updateVisualElements(font);
}

void MainMenu::setPlayerStatus(bool loggedIn, Player* player) {
    isLoggedIn = loggedIn;
    currentPlayer = player;
    selectedItem = 0;
    updateUserInfo();
    
    // Update visual elements to show correct menu for logged-in/guest state
    if (currentFont) {
        updateVisualElements(*currentFont);
    }
}

void MainMenu::updateUserInfo() {
    if (isLoggedIn && currentPlayer) {
        userInfoText.setString("Welcome, " + currentPlayer->getUsername() + 
                              " | Score: " + std::to_string(currentPlayer->getTotalScore()));
    } else {
        userInfoText.setString("Playing as Guest");
    }
    
    // Center the user info text
    userInfoText.setPosition(600 - userInfoText.getLocalBounds().width / 2, 130);
}

void MainMenu::updateVisualElements(const sf::Font& font) {
    buttons.clear();
    menuTexts.clear();
    
    DArray<std::string> displayItems;
    
    if (isLoggedIn) {
        displayItems.append("START GAME");
        displayItems.append("MULTIPLAYER");
        displayItems.append("LEADERBOARD");
        displayItems.append("PROFILE");
        displayItems.append("FRIENDS");
        displayItems.append("THEMES");
        displayItems.append("SAVED GAMES");
        displayItems.append("MATCHMAKING");
        displayItems.append("LOGOUT");
        displayItems.append("EXIT");
    } else {
        for (int i = 0; i < guestMenuItems.size(); i++) {
            displayItems.append(guestMenuItems.get(i));
        }
    }
    
    int startY = 170;
    int spacing = 35;
    
    for (int i = 0; i < displayItems.size(); ++i) {
        // Create text for menu item
        sf::Text text;
        text.setFont(font);
        
        // Format: Add "> <" wrapper for selected item
        if (i == selectedItem) {
            text.setString("> " + displayItems.get(i) + " <");
            text.setCharacterSize(50);
            text.setFillColor(sf::Color::Magenta);
        } else {
            text.setString(displayItems.get(i));
            text.setCharacterSize(40);
            text.setFillColor(sf::Color::White);
        }
        
        // Center text horizontally on 1200px window
        sf::FloatRect textBounds = text.getLocalBounds();
        int xPos = 600 - textBounds.width / 2;
        int yPos = startY + (i * spacing);
        text.setPosition(xPos, yPos);
        
        menuTexts.append(text);
    }
}

void MainMenu::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseMoved) {
        // Update selection based on mouse position
        for (int i = 0; i < buttons.size(); ++i) {
            if (isMouseOverButton(window, i)) {
                selectedItem = i;
                if (menuTexts.size() > 0) updateVisualElements(*currentFont);
                break;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Check which button was clicked
            for (int i = 0; i < buttons.size(); ++i) {
                if (isMouseOverButton(window, i)) {
                    selectedItem = i;
                    break;
                }
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        const DArray<std::string>& items = isLoggedIn ? menuItems : guestMenuItems;
        
        if (event.key.code == sf::Keyboard::Up) {
            selectedItem = (selectedItem - 1 + items.size()) % items.size();
            if (menuTexts.size() > 0) updateVisualElements(*currentFont);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedItem = (selectedItem + 1) % items.size();
            if (menuTexts.size() > 0) updateVisualElements(*currentFont);
        }
        else if (event.key.code == sf::Keyboard::Return) {
            // Enter key - selection is handled by getSelectedAction
        }
    }
}

bool MainMenu::isMouseOverButton(const sf::RenderWindow& window, int index) const {
    if (index < 0 || index >= buttons.size()) return false;
    
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f buttonPos = buttons[index].getPosition();
    sf::Vector2f buttonSize = buttons[index].getSize();
    
    return (mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
            mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y);
}

void MainMenu::draw(sf::RenderWindow& window) {
    // Draw background image first (so it appears behind menu items)
    if (backgroundLoaded) {
        window.draw(backgroundSprite);
    }
    
    // Draw title
    window.draw(titleText);
    
    // Draw user info
    window.draw(userInfoText);
    
    // Draw menu items (text-only)
    for (int i = 0; i < menuTexts.size(); ++i) {
        window.draw(menuTexts.get(i));
    }
    
    // Draw navigation hints
    if (menuTexts.size() > 0 && currentFont) {
        sf::Text instructions;
        instructions.setFont(*currentFont);
        instructions.setString("UP/DOWN: Navigate | ENTER: Select | ESC: Back");
        instructions.setCharacterSize(14);
        instructions.setFillColor(sf::Color::Green);
        instructions.setPosition(600 - instructions.getLocalBounds().width / 2, 580);
        window.draw(instructions);
    }
}

int MainMenu::getSelectedAction() const {
    if (!isLoggedIn) {
        // Guest menu mapping: START GAME, LOGIN, REGISTER, EXIT
        switch (selectedItem) {
            case 0: return START_GAME;      // Start Game
            case 1: return LOGIN;           // Login - now correctly returns LOGIN
            case 2: return REGISTER;        // Register - now correctly returns REGISTER
            case 3: return EXIT;            // Exit
            default: return NO_ACTION;
        }
    } else {
        // Logged-in user menu mapping: START GAME, MULTIPLAYER, LEADERBOARD, PROFILE, FRIENDS, THEMES, SAVED GAMES, MATCHMAKING, LOGOUT, EXIT
        switch (selectedItem) {
            case 0: return START_GAME;      // Start Game
            case 1: return MULTIPLAYER;     // Multiplayer
            case 2: return LEADERBOARD;     // Leaderboard
            case 3: return PROFILE;         // Profile
            case 4: return FRIENDS;         // Friends
            case 5: return THEMES;          // Themes
            case 6: return SAVED_GAMES;     // Saved Games
            case 7: return MATCHMAKING;     // Matchmaking
            case 8: return LOGOUT;          // Logout
            case 9: return EXIT;            // Exit
            default: return NO_ACTION;
        }
    }
}

void MainMenu::resetSelection() {
    selectedItem = 0;
    if (menuTexts.size() > 0 && currentFont) updateVisualElements(*currentFont);
}

void MainMenu::setBackgroundImage(const std::string& imagePath) {
    if (backgroundTexture.loadFromFile(imagePath)) {
        backgroundLoaded = true;
        backgroundImagePath = imagePath;
        
        // Create sprite from texture
        backgroundSprite.setTexture(backgroundTexture);
        
        // Scale sprite to fit window (1200x630)
        sf::Vector2f windowSize(1200.0f, 630.0f);
        sf::Vector2f textureSize(static_cast<float>(backgroundTexture.getSize().x),
                                 static_cast<float>(backgroundTexture.getSize().y));
        
        float scaleX = windowSize.x / textureSize.x;
        float scaleY = windowSize.y / textureSize.y;
        
        // Use the larger scale to cover entire window (may crop some edges)
        float scale = std::max(scaleX, scaleY);
        backgroundSprite.setScale(scale, scale);
        
        // Center the sprite in the window
        sf::FloatRect spriteBounds = backgroundSprite.getLocalBounds();
        backgroundSprite.setPosition(
            (windowSize.x - spriteBounds.width * scale) / 2.0f,
            (windowSize.y - spriteBounds.height * scale) / 2.0f
        );
    } else {
        backgroundLoaded = false;
        std::cerr << "Failed to load background image from: " << imagePath << std::endl;
    }
}