#include "LoginScreen.h"
#include "authentication.h"
#include "gui.h"
#include "DArray.h"
#include <iostream>

LoginScreen::LoginScreen() {
    currentScreen = "main";
    usernameInput = "";
    passwordInput = "";
    emailInput = "";
    nicknameInput = "";
    message = "";
    messageColor = sf::Color::White;
    shouldStart = false;
    selectedOption = 0;
    inInputMode = false;
    selectedField = 0;
}

void LoginScreen::handleEvent(sf::Event& event, AuthSystem& authSystem, sf::RenderWindow& window) {
    if (event.type == sf::Event::TextEntered) {
        // Only accept text input when in input mode on login/register screens
        if (inInputMode && (currentScreen == "login" || currentScreen == "register")) {
            if (event.text.unicode == '\b') { // Backspace
                if (selectedField == 0 && !usernameInput.empty()) {
                    usernameInput.pop_back();
                } else if (selectedField == 1 && !passwordInput.empty()) {
                    passwordInput.pop_back();
                } else if (selectedField == 2 && !emailInput.empty()) {
                    emailInput.pop_back();
                } else if (selectedField == 3 && !nicknameInput.empty()) {
                    nicknameInput.pop_back();
                }
            } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                // Only accept printable ASCII characters (exclude newline, tab, etc.)
                char c = static_cast<char>(event.text.unicode);
                if (selectedField == 0) {
                    usernameInput += c;
                } else if (selectedField == 1) {
                    passwordInput += c;
                } else if (selectedField == 2) {
                    emailInput += c;
                } else if (selectedField == 3) {
                    nicknameInput += c;
                }
            }
        }
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            if (currentScreen != "main") {
                currentScreen = "main";
                selectedOption = 0;
                inInputMode = false;
                reset();
            }
        } else if (event.key.code == sf::Keyboard::Up) {
            if (inInputMode && selectedField > 0) {
                selectedField--;
            } else if (!inInputMode && selectedOption > 0) {
                selectedOption--;
            }
        } else if (event.key.code == sf::Keyboard::Down) {
            if (inInputMode) {
                int maxFields = (currentScreen == "register") ? 4 : 2;
                if (selectedField < maxFields - 1) {
                    selectedField++;
                }
            } else {
                int maxOptions = 0;
                if (currentScreen == "main") maxOptions = 4;
                else if (currentScreen == "login") maxOptions = 3;
                else if (currentScreen == "register") maxOptions = 2;
                
                if (selectedOption < maxOptions - 1) {
                    selectedOption++;
                }
            }
        } else if (event.key.code == sf::Keyboard::Return) {
            if (currentScreen == "main") {
                if (selectedOption == 0) { // Login
                    currentScreen = "login";
                    selectedOption = 0;
                    inInputMode = true;
                    selectedField = 0;
                    reset();
                } else if (selectedOption == 1) { // Register
                    currentScreen = "register";
                    selectedOption = 0;
                    inInputMode = true;
                    selectedField = 0;
                    reset();
                } else if (selectedOption == 2) { // Exit
                    window.close();
                } else if (selectedOption == 3) { // Play as Guest
                    shouldStart = true;
                }
            } else if (currentScreen == "login") {
                if (inInputMode && selectedOption == 0) { // Submit login from input mode
                    if (!usernameInput.empty() && !passwordInput.empty()) {
                        if (authSystem.login(usernameInput, passwordInput)) {
                            message = "Login successful!";
                            messageColor = sf::Color::Green;
                            shouldStart = true;
                        } else {
                            message = "Login failed!";
                            messageColor = sf::Color::Red;
                        }
                    }
                } else if (!inInputMode) {
                    if (selectedOption == 0) { // Login button (when not in input mode)
                        if (!usernameInput.empty() && !passwordInput.empty()) {
                            if (authSystem.login(usernameInput, passwordInput)) {
                                message = "Login successful!";
                                messageColor = sf::Color::Green;
                                shouldStart = true;
                            } else {
                                message = "Login failed!";
                                messageColor = sf::Color::Red;
                            }
                        }
                    } else if (selectedOption == 1) { // Back button
                        currentScreen = "main";
                        selectedOption = 0;
                        inInputMode = false;
                        reset();
                    }
                }
            } else if (currentScreen == "register") {
                if (inInputMode && selectedOption == 0) { // Submit from input mode
                    if (!usernameInput.empty() && !passwordInput.empty()) {
                        if (authSystem.registerPlayer(usernameInput, passwordInput, nicknameInput, emailInput)) {
                            message = "Registration successful!";
                            messageColor = sf::Color::Green;
                            currentScreen = "login";
                            selectedOption = 0;
                            inInputMode = true;
                            selectedField = 0;
                            passwordInput = "";
                        } else {
                            message = "Registration failed!";
                            messageColor = sf::Color::Red;
                        }
                    }
                } else if (!inInputMode) {
                    if (selectedOption == 0) { // Register button
                        if (!usernameInput.empty() && !passwordInput.empty()) {
                            if (authSystem.registerPlayer(usernameInput, passwordInput, nicknameInput, emailInput)) {
                                message = "Registration successful!";
                                messageColor = sf::Color::Green;
                                currentScreen = "login";
                                selectedOption = 0;
                                inInputMode = true;
                                selectedField = 0;
                                passwordInput = "";
                            } else {
                                message = "Registration failed!";
                                messageColor = sf::Color::Red;
                            }
                        }
                    } else if (selectedOption == 1) { // Back button
                        currentScreen = "main";
                        selectedOption = 0;
                        inInputMode = false;
                        reset();
                    }
                }
            }
        }
    }
}

