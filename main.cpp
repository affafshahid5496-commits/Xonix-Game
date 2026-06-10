#include <SFML/Graphics.hpp>
#include <iostream>
#include "LoginScreen.h"
#include "GameScreen.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "Multiplayer.h"
#include "FriendsMenu.h"
#include "Leaderboard.h"
#include "Levels.h"
#include "SavedGamesScreen.h"
#include "Matchmaking.h"
#include "MatchmakingDifficulty.h"
#include "authentication.h"
#include "ThemesScreen.h"
#include "theme_manager.h"
#include "Theme.h"
#include "ProfileScreen.h"

enum class AppState {
    LOGIN_SCREEN,      // Login screen
    REGISTER_SCREEN,   // Register screen
    MAIN_MENU,         // Main menu with all options
    LEVEL_SELECT,      // Level/Difficulty selection for single player
    GAME,              // Active game
    GAME_PAUSED,       // Game paused with pause menu
    MULTIPLAYER,       // Multiplayer game
    GAME_OVER,         // Game over state
    PROFILE,           // Profile view
    LEADERBOARD,       // Leaderboard view
    FRIENDS,           // Friends management
    THEMES,            // Theme selection
    SAVED_GAMES,       // Saved games browser
    MATCHMAKING_DIFFICULTY,  // Select difficulty for matchmaking
    MATCHMAKING        // Matchmaking queue
};

class Application {
private:
    sf::RenderWindow window;
    sf::Font font;
    AuthSystem authSystem;
    Player* currentPlayer;
    
    LoginScreen loginScreen;
    MainMenu mainMenu;
    PauseMenu pauseMenu;
    Levels levelsScreen;
    GameScreen gameScreen;
    MultiplayerScreen multiplayerScreen;
    FriendsMenu friendsMenu;
    SavedGamesScreen savedGamesScreen;
    Leaderboard leaderboard{"players_data.txt", "leaderboard.txt"};
    Matchmaking matchmaking;
    MatchmakingDifficulty matchmakingDifficulty;
    ThemeManager themeManager;
    ThemesScreen themesScreen;
    ProfileScreen profileScreen;
    Levels::Difficulty selectedDifficulty;
    
    // Matchmaking timing
    float matchmakingTimer;
    const float MATCHMAKING_INTERVAL = 2.0f; // Check for matches every 2 seconds
    
    // Current match tracking
    GameRoom* currentMatchRoom;
    
    AppState currentState;
    bool shouldExit;
    
public:
    Application()
        : window(sf::VideoMode(1200, 630), "XONIX Game"),
          currentPlayer(nullptr),
          currentState(AppState::LOGIN_SCREEN),
          selectedDifficulty(Levels::MEDIUM),
          matchmakingTimer(0),
          currentMatchRoom(nullptr),
          shouldExit(false) {
        
        window.setFramerateLimit(60);
        
        // Load font
        if (!font.loadFromFile("../arial.ttf")) {
            std::cerr << "Warning: Could not load font file (arial.ttf)" << std::endl;
            // You can create a basic font here as fallback
        }
        
        // Initialize screens
        mainMenu.initialize(font);
        mainMenu.setPlayerStatus(false); // Start as guest
        
        // Load background image for main menu (optional - user can specify any image path)
        mainMenu.setBackgroundImage("../images/3d-fantasy-scene.jpg");
        
        pauseMenu.initialize(font);
        levelsScreen.initialize(font);
        matchmakingDifficulty.initialize(font);
        themesScreen = ThemesScreen(&themeManager, currentPlayer);
        
        // Initialize matchmaking with leaderboard
        matchmaking.setLeaderboard(&leaderboard);
    }
    
