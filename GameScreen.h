#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "Player.h"
#include "authentication.h"
#include "Levels.h"
#include "SaveGame.h"

// Game constants
const int GRID_ROWS = 25;
const int GRID_COLS = 40;
const int TILE_SIZE = 18;
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 630;
const int BOARD_WIDTH = GRID_COLS * TILE_SIZE;   // 720
const int BOARD_HEIGHT = GRID_ROWS * TILE_SIZE;  // 450
const int BOARD_X = (WINDOW_WIDTH - BOARD_WIDTH) / 2;   // 240
const int BOARD_Y = (WINDOW_HEIGHT - BOARD_HEIGHT) / 2; // 90

struct Enemy
{
    int x, y, dx, dy;
    bool frozen;
    float freezeTimer;
    float speedMultiplier; // Speed modifier based on difficulty

    Enemy();
    void move(int grid[25][40], int tileSize);
    void freeze();
    void unfreeze();
    void setSpeedMultiplier(float speed) { speedMultiplier = speed; }
};

class GameScreen {
private:
    int grid[GRID_ROWS][GRID_COLS];
    int score;
    int powerUps;
    int tilesCapturedThisMove;
    int bonusCounter;
    bool gamePaused;
    bool gameOver;
    bool loadedFromSave;  // Track if game was loaded from saved game
    int bonusOccurrences;
    // Player and game state
    Player* currentPlayer;
    AuthSystem* authSystem;
    Enemy enemies[10];
    int enemyCount;
    float enemySpeed;
    float playerSpeed;
    
    // Player position and movement
    int playerX, playerY;
    int playerDX, playerDY;
    int lastPlayerX, lastPlayerY;  // Track previous position
    bool wallCollisionProcessed;    // Flag to prevent repeated collision processing
    
    // Timing
    float moveTimer;
    float moveDelay;
    float gameTimer;  // Total elapsed time in game
    
    // Textures and sprites
    sf::Texture tileTexture, enemyTexture, gameoverTexture, backgroundTexture;
    sf::Sprite tileSpriteSheet, enemySprite, gameoverSprite, backgroundSprite;
    
    // Audio
    sf::Music backgroundMusic;
    
    // Save game manager
    SaveGameManager* saveGameManager;
    Levels::Difficulty currentDifficulty;
    
    // Theme support
    std::string backgroundImagePath;
    
public:
    GameScreen();
    ~GameScreen();
    
    // Initialize game
    void initialize(Player* player, AuthSystem* auth = nullptr, Levels::Difficulty difficulty = Levels::MEDIUM);
    void reset();
    void setBackgroundImage(const std::string& imagePath);  // Set background from theme
    void onGameComplete(int finalScore);
    void applyDifficultySettings(Levels::Difficulty difficulty);
    // Handle input
    void handleEvent(sf::Event& event);
    
    // Update game logic
    void update(float deltaTime);
    
    // Render
    void draw(sf::RenderWindow& window, sf::Font& font);
    
    // Game state queries
    bool isGameOver() const { return gameOver; }
    bool isPaused() const { return gamePaused; }
    int getScore() const { return score; }
    int getPowerUps() const { return powerUps; }
    int getCurrentDifficulty() const { return (int)currentDifficulty; }
    bool isLoadedFromSave() const { return loadedFromSave; }
    int getCurrentPlayerId();
    // Game state control
    void setPaused(bool paused) { gamePaused = paused; }
    void usePowerUp();
    
    // Save/Load functionality
    GameSave createGameSave() const;
    bool loadGameSave(const GameSave& save);
    bool saveCurrentGame();
    bool loadSavedGame(const std::string& username);
    bool hasSavedGame(const std::string& username) const;
    bool deleteSavedGame(const std::string& username);
    SaveGameManager* getSaveGameManager() const { return saveGameManager; }
    
private:
    // Grid operations
    void initializeGrid();
    void drop(int y, int x);
    
    // Scoring system
    void updateScore(int tilesCaptured);
    void checkGameOver();
    
    // Drawing helpers
    void drawGameBoard(sf::RenderWindow& window);
    void drawPlayer(sf::RenderWindow& window);
    void drawEnemies(sf::RenderWindow& window);
    void drawUI(sf::RenderWindow& window, sf::Font& font);
    void drawGameOverScreen(sf::RenderWindow& window, sf::Font& font);
    
    // Utility
    bool loadTextures();
};
