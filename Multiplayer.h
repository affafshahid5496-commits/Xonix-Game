#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "Player.h"
#include "authentication.h"

// Game constants for multiplayer
const int MP_GRID_ROWS = 25;
const int MP_GRID_COLS = 40;
const int MP_TILE_SIZE = 18;
const int MP_WINDOW_WIDTH = 1200;
const int MP_WINDOW_HEIGHT = 630;
const int MP_BOARD_WIDTH = MP_GRID_COLS * MP_TILE_SIZE;
const int MP_BOARD_HEIGHT = MP_GRID_ROWS * MP_TILE_SIZE;
const int MP_BOARD_X = (MP_WINDOW_WIDTH - MP_BOARD_WIDTH) / 2;
const int MP_BOARD_Y = (MP_WINDOW_HEIGHT - MP_BOARD_HEIGHT) / 2;

// Multiplayer Enemy
struct MP_Enemy {
    int x, y, dx, dy;
    bool frozen;
    float freezeTimer;

    MP_Enemy();
    MP_Enemy(int minSpd, int maxSpd);
    void move(int grid[MP_GRID_ROWS][MP_GRID_COLS], int tileSize);
    void freeze();
    void unfreeze();
};

// Multiplayer Player
struct MP_Player {
    int x, y;
    int dx, dy;
    int score;
    int powerUps;
    int tilesCapturedThisMove;
    int bonusCounter;
    bool isAlive;
    bool frozen;
    float freezeTimer;
    bool isConstructing;
    std::string username;
    sf::Color color;
    
    MP_Player();
    void reset();
    void updateScore(int tilesCaptured);
};

class MultiplayerScreen {
private:
    int grid[MP_GRID_ROWS][MP_GRID_COLS];
    bool gamePaused;
    bool gameOver;
    bool gameStarted;
    
    // Players
    MP_Player player1;
    MP_Player player2;
    Player* player1Profile;
    Player* player2Profile;
    AuthSystem* authSystem;
    
    // Enemies
    MP_Enemy enemies[10];
    int enemyCount;
    
    // Timing
    float moveTimer;
    float moveDelay;
    
    // Movement tracking
    bool p1WasMoving;
    bool p2WasMoving;
    
    // Textures and sprites
    sf::Texture tileTexture, enemyTexture, backgroundTexture;
    sf::Sprite tileSpriteSheet, enemySprite, backgroundSprite;
    
    // Audio
    sf::Music backgroundMusic;
    
public:
    MultiplayerScreen();
    ~MultiplayerScreen();
    
    // Initialize game
    void initialize(Player* p1, Player* p2 = nullptr, AuthSystem* auth = nullptr);
    void reset();
    
    // Handle input
    void handleEvent(sf::Event& event);
    
    // Update game logic
    void update(float deltaTime);
    
    // Render
    void draw(sf::RenderWindow& window, sf::Font& font);
    
    // Game state queries
    bool isGameOver() const { return gameOver; }
    bool isPaused() const { return gamePaused; }
    bool isGameStarted() const { return gameStarted; }
    
    // Player info getters
    std::string getPlayer1Name() const { return player1.username; }
    std::string getPlayer2Name() const { return player2.username; }
    Player* getPlayer1Profile() const { return player1Profile; }
    Player* getPlayer2Profile() const { return player2Profile; }
    int getPlayer1Score() const { return player1.score; }
    int getPlayer2Score() const { return player2.score; }
    
    // Game state control
    void setPaused(bool paused) { gamePaused = paused; }
    void startGame() { gameStarted = true; }
    
    // Texture and resource loading
    bool loadTextures();
    
    // Game outcome
    void determineWinner();
    
private:
    // Grid operations
    void initializeGrid();
    void dropP1(int y, int x);
    void dropP2(int y, int x);
    int countP1CapturedTiles();
    int countP2CapturedTiles();
    void awardPointsP1(int capturedBefore);
    void awardPointsP2(int capturedBefore);
    
    // Player actions
    void usePowerUp(MP_Player& player);
    void checkCollisions();
    void checkGameOver();
    
    // Drawing helpers
    void drawGameBoard(sf::RenderWindow& window);
    void drawPlayers(sf::RenderWindow& window);
    void drawEnemies(sf::RenderWindow& window);
    void drawUI(sf::RenderWindow& window, sf::Font& font);
    void drawGameOverScreen(sf::RenderWindow& window, sf::Font& font);
    void drawWaitingScreen(sf::RenderWindow& window, sf::Font& font);
};