void LoginScreen::draw(sf::RenderWindow& window, sf::Font& font) {
    if (currentScreen == "main") {
        drawMainMenu(window, font);
    } else if (currentScreen == "login") {
        drawLoginScreen(window, font);
    } else if (currentScreen == "register") {
        drawRegisterScreen(window, font);
    }
}

void LoginScreen::drawMainMenu(sf::RenderWindow& window, sf::Font& font) {
    int windowWidth = 1200;
    int windowHeight = 630;
    int spacing = 85;
    int startY = 100;
    
    // Title
    sf::Text title = GUI::createText(font, "XONIX GAME", 80, sf::Color::Cyan, (windowWidth - 350) / 2, 20);
    window.draw(title);
    
    // Options: Login, Register, Exit, Play as Guest
    DArray<std::string> options;
    options.append("Login");
    options.append("Register");
    options.append("Exit");
    options.append("Play as Guest");
    
    for (int i = 0; i < options.size(); i++) {
        int yPos = startY + (i * spacing);
        
        // Selected option: large text in Yellow with wrapper
        if (selectedOption == i) {
            std::string displayText = "> " + options.get(i) + " <";
            sf::Text optionText = GUI::createText(font, displayText, 50, sf::Color::Yellow, (windowWidth - 300) / 2, yPos);
            window.draw(optionText);
        } else {
            // Unselected: normal size in White
            sf::Text optionText = GUI::createText(font, options.get(i), 40, sf::Color::White, (windowWidth - 250) / 2, yPos);
            window.draw(optionText);
        }
    }
    
   
    
    // Message
    if (!message.empty()) {
        sf::Text msgText = GUI::createText(font, message, 18, messageColor, (windowWidth - 300) / 2, windowHeight - 60);
        window.draw(msgText);
    }
}