    bool run() {
        sf::Clock clock;
        
        while (window.isOpen() && !shouldExit) {
            float deltaTime = clock.restart().asSeconds();
            
            handleEvents();
            update(deltaTime);
            render();
        }
        
        // Save on exit
        if (currentPlayer) {
            authSystem.saveSystemState();
        }
        
        return true;
    }
    
private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            switch (currentState) {
                case AppState::LOGIN_SCREEN:
                case AppState::REGISTER_SCREEN:
                    handleLoginEvents(event);
                    break;
                case AppState::MAIN_MENU:
                    handleMainMenuEvents(event);
                    break;
                case AppState::LEVEL_SELECT:
                    handleLevelSelectEvents(event);
                    break;
                case AppState::MATCHMAKING_DIFFICULTY:
                    handleMatchmakingDifficultyEvents(event);
                    break;
                case AppState::GAME:
                    handleGameEvents(event);
                    break;
                case AppState::GAME_PAUSED:
                    handlePauseMenuEvents(event);
                    break;
                case AppState::MULTIPLAYER:
                    handleMultiplayerEvents(event);
                    break;
                case AppState::GAME_OVER:
                    handleGameOverEvents(event);
                    break;
                case AppState::PROFILE:
                case AppState::LEADERBOARD:
                case AppState::FRIENDS:
                case AppState::THEMES:
                case AppState::SAVED_GAMES:
                case AppState::MATCHMAKING:
                    handleSubMenuEvents(event);
                    break;
            }
        }
    }
    
    void handleLoginEvents(sf::Event& event) {
        loginScreen.handleEvent(event, authSystem, window);
        
        // Check if user wants to proceed to main menu
        if (loginScreen.shouldStartGame()) {
            currentPlayer = authSystem.getCurrentPlayer();
            mainMenu.setPlayerStatus(currentPlayer != nullptr, currentPlayer);
            currentState = AppState::MAIN_MENU;
            mainMenu.resetSelection();
        }
        // Check if user went back to main screen (from login/register) - return to MainMenu
        else if (loginScreen.getCurrentScreen() == "main") {
            // User is back at the login screen's main menu, return to app's main menu
            currentPlayer = authSystem.getCurrentPlayer();
            mainMenu.setPlayerStatus(currentPlayer != nullptr, currentPlayer);
            currentState = AppState::MAIN_MENU;
            mainMenu.resetSelection();
        }
    }
    
    void handleMainMenuEvents(sf::Event& event) {
        mainMenu.handleEvent(event, window);
        
        // Handle menu selection with Enter key
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
            int action = mainMenu.getSelectedAction();
            handleMenuAction(action);
        }
        
        // Also handle mouse clicks for menu selection
        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            int action = mainMenu.getSelectedAction();
            handleMenuAction(action);
        }
        
        // Handle escape to exit from main menu
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            shouldExit = true;
        }
    }
    
    void handleLevelSelectEvents(sf::Event& event) {
        levelsScreen.handleEvent(event, window);
        
        // Handle level selection with Enter key
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
            int action = levelsScreen.getSelectedAction();
            handleLevelSelectAction(action);
        }
        
        // Handle mouse clicks for level selection
        if (event.type == sf::Event::MouseButtonPressed && 
            event.mouseButton.button == sf::Mouse::Left) {
            int action = levelsScreen.getSelectedAction();
            handleLevelSelectAction(action);
        }
        
        // Handle escape to go back to main menu
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            returnToMainMenu();
        }
    }
    
    void handleLevelSelectAction(int action) {
        switch (action) {
            case Levels::SELECT_EASY:
                selectedDifficulty = Levels::EASY;
                startGame();
                break;
                
            case Levels::SELECT_MEDIUM:
                selectedDifficulty = Levels::MEDIUM;
                startGame();
                break;
                
            case Levels::SELECT_HARD:
                selectedDifficulty = Levels::HARD;
                startGame();
                break;
                
            case Levels::BACK_TO_MENU:
                returnToMainMenu();
                break;
                
            default:
                break;
        }
    }
    
    void handleMenuAction(int action) {
        switch (action) {
            case MainMenu::START_GAME:
                currentState = AppState::LEVEL_SELECT;
                levelsScreen.resetSelection();
                break;
                
            case MainMenu::LOGIN:
                // Go to login screen
                currentState = AppState::LOGIN_SCREEN;
                loginScreen.reset();
                loginScreen.setInitialScreen("login");
                break;
                
            case MainMenu::REGISTER:
                // Go to register screen
                currentState = AppState::REGISTER_SCREEN;
                loginScreen.reset();
                loginScreen.setInitialScreen("register");
                break;
                
            case MainMenu::MULTIPLAYER:
                if (currentPlayer) {
                    startMultiplayer();
                } else {
                    // Guests need to login to play multiplayer
                    currentState = AppState::LOGIN_SCREEN;
                    loginScreen.reset();
                }
                break;
                
            case MainMenu::LEADERBOARD:
                showLeaderboard();
                break;
                
            case MainMenu::PROFILE:
                if (currentPlayer) {
                    showProfile();
                } else {
                    // Guests go to login screen
                    currentState = AppState::LOGIN_SCREEN;
                    loginScreen.reset();
                }
                break;
                
            case MainMenu::FRIENDS:
                if (currentPlayer) {
                    showFriends();
                } else {
                    currentState = AppState::LOGIN_SCREEN;
                    loginScreen.reset();
                }
                break;
                
            case MainMenu::THEMES:
                showThemes();
                break;
                
            case MainMenu::SAVED_GAMES:
                if (currentPlayer) {
                    showSavedGames();
                } else {
                    currentState = AppState::LOGIN_SCREEN;
                    loginScreen.reset();
                }
                break;
                
            case MainMenu::MATCHMAKING:
                if (currentPlayer) {
                    showMatchmakingDifficulty();
                } else {
                    currentState = AppState::LOGIN_SCREEN;
                    loginScreen.reset();
                }
                break;
                
            case MainMenu::LOGOUT:
                logout();
                break;
                
            case MainMenu::EXIT:
                shouldExit = true;
                window.close();
                break;
                
            default:
                break;
        }
    }
    
    void handleGameEvents(sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                // Go to pause menu instead of directly quitting
                currentState = AppState::GAME_PAUSED;
                pauseMenu.resetSelection();
                return;
            }
        }
        
        gameScreen.handleEvent(event);
        
        // Check if game is over
        if (gameScreen.isGameOver()) {
            currentState = AppState::GAME_OVER;
            // Update player stats if logged in
            if (currentPlayer) {
                gameScreen.onGameComplete(gameScreen.getScore());
            }
        }
    }
    
    void handleGameOverEvents(sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
                // Restart the game with same difficulty
                gameScreen.reset();
                currentState = AppState::GAME;
            } else if (event.key.code == sf::Keyboard::M) {
                // Return to main menu
                returnToMainMenu();
            } else if (event.key.code == sf::Keyboard::Escape) {
                // Also allow ESC to return to menu
                returnToMainMenu();
            }
        }
        
        gameScreen.handleEvent(event);
    }
    
    void handlePauseMenuEvents(sf::Event& event) {
        pauseMenu.handleEvent(event, window);
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Return) {
                int action = pauseMenu.getSelectedAction();
                
                if (action == PauseMenu::RESUME_GAME) {
                    // Resume the game
                    currentState = AppState::GAME;
                } else if (action == PauseMenu::SAVE_GAME) {
                    // Save the game and return to main menu
                    if (currentPlayer) {
                        authSystem.savePlayerData(currentPlayer->getUsername());
                        gameScreen.saveCurrentGame();
                        // Also save game record to records file
                        if (gameScreen.getSaveGameManager()) {
                            gameScreen.getSaveGameManager()->saveGameRecord(
                                currentPlayer->getUsername(),
                                gameScreen.getScore(),
                                gameScreen.getCurrentDifficulty()
                            );
                        }
                        std::cout << "Game saved successfully!" << std::endl;
                    }
                    returnToMainMenu();
                } else if (action == PauseMenu::QUIT_GAME) {
                    // Quit to main menu
                    returnToMainMenu();
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                // ESC also resumes the game from pause
                currentState = AppState::GAME;
            }
        }
    }
    
    void handleMultiplayerEvents(sf::Event& event) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            // Return to main menu and close the match room
            if (currentMatchRoom) {
                matchmaking.closeGameRoom(currentMatchRoom->roomId);
                currentMatchRoom = nullptr;
            }
            returnToMainMenu();
            return;
        }
        
        multiplayerScreen.handleEvent(event);
        
        // Check if game is over
        if (multiplayerScreen.isGameOver()) {
            // Update leaderboard with match results
            updateLeaderboardWithMatchResults();
            
            // Close the match room
            if (currentMatchRoom) {
                matchmaking.closeGameRoom(currentMatchRoom->roomId);
                currentMatchRoom = nullptr;
            }
            
            // Save player data
            if (currentPlayer) {
                authSystem.saveSystemState();
            }
            
            returnToMainMenu();
        }
    }
    
    void handleMatchmakingDifficultyEvents(sf::Event& event) {
        matchmakingDifficulty.handleEvent(event, window);
        
        // Handle difficulty selection with Enter key
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
            int action = matchmakingDifficulty.getSelectedAction();
            handleMatchmakingDifficultyAction(action);
        }
        
        // Handle escape to go back to main menu
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            returnToMainMenu();
        }
    }
    
    void handleMatchmakingDifficultyAction(int action) {
        switch (action) {
            case MatchmakingDifficulty::SELECT_EASY:
            case MatchmakingDifficulty::SELECT_MEDIUM:
            case MatchmakingDifficulty::SELECT_HARD:
                // Selected difficulty, now go to matchmaking queue
                if (currentPlayer) {
                    matchmaking.removePlayerFromQueue(currentPlayer->getUsername()); // Clear if already in queue
                    // Use a default rank (1 = highest rank). In a full implementation, 
                    // you would search the leaderboard for the player's actual rank
                    int playerRank = 1; // Default rank
                    
                    // Determine difficulty and add to appropriate queue
                    Levels::Difficulty difficulty = Levels::MEDIUM; // Default
                    if (action == MatchmakingDifficulty::SELECT_EASY) {
                        difficulty = Levels::EASY;
                    } else if (action == MatchmakingDifficulty::SELECT_MEDIUM) {
                        difficulty = Levels::MEDIUM;
                    } else if (action == MatchmakingDifficulty::SELECT_HARD) {
                        difficulty = Levels::HARD;
                    }
                    
                    matchmaking.addPlayerToQueue(currentPlayer, playerRank, difficulty); // Add to queue with difficulty
                }
                currentState = AppState::MATCHMAKING;
                break;
                
            case MatchmakingDifficulty::NO_ACTION:
            default:
                break;
        }
    }
    
    void handleProfileAction(int action) {
        if (!currentPlayer) return;
        
        switch (action) {
            case ProfileScreen::EDIT_NICKNAME: {
                std::string newNickname;
                std::cout << "Enter new nickname (current: " << currentPlayer->getNickname() << "): ";
                std::getline(std::cin, newNickname);
                
                if (!newNickname.empty()) {
                    currentPlayer->setNickname(newNickname);
                    authSystem.updatePlayerProfile(currentPlayer->getUsername(), newNickname, currentPlayer->getEmail());
                    std::cout << "Nickname updated successfully!" << std::endl;
                    profileScreen.setPlayer(currentPlayer); // Refresh display
                }
                break;
            }
            case ProfileScreen::EDIT_EMAIL: {
                std::string newEmail;
                std::cout << "Enter new email (current: " << (currentPlayer->getEmail().empty() ? "Not set" : currentPlayer->getEmail()) << "): ";
                std::getline(std::cin, newEmail);
                
                if (!newEmail.empty() && Player::validateEmail(newEmail)) {
                    currentPlayer->setEmail(newEmail);
                    authSystem.updatePlayerProfile(currentPlayer->getUsername(), currentPlayer->getNickname(), newEmail);
                    std::cout << "Email updated successfully!" << std::endl;
                    profileScreen.setPlayer(currentPlayer); // Refresh display
                } else if (!newEmail.empty()) {
                    std::cout << "Invalid email format!" << std::endl;
                }
                break;
            }
            case ProfileScreen::CHANGE_PASSWORD: {
                std::string oldPassword, newPassword, confirmPassword;
                std::cout << "Enter current password: ";
                std::getline(std::cin, oldPassword);
                
                if (oldPassword != currentPlayer->getPassword()) {
                    std::cout << "Incorrect password!" << std::endl;
                    break;
                }
                
                std::cout << "Enter new password (min 6 chars, must contain letter and number): ";
                std::getline(std::cin, newPassword);
                
                if (!Player::validatePassword(newPassword)) {
                    std::cout << "Password does not meet requirements!" << std::endl;
                    break;
                }
                
                std::cout << "Confirm new password: ";
                std::getline(std::cin, confirmPassword);
                
                if (newPassword != confirmPassword) {
                    std::cout << "Passwords do not match!" << std::endl;
                    break;
                }
                
                if (authSystem.changePassword(currentPlayer->getUsername(), oldPassword, newPassword)) {
                    std::cout << "Password changed successfully!" << std::endl;
                }
                break;
            }
            case ProfileScreen::BACK:
                returnToMainMenu();
                break;
            case ProfileScreen::NO_ACTION:
            default:
                break;
        }
    }
    
    void handleSubMenuEvents(sf::Event& event) {
        // Handle Friends menu specific events first (must process before global ESC handler)
        if (currentState == AppState::FRIENDS) {
            friendsMenu.handleEvent(event, window);
            
            // Check if user selected BACK from friends menu
            if (friendsMenu.shouldReturn()) {
                friendsMenu.resetReturnFlag();
                currentState = AppState::MAIN_MENU;
                mainMenu.resetSelection();
                return;
            }
            // Don't process other handlers for Friends menu
            return;
        }
        
        // Handle escape key to return to main menu from other submenus
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            currentState = AppState::MAIN_MENU;
            mainMenu.resetSelection();
            return;
        }
        
        // Handle Themes menu specific events
        if (currentState == AppState::THEMES) {
            themesScreen.handleInput(event);
        }
        
        // Handle Profile menu specific events
        if (currentState == AppState::PROFILE) {
            profileScreen.handleInput(event);
            
            // Handle action selection with Enter key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                handleProfileAction(profileScreen.getSelectedAction());
            }
        }
        // Handle SavedGamesScreen events
        if (currentState == AppState::SAVED_GAMES) {
            savedGamesScreen.handleEvent(event, window);
            
            // Handle Enter key to select a saved game
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                SavedGameRecord* selectedGame = savedGamesScreen.getSelectedGame();
                if (selectedGame && currentPlayer) {
                    // Load the selected game
                    if (gameScreen.loadSavedGame(currentPlayer->getUsername())) {
                        currentState = AppState::GAME;
                        gameScreen.setPaused(false);
                    } else {
                        std::cout << "Failed to load saved game" << std::endl;
                    }
                }
            }
        }
        
        // Handle Matchmaking queue events
        if (currentState == AppState::MATCHMAKING) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return) {
                    // Player joins the matchmaking queue
                    if (currentPlayer) {
                        int rank = 1; // Default rank, can be fetched from leaderboard
                        matchmaking.addPlayerToQueue(currentPlayer, rank, Levels::MEDIUM); // Default to MEDIUM difficulty
                    }
                } else if (event.key.code == sf::Keyboard::C) {
                    // Cancel matchmaking and remove from queue
                    if (currentPlayer) {
                        matchmaking.removePlayerFromQueue(currentPlayer->getUsername());
                    }
                }
            }
        }
    }
    
    void startGame() {
        // Reinitialize game screen with selected difficulty
        gameScreen.initialize(currentPlayer, &authSystem, selectedDifficulty);
        
        // Apply current theme background to game screen
        Theme currentTheme = themeManager.getCurrentTheme();
        if (currentTheme.id != 0 && !currentTheme.imagePath.empty()) {
            gameScreen.setBackgroundImage(currentTheme.imagePath);
        }
        
        currentState = AppState::GAME;
    }
    
    void startMultiplayer() {
        // For now, both players will use the same logged-in player
        // In a full game, you might have guest player 2 or another logged-in player
        multiplayerScreen.initialize(currentPlayer, currentPlayer, &authSystem);
        multiplayerScreen.loadTextures();
        currentState = AppState::MULTIPLAYER;
    }
    
    void returnToLogin() {
        authSystem.logout();
        currentPlayer = nullptr;
        loginScreen.reset();
        mainMenu.setPlayerStatus(false);
        currentState = AppState::LOGIN_SCREEN;
    }
    
    void returnToMainMenu() {
        if (currentPlayer) {
            // Save any progress if needed
            authSystem.saveSystemState();
        }
        mainMenu.resetSelection();
        currentState = AppState::MAIN_MENU;
    }
    
    void logout() {
        authSystem.logout();
        currentPlayer = nullptr;
        mainMenu.setPlayerStatus(false);
        mainMenu.resetSelection();
        std::cout << "Successfully logged out" << std::endl;
        currentState = AppState::LOGIN_SCREEN;
        loginScreen.reset();
        loginScreen.setInitialScreen("login");
    }
    
    // Submenu display methods
    void showProfile() {
        if (currentPlayer) {
            profileScreen.setPlayer(currentPlayer);
            profileScreen.resetSelection();
        }
        currentState = AppState::PROFILE;
    }
    
    void showLeaderboard() {
        currentState = AppState::LEADERBOARD;
    }
    
    void showFriends() {
        if (currentPlayer) {
            friendsMenu.initialize(font, currentPlayer, &authSystem);
            friendsMenu.resetReturnFlag();
            friendsMenu.resetSelection();
        }
        currentState = AppState::FRIENDS;
    }
    
    void showThemes() {
        if (currentPlayer) {
            themesScreen = ThemesScreen(&themeManager, currentPlayer);
        } else {
            themesScreen = ThemesScreen(&themeManager, nullptr);
        }
        currentState = AppState::THEMES;
    }
    
    void showMatchmakingDifficulty() {
        if (currentPlayer) {
            matchmakingDifficulty.resetSelection();
            currentState = AppState::MATCHMAKING_DIFFICULTY;
        }
    }
    
    void showSavedGames() {
        if (currentPlayer) {
            savedGamesScreen.initialize(font, currentPlayer->getUsername());
        }
        currentState = AppState::SAVED_GAMES;
    }
    
    void update(float deltaTime) {
        // Process matchmaking periodically
        if (currentState == AppState::MATCHMAKING) {
            matchmakingTimer += deltaTime;
            if (matchmakingTimer >= MATCHMAKING_INTERVAL) {
                matchmakingTimer = 0;
                processMatchmaking();
            }
        }
        
        switch (currentState) {
            case AppState::LOGIN_SCREEN:
            case AppState::REGISTER_SCREEN:
                // Login/Register screen handles its own updates
                break;
            case AppState::MAIN_MENU:
                // Main menu updates handled in event handling
                break;
            case AppState::LEVEL_SELECT:
                // Level select updates handled in event handling
                break;
            case AppState::GAME:
                gameScreen.update(deltaTime);
                break;
            case AppState::GAME_PAUSED:
                // Game is paused, don't update game state
                break;
            case AppState::MULTIPLAYER:
                multiplayerScreen.update(deltaTime);
                break;
            case AppState::GAME_OVER:
                // Game over state is handled by GameScreen
                break;
            case AppState::PROFILE:
            case AppState::LEADERBOARD:
            case AppState::FRIENDS:
            case AppState::THEMES:
            case AppState::SAVED_GAMES:
            case AppState::MATCHMAKING_DIFFICULTY:
            case AppState::MATCHMAKING:
                // Submenu updates can be handled here if needed
                break;
        }
    }
    
    void updateLeaderboardWithMatchResults() {
        // Update leaderboard with player scores from multiplayer match
        if (currentMatchRoom && currentMatchRoom->player1 && currentMatchRoom->player2) {
            // Get the scores of both players
            int player1Score = currentMatchRoom->player1->getTotalScore();
            int player2Score = currentMatchRoom->player2->getTotalScore();
            
            // Update leaderboard for both players
            leaderboard.updatePlayerScore(
                currentMatchRoom->player1->getPlayerId(),
                player1Score,
                currentMatchRoom->player1->getUsername()
            );
            
            leaderboard.updatePlayerScore(
                currentMatchRoom->player2->getPlayerId(),
                player2Score,
                currentMatchRoom->player2->getUsername()
            );
            
            std::cout << "Leaderboard updated: " << currentMatchRoom->player1->getUsername() 
                      << " (" << player1Score << " pts) vs " 
                      << currentMatchRoom->player2->getUsername() 
                      << " (" << player2Score << " pts)\n";
        }
    }
    
    void processMatchmaking() {
        // Check if there are at least 2 players waiting in queue
        if (matchmaking.getQueueSize() >= 2) {
            // Call the matchmaking system to create a match
            matchmaking.processMatchmaking();
            
            // Get the newly created game room
            currentMatchRoom = matchmaking.getLastCreatedRoom();
            
            if (currentMatchRoom && currentMatchRoom->player1 && currentMatchRoom->player2) {
                // Initialize multiplayer screen with the matched players
                multiplayerScreen.initialize(currentMatchRoom->player1, currentMatchRoom->player2, &authSystem);
                currentState = AppState::MULTIPLAYER;
                std::cout << "Match started: " << currentMatchRoom->player1->getUsername() 
                          << " (Rank: 1) vs " << currentMatchRoom->player2->getUsername() 
                          << " (Rank: 1) in Room " << currentMatchRoom->roomId << "\n";
            }
        }
    }
    
    void render() {
        // Don't clear the window if we're paused - this prevents blinking
        if (currentState != AppState::GAME_PAUSED) {
            window.clear(sf::Color::Black);
        }
        
        switch (currentState) {
            case AppState::LOGIN_SCREEN:
            case AppState::REGISTER_SCREEN:
                loginScreen.draw(window, font);
                break;
            case AppState::MAIN_MENU:
                mainMenu.draw(window);
                break;
            case AppState::LEVEL_SELECT:
                levelsScreen.draw(window);
                break;
            case AppState::GAME:
            case AppState::GAME_OVER:
                gameScreen.draw(window, font);
                break;
            case AppState::GAME_PAUSED:
                // Draw the game in the background with pause menu overlay
                gameScreen.draw(window, font);
                pauseMenu.draw(window);
                break;
            case AppState::MULTIPLAYER:
                multiplayerScreen.draw(window, font);
                break;
            case AppState::PROFILE:
                renderProfileScreen();
                break;
            case AppState::LEADERBOARD:
                renderLeaderboardScreen();
                break;
            case AppState::FRIENDS:
                friendsMenu.draw(window);
                break;
            case AppState::THEMES:
                renderThemesScreen();
                break;
            case AppState::SAVED_GAMES:
                renderSavedGamesScreen();
                break;
            case AppState::MATCHMAKING_DIFFICULTY:
                matchmakingDifficulty.draw(window);
                break;
            case AppState::MATCHMAKING:
                renderMatchmakingScreen();
                break;
        }
        
        window.display();
    }
    
    // Submenu rendering methods
    void renderProfileScreen() {
        profileScreen.draw(window, font);
        renderBackButton();
    }
    
    void renderLeaderboardScreen() {
        // Reload leaderboard data from file before displaying
        leaderboard.reload();
        
        sf::Text title;
        title.setFont(font);
        title.setString("LEADERBOARD");
        title.setCharacterSize(48);
        title.setFillColor(sf::Color::Yellow);
        title.setStyle(sf::Text::Bold);
        int titleX = 600 - title.getLocalBounds().width / 2;
        title.setPosition(titleX, 30);
        window.draw(title);
        
        // Display top players
        int topCount = leaderboard.getTopPlayersCount();
        if (topCount == 0) {
            sf::Text emptyText;
            emptyText.setFont(font);
            emptyText.setString("No players on leaderboard yet!\nPlay some games to get on the leaderboard.");
            emptyText.setCharacterSize(24);
            emptyText.setFillColor(sf::Color::White);
            int emptyX = 600 - emptyText.getLocalBounds().width / 2;
            emptyText.setPosition(emptyX, 200);
            window.draw(emptyText);
        } else {
            // Get sorted leaderboard data
            LeaderboardNode* sortedPlayers = new LeaderboardNode[topCount];
            leaderboard.getTopPlayersSorted(sortedPlayers);
            
            int startY = 120;
            int spacing = 35;
            
            // Display top 10 or available players
            int displayCount = (topCount > 10) ? 10 : topCount;
            
            for (int i = 0; i < displayCount; i++) {
                // Rank number with different colors for top 3
                sf::Text rankText;
                rankText.setFont(font);
                rankText.setString(std::to_string(i + 1) + ".");
                rankText.setCharacterSize(28);
                if (i == 0) rankText.setFillColor(sf::Color(255, 215, 0));
                else if (i == 1) rankText.setFillColor(sf::Color(192, 192, 192));
                else if (i == 2) rankText.setFillColor(sf::Color(205, 127, 50)); // Bronze
                else rankText.setFillColor(sf::Color::White);
                rankText.setPosition(150, startY + (i * spacing));
                window.draw(rankText);
                
                // Player username
                sf::Text playerIdText;
                playerIdText.setFont(font);
                playerIdText.setString(sortedPlayers[i].username.empty() ? "Player #" + std::to_string(sortedPlayers[i].playerId) : sortedPlayers[i].username);
                playerIdText.setCharacterSize(24);
                playerIdText.setFillColor(sf::Color::Cyan);
                playerIdText.setPosition(250, startY + (i * spacing));
                window.draw(playerIdText);
                
                // Score
                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setString(std::to_string(sortedPlayers[i].score) + " pts");
                scoreText.setCharacterSize(24);
                scoreText.setFillColor(sf::Color::Green);
                int scoreX = 800 - scoreText.getLocalBounds().width / 2;
                scoreText.setPosition(scoreX, startY + (i * spacing));
                window.draw(scoreText);
            }
            
            delete[] sortedPlayers;
        }
        
        renderBackButton();
    }
    
    void renderThemesScreen() {
        themesScreen.draw(window, font);
        renderBackButton();
    }
    
    void renderSavedGamesScreen() {
        savedGamesScreen.draw(window);
        renderBackButton();
    }
    
    void renderMatchmakingScreen() {
        sf::Text title;
        title.setFont(font);
        title.setString("MATCHMAKING QUEUE");
        title.setCharacterSize(48);
        title.setFillColor(sf::Color::Green);
        title.setStyle(sf::Text::Bold);
        int titleX = 600 - title.getLocalBounds().width / 2;
        title.setPosition(titleX, 30);
        window.draw(title);
        
        int queueSize = matchmaking.getQueueSize();
        int activeRooms = matchmaking.getActiveRoomCount();
        
        sf::Text statusText;
        statusText.setFont(font);
        statusText.setString("Players waiting: " + std::to_string(queueSize) + 
                            "\nActive matches: " + std::to_string(activeRooms));
        statusText.setCharacterSize(28);
        statusText.setFillColor(sf::Color::Cyan);
        int statusX = 600 - statusText.getLocalBounds().width / 2;
        statusText.setPosition(statusX, 150);
        window.draw(statusText);
        
        sf::Text infoText;
        infoText.setFont(font);
        if (queueSize == 0) {
            infoText.setString("Press ENTER to join the queue\nPress C to cancel matchmaking");
        } else {
            infoText.setString("You are in the queue at position " + std::to_string(queueSize) + 
                              "\nWaiting for opponent...\nPress C to cancel");
        }
        infoText.setCharacterSize(22);
        infoText.setFillColor(sf::Color::Yellow);
        int infoX = 600 - infoText.getLocalBounds().width / 2;
        infoText.setPosition(infoX, 280);
        window.draw(infoText);
        
        renderBackButton();
    }
    
    void showMatchmaking() {
        if (currentPlayer) {
            // Initialize matchmaking for this player
            // Reset player from queue if they were in one
            matchmaking.removePlayerFromQueue(currentPlayer->getUsername());
        }
        currentState = AppState::MATCHMAKING;
    }
    
    void renderBackButton() {
        sf::Text backText;
        backText.setFont(font);
        backText.setString("Press ESC to return to Main Menu");
        backText.setCharacterSize(18);
        backText.setFillColor(sf::Color::Yellow);
        int backX = 600 - backText.getLocalBounds().width / 2;
        backText.setPosition(backX, 550);
        window.draw(backText);
    }
};

int main() {
    try {
        Application app;
        if (app.run()) {
            std::cout << "Application exited successfully." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}