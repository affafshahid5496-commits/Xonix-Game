#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class AuthSystem; // Forward declaration

class LoginScreen {
private:
    std::string currentScreen; // "main", "login", "register"
    std::string usernameInput;
    std::string passwordInput;
    std::string emailInput;
    std::string nicknameInput;
    std::string message;
    sf::Color messageColor;
    bool shouldStart;
    
    // Keyboard navigation
    int selectedOption; // Current menu option index
    bool inInputMode;   // True when editing text fields
    int selectedField;  // Current field being edited (0 = username, 1 = password, 2 = email, 3 = nickname)

public:
    LoginScreen();
    
    void handleEvent(sf::Event& event, AuthSystem& authSystem, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window, sf::Font& font);
    void reset();
    void setInitialScreen(const std::string& screen); // Set to "login" or "register"
    
    bool shouldStartGame() const;
    std::string getCurrentScreen() const;
    std::string getMessage() const;
    
private:
    void drawMainMenu(sf::RenderWindow& window, sf::Font& font);
    void drawLoginScreen(sf::RenderWindow& window, sf::Font& font);
    void drawRegisterScreen(sf::RenderWindow& window, sf::Font& font);
    void handleMainMenuEvent(sf::Event& event, AuthSystem& authSystem, sf::RenderWindow& window);
    void handleLoginEvent(sf::Event& event, AuthSystem& authSystem, sf::RenderWindow& window);
    void handleRegisterEvent(sf::Event& event, AuthSystem& authSystem, sf::RenderWindow& window);
};