void LoginScreen::drawLoginScreen(sf::RenderWindow& window, sf::Font& font) {
    int windowWidth = 1200;
    int windowHeight = 630;
    
    // Title
    sf::Text title = GUI::createText(font, "LOGIN", 60, sf::Color::Cyan, (windowWidth - 200) / 2, 20);
    window.draw(title);
    
    if (inInputMode) {
        // Input mode - show large text fields
        DArray<std::string> labels;
        labels.append("Username:");
        labels.append("Password:");
        int startY = 120;
        int fieldSpacing = 100;
        
        for (int i = 0; i < 2; i++) {
            int yPos = startY + (i * fieldSpacing);
            
            // Label
            sf::Text label = GUI::createText(font, labels.get(i), 28, sf::Color::White, 150, yPos);
            window.draw(label);
            
            // Input display - highlight if selected
            std::string displayText = (i == 0) ? usernameInput : std::string(passwordInput.size(), '*');
            sf::Color textColor = (selectedField == i) ? sf::Color::Yellow : sf::Color::White;
            int fontSize = (selectedField == i) ? 45 : 35;
            std::string wrappedText = (selectedField == i) ? ("> " + displayText + " <") : displayText;
            
            sf::Text text = GUI::createText(font, wrappedText, fontSize, textColor, 200, yPos + 35);
            window.draw(text);
        }
        
        // Show submit hint
        sf::Text hint = GUI::createText(font, "UP/DOWN: Switch Fields | ENTER: Submit | ESC: Back", 13, sf::Color::Green, 130, windowHeight - 50);
        window.draw(hint);
    } else {
        // Display mode - show input values and navigation options
        int displayStartY = 120;
        
        // Show current input
        sf::Text userLabel = GUI::createText(font, "Username:", 24, sf::Color::White, 150, displayStartY);
        sf::Text userText = GUI::createText(font, usernameInput, 32, sf::Color::Cyan, 200, displayStartY + 35);
        window.draw(userLabel);
        window.draw(userText);
        
        sf::Text passLabel = GUI::createText(font, "Password:", 24, sf::Color::White, 150, displayStartY + 100);
        std::string hiddenPass(passwordInput.size(), '*');
        sf::Text passText = GUI::createText(font, hiddenPass, 32, sf::Color::Cyan, 200, displayStartY + 135);
        window.draw(passLabel);
        window.draw(passText);
        
        // Navigation options - Large text
        DArray<std::string> options;
        options.append("Login");
        options.append("Back");
        int navStartY = 310;
        int navSpacing = 60;
        
        for (int i = 0; i < 2; i++) {
            int yPos = navStartY + (i * navSpacing);
            
            if (selectedOption == i) {
                std::string displayText = "> " + options.get(i) + " <";
                sf::Text optionText = GUI::createText(font, displayText, 45, sf::Color::Yellow, (windowWidth - 250) / 2, yPos);
                window.draw(optionText);
            } else {
                sf::Text optionText = GUI::createText(font, options.get(i), 32, sf::Color::White, (windowWidth - 200) / 2, yPos);
                window.draw(optionText);
            }
        }
        
        sf::Text hint = GUI::createText(font, "UP/DOWN: Navigate | ENTER: Submit | ESC: Back", 13, sf::Color::Green, 140, windowHeight - 50);
        window.draw(hint);
    }
    
    // Message
    if (!message.empty()) {
        sf::Text msgText = GUI::createText(font, message, 16, messageColor, (windowWidth - 300) / 2, windowHeight - 30);
        window.draw(msgText);
    }
}

void LoginScreen::drawRegisterScreen(sf::RenderWindow& window, sf::Font& font) {
    int windowWidth = 1200;
    int windowHeight = 630;
    
    // Title
    sf::Text title = GUI::createText(font, "REGISTER", 60, sf::Color::Cyan, (windowWidth - 250) / 2, 15);
    window.draw(title);
    
    if (inInputMode) {
        // Input mode - show large text fields for all 4 fields
        DArray<std::string> labels;
        labels.append("Username:");
        labels.append("Password:");
        labels.append("Email:");
        labels.append("Nickname:");
        int startY = 95;
        int fieldSpacing = 75;
        
        for (int i = 0; i < 4; i++) {
            int yPos = startY + (i * fieldSpacing);
            
            // Label
            sf::Text label = GUI::createText(font, labels.get(i), 20, sf::Color::White, 140, yPos);
            window.draw(label);
            
            // Input display - highlight if selected
            std::string displayText;
            if (i == 0) displayText = usernameInput;
            else if (i == 1) displayText = std::string(passwordInput.size(), '*');
            else if (i == 2) displayText = emailInput;
            else displayText = nicknameInput;
            
            sf::Color textColor = (selectedField == i) ? sf::Color::Yellow : sf::Color::White;
            int fontSize = (selectedField == i) ? 32 : 26;
            std::string wrappedText = (selectedField == i) ? ("> " + displayText + " <") : displayText;
            
            sf::Text text = GUI::createText(font, wrappedText, fontSize, textColor, 280, yPos + 15);
            window.draw(text);
        }
        
        
    } else {
        // Display mode - show input values and navigation options
        int displayStartY = 80;
        int displaySpacing = 75;
        DArray<std::string> displayLabels;
        displayLabels.append("Username:");
        displayLabels.append("Password:");
        displayLabels.append("Email:");
        displayLabels.append("Nickname:");
        
        for (int i = 0; i < 4; i++) {
            int yPos = displayStartY + (i * displaySpacing);
            
            sf::Text label = GUI::createText(font, displayLabels.get(i), 18, sf::Color::White, 140, yPos);
            window.draw(label);
            
            std::string displayText;
            if (i == 0) displayText = usernameInput;
            else if (i == 1) displayText = std::string(passwordInput.size(), '*');
            else if (i == 2) displayText = emailInput;
            else displayText = nicknameInput;
            
            sf::Text text = GUI::createText(font, displayText, 22, sf::Color::Cyan, 280, yPos + 15);
            window.draw(text);
        }
        
        // Navigation options - Large text
        DArray<std::string> options;
        options.append("Register");
        options.append("Back");
        int navStartY = 380;
        int navSpacing = 50;
        
        for (int i = 0; i < 2; i++) {
            int yPos = navStartY + (i * navSpacing);
            
            if (selectedOption == i) {
                std::string displayText = "> " + options[i] + " <";
                sf::Text optionText = GUI::createText(font, displayText, 40, sf::Color::Yellow, (windowWidth - 300) / 2, yPos);
                window.draw(optionText);
            } else {
                sf::Text optionText = GUI::createText(font, options[i], 34, sf::Color::White, (windowWidth - 220) / 2, yPos);
                window.draw(optionText);
            }
        }
        
        sf::Text hint = GUI::createText(font, "UP/DOWN: Navigate | ENTER: Submit | ESC: Back", 12, sf::Color::Green, 145, windowHeight - 35);
        window.draw(hint);
    }
    
    // Message
    if (!message.empty()) {
        sf::Text msgText = GUI::createText(font, message, 15, messageColor, (windowWidth - 300) / 2, windowHeight - 20);
        window.draw(msgText);
    }
}

void LoginScreen::reset() {
    usernameInput = "";
    passwordInput = "";
    emailInput = "";
    nicknameInput = "";
    message = "";
    selectedField = 0;
    shouldStart = false;
    selectedOption = 0;
    inInputMode = false;
}

bool LoginScreen::shouldStartGame() const {
    return shouldStart;
}

std::string LoginScreen::getCurrentScreen() const {
    return currentScreen;
}

std::string LoginScreen::getMessage() const {
    return message;
}

void LoginScreen::setInitialScreen(const std::string& screen) {
    if (screen == "login" || screen == "register") {
        currentScreen = screen;
        selectedOption = 0;
        inInputMode = true;
        selectedField = 0;
        usernameInput = "";
        passwordInput = "";
        emailInput = "";
        nicknameInput = "";
        message = "";
    }
}