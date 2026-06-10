#include "Multiplayer.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

// MP_Enemy implementation
MP_Enemy::MP_Enemy() {
    x = (5 + rand() % (MP_GRID_COLS - 10)) * MP_TILE_SIZE;
    y = (5 + rand() % (MP_GRID_ROWS - 10)) * MP_TILE_SIZE;
    dx = 4 - rand() % 8;
    dy = 4 - rand() % 8;
    frozen = false;
    freezeTimer = 0;
}

MP_Enemy::MP_Enemy(int minSpd, int maxSpd) {
    x = (5 + rand() % (MP_GRID_COLS - 10)) * MP_TILE_SIZE;
    y = (5 + rand() % (MP_GRID_ROWS - 10)) * MP_TILE_SIZE;
    
    int speed = minSpd + rand() % (maxSpd - minSpd + 1);
    dx = (rand() % 2) ? speed : -speed;
    dy = (rand() % 2) ? speed : -speed;
    
    frozen = false;
    freezeTimer = 0;
}

void MP_Enemy::move(int grid[MP_GRID_ROWS][MP_GRID_COLS], int tileSize) {
    if (frozen) return;

    x += dx;
    int gridX = x / tileSize;
    int gridY = y / tileSize;
    
    // Bounce off walls, player trails, and captured areas
    if (gridX < 0 || gridX >= MP_GRID_COLS || gridY < 0 || gridY >= MP_GRID_ROWS || 
        grid[gridY][gridX] == 1 || grid[gridY][gridX] == 2 || grid[gridY][gridX] == 3 || 
        grid[gridY][gridX] == 4 || grid[gridY][gridX] == 5) {
        dx = -dx;
        x += dx;
    }

    y += dy;
    gridX = x / tileSize;
    gridY = y / tileSize;
    
    // Bounce off walls, player trails, and captured areas
    if (gridY < 0 || gridY >= MP_GRID_ROWS || gridX < 0 || gridX >= MP_GRID_COLS || 
        grid[gridY][gridX] == 1 || grid[gridY][gridX] == 2 || grid[gridY][gridX] == 3 || 
        grid[gridY][gridX] == 4 || grid[gridY][gridX] == 5) {
        dy = -dy;
        y += dy;
    }
}

void MP_Enemy::freeze() {
    frozen = true;
    freezeTimer = 3.0f;
}

void MP_Enemy::unfreeze() {
    frozen = false;
    freezeTimer = 0;
}

// MP_Player implementation
MP_Player::MP_Player() {
    reset();
    color = sf::Color::White;
}

void MP_Player::reset() {
    x = 10;
    y = 0;
    dx = 0;
    dy = 0;
    score = 0;
    powerUps = 0;
    tilesCapturedThisMove = 0;
    bonusCounter = 0;
    isAlive = true;
    frozen = false;
    freezeTimer = 0.0f;
    isConstructing = false;
    username = "Player";
}

void MP_Player::updateScore(int tilesCaptured) {
    if (tilesCaptured == 0) return;
    
    int threshold = (bonusCounter >= 3) ? 5 : 10;
    
    if (tilesCaptured > threshold) {
        bonusCounter++;
    }
    
    int multiplier = 1;
    if (bonusCounter >= 1) multiplier = 2;
    if (bonusCounter >= 3) multiplier = 3;
    if (bonusCounter >= 5) multiplier = 4;
    
    int points = tilesCaptured * multiplier;
    score += points;
    
    // Power-up system
    if (score >= 50 && powerUps == 0) {
        powerUps = 1;
    } else if (score >= 70 && powerUps == 1) {
        powerUps = 2;
    } else if (score >= 100 && powerUps == 2) {
        powerUps = 3;
    }
}

// MultiplayerScreen implementation
MultiplayerScreen::MultiplayerScreen() 
    : gamePaused(false), gameOver(false), gameStarted(false),
      player1Profile(nullptr), player2Profile(nullptr), authSystem(nullptr),
      enemyCount(6), moveTimer(0), moveDelay(0.07f),
      p1WasMoving(false), p2WasMoving(false) {
    
    initializeGrid();
    loadTextures();
    
    // Initialize players
    player1.color = sf::Color::Blue;
    player1.username = "Player 1";
    
    player2.color = sf::Color::Red;
    player2.username = "Player 2";
    
    // Initialize enemies
    for (int i = 0; i < 10; i++) {
        enemies[i] = MP_Enemy();
    }
}

MultiplayerScreen::~MultiplayerScreen() {
}

bool MultiplayerScreen::loadTextures() {
    if (!tileTexture.loadFromFile("images/tiles.png")) {
        std::cerr << "Error loading tiles texture" << std::endl;
        return false;
    }
    if (!enemyTexture.loadFromFile("images/enemy.png")) {
        std::cerr << "Error loading enemy texture" << std::endl;
        return false;
    }
    if (!backgroundTexture.loadFromFile("images/3d-fantasy-scene.jpg")) {
        std::cerr << "Warning: Could not load background texture" << std::endl;
    }
    
    tileSpriteSheet.setTexture(tileTexture);
    enemySprite.setTexture(enemyTexture);
    backgroundSprite.setTexture(backgroundTexture);
    
    backgroundSprite.setPosition(MP_BOARD_X, MP_BOARD_Y);
    backgroundSprite.setScale(
        (float)MP_BOARD_WIDTH / backgroundTexture.getSize().x,
        (float)MP_BOARD_HEIGHT / backgroundTexture.getSize().y
    );
    
    enemySprite.setOrigin(20, 20);
    
    if (!backgroundMusic.openFromFile("audio/Sakura-Girl-Yay-chosic.com_.wav")) {
        std::cerr << "Warning: Could not load background music from audio/Sakura-Girl-Yay-chosic.com_.wav" << std::endl;
    } else {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50.0f);
        backgroundMusic.play();
        std::cout << "Background music loaded and playing (Multiplayer)" << std::endl;
    }
    
    return true;
}

void MultiplayerScreen::initialize(Player* p1, Player* p2, AuthSystem* auth) {
    player1Profile = p1;
    player2Profile = p2;
    authSystem = auth;
    
    if (p1) {
        player1.username = p1->getUsername();
        player1.color = sf::Color::Blue;
    } else {
        player1.username = "Player 1";
    }
    
    if (p2) {
        player2.username = p2->getUsername();
        player2.color = sf::Color::Red;
    } else {
        player2.username = "Player 2";
    }
    
    reset();
}

void MultiplayerScreen::reset() {
    initializeGrid();
    gameOver = false;
    gameStarted = false;
    moveTimer = 0;
    p1WasMoving = false;
    p2WasMoving = false;
    
    player1.reset();
    player2.reset();
    player1.x = 10;
    player1.y = 0;
    player2.x = MP_GRID_COLS - 11;
    player2.y = MP_GRID_ROWS - 1;
    
    // Initialize enemies with better overlap detection
    for (int i = 0; i < enemyCount; i++) {
        bool overlaps = true;
        int attempts = 0;
        
        while (overlaps && attempts < 10) {
            enemies[i] = MP_Enemy();
            overlaps = false;
            
            // Check overlap with other enemies
            for (int j = 0; j < i; j++) {
                int dx = abs(enemies[i].x - enemies[j].x);
                int dy = abs(enemies[i].y - enemies[j].y);
                if (dx < MP_TILE_SIZE * 2 && dy < MP_TILE_SIZE * 2) {
                    overlaps = true;
                    break;
                }
            }
            
         
            
            attempts++;
        }
    }
}

void MultiplayerScreen::initializeGrid() {
    for (int i = 0; i < MP_GRID_ROWS; i++) {
        for (int j = 0; j < MP_GRID_COLS; j++) {
            if (i == 0 || j == 0 || i == MP_GRID_ROWS - 1 || j == MP_GRID_COLS - 1) {
                grid[i][j] = 1;
            } else {
                grid[i][j] = 0;
            }
        }
    }
}

void MultiplayerScreen::dropP1(int y, int x) {
    if (y < 0 || y >= MP_GRID_ROWS || x < 0 || x >= MP_GRID_COLS) return;
    
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (y - 1 >= 0 && grid[y - 1][x] == 0) dropP1(y - 1, x);
    if (y + 1 < MP_GRID_ROWS && grid[y + 1][x] == 0) dropP1(y + 1, x);
    if (x - 1 >= 0 && grid[y][x - 1] == 0) dropP1(y, x - 1);
    if (x + 1 < MP_GRID_COLS && grid[y][x + 1] == 0) dropP1(y, x + 1);
}

void MultiplayerScreen::dropP2(int y, int x) {
    if (y < 0 || y >= MP_GRID_ROWS || x < 0 || x >= MP_GRID_COLS) return;
    
    if (grid[y][x] == 0) grid[y][x] = -2;
    if (y - 1 >= 0 && grid[y - 1][x] == 0) dropP2(y - 1, x);
    if (y + 1 < MP_GRID_ROWS && grid[y + 1][x] == 0) dropP2(y + 1, x);
    if (x - 1 >= 0 && grid[y][x - 1] == 0) dropP2(y, x - 1);
    if (x + 1 < MP_GRID_COLS && grid[y][x + 1] == 0) dropP2(y, x + 1);
}

int MultiplayerScreen::countP1CapturedTiles() {
    int count = 0;
    for (int i = 0; i < MP_GRID_ROWS; i++) {
        for (int j = 0; j < MP_GRID_COLS; j++) {
            if (grid[i][j] == 4) {
                count++;
            }
        }
    }
    return count;
}

int MultiplayerScreen::countP2CapturedTiles() {
    int count = 0;
    for (int i = 0; i < MP_GRID_ROWS; i++) {
        for (int j = 0; j < MP_GRID_COLS; j++) {
            if (grid[i][j] == 5) {
                count++;
            }
        }
    }
    return count;
}

void MultiplayerScreen::awardPointsP1(int capturedBefore) {
    int capturedNow = countP1CapturedTiles();
    int tilesCaptured = capturedNow - capturedBefore;
    
    player1.tilesCapturedThisMove = tilesCaptured;
    
    if (tilesCaptured > 0) {
        player1.updateScore(tilesCaptured);
    }
    
    p1WasMoving = false;
}

void MultiplayerScreen::awardPointsP2(int capturedBefore) {
    int capturedNow = countP2CapturedTiles();
    int tilesCaptured = capturedNow - capturedBefore;
    
    player2.tilesCapturedThisMove = tilesCaptured;
    
    if (tilesCaptured > 0) {
        player2.updateScore(tilesCaptured);
    }
    
    p2WasMoving = false;
}

void MultiplayerScreen::usePowerUp(MP_Player& player) {
    if (player.powerUps > 0) {
        player.powerUps--;
        
        for (int i = 0; i < enemyCount; i++) {
            enemies[i].freeze();
        }
        
        if (player.username == player1.username) {
            player2.frozen = true;
            player2.freezeTimer = 3.0f;
        } else {
            player1.frozen = true;
            player1.freezeTimer = 3.0f;
        }
    }
}

void MultiplayerScreen::checkCollisions() {
    // Player to player collision
    if (player1.x == player2.x && player1.y == player2.y) {
        bool p1Moving = p1WasMoving;
        bool p2Moving = p2WasMoving;
        
        if (p1Moving && p2Moving) {
            player1.isAlive = false;
            player2.isAlive = false;
        } else if (p1Moving && !p2Moving) {
            player1.isAlive = false;
        } else if (p2Moving && !p1Moving) {
            player2.isAlive = false;
        }
    }
    
    // Player 1 touches opponent trail
    if (player1.isAlive && grid[player1.y][player1.x] == 3) {
        player1.isAlive = false;
    }
    
    // Player 2 touches opponent trail
    if (player2.isAlive && grid[player2.y][player2.x] == 2) {
        player2.isAlive = false;
    }
    
    // Check enemy collision
    for (int i = 0; i < enemyCount; i++) {
        int enemyGridX = enemies[i].x / MP_TILE_SIZE;
        int enemyGridY = enemies[i].y / MP_TILE_SIZE;
        
        if (player1.isAlive && (player1.x == enemyGridX && player1.y == enemyGridY)) {
            if (!enemies[i].frozen) {
                player1.isAlive = false;
            }
        }
        
        if (player2.isAlive && (player2.x == enemyGridX && player2.y == enemyGridY)) {
            if (!enemies[i].frozen) {
                player2.isAlive = false;
            }
        }
    }
}

void MultiplayerScreen::checkGameOver() {
    if (!player1.isAlive && !player2.isAlive) {
        gameOver = true;
        determineWinner();
    }
}

void MultiplayerScreen::determineWinner() {
    if (player1Profile) {
        player1Profile->updateScore(player1.score);
    }
    if (player2Profile) {
        player2Profile->updateScore(player2.score);
    }
}

void MultiplayerScreen::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (!gameStarted) {
            if (event.key.code == sf::Keyboard::Enter) {
                startGame();
            }
            return;
        }
        
        // Player 1 controls (Arrow keys)
        if (event.key.code == sf::Keyboard::Left) { 
            player1.dx = -1; player1.dy = 0;
            p1WasMoving = true;
        }
        else if (event.key.code == sf::Keyboard::Right) { 
            player1.dx = 1; player1.dy = 0;
            p1WasMoving = true;
        }
        else if (event.key.code == sf::Keyboard::Up) { 
            player1.dx = 0; player1.dy = -1;
            p1WasMoving = true;
        }
        else if (event.key.code == sf::Keyboard::Down) { 
            player1.dx = 0; player1.dy = 1;
            p1WasMoving = true;
        }
        else if (event.key.code == sf::Keyboard::RShift) { 
            usePowerUp(player1);
        }
        
        // Player 2 controls (WASD + LShift)
        else if (event.key.code == sf::Keyboard::A) { 
            player2.dx = -1; player2.dy = 0;
            p2WasMoving = true;
        }
        else if (event.key.code == sf::Keyboard::D) { 
            player2.dx = 1; player2.dy = 0;
            p2WasMoving = true;
        }
        else if (event.key.code == sf::Keyboard::W) { 
            player2.dx = 0; player2.dy = -1;
            p2WasMoving = true;
        }
        else if (event.key.code == sf::Keyboard::S) { 
            player2.dx = 0; player2.dy = 1;
            p2WasMoving = true;
        }
        else if (event.key.code == sf::Keyboard::LShift) { 
            usePowerUp(player2);
        }
    }
}

void MultiplayerScreen::update(float deltaTime) {
    if (!gameStarted || gameOver || gamePaused) return;

    moveTimer += deltaTime;

    // Update freeze timers
    if (player1.frozen) {
        player1.freezeTimer -= deltaTime;
        if (player1.freezeTimer <= 0) {
            player1.frozen = false;
            player1.freezeTimer = 0;
        }
    }
    
    if (player2.frozen) {
        player2.freezeTimer -= deltaTime;
        if (player2.freezeTimer <= 0) {
            player2.frozen = false;
            player2.freezeTimer = 0;
        }
    }

    // Update enemy freeze timers
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].frozen) {
            enemies[i].freezeTimer -= deltaTime;
            if (enemies[i].freezeTimer <= 0) {
                enemies[i].unfreeze();
            }
        }
    }

    if (moveTimer > moveDelay) {
        // PLAYER 1 MOVEMENT LOGIC
        if (player1.isAlive && p1WasMoving && !player1.frozen) {
            int newX = player1.x + player1.dx;
            int newY = player1.y + player1.dy;
            
            // Clamp to bounds
            if (newX < 0) newX = 0;
            if (newX >= MP_GRID_COLS) newX = MP_GRID_COLS - 1;
            if (newY < 0) newY = 0;
            if (newY >= MP_GRID_ROWS) newY = MP_GRID_ROWS - 1;
            
            int cellValue = grid[newY][newX];
            
            // Hit wall or opponent captured area - complete construction
            if (cellValue == 1 || cellValue == 5 || cellValue == 4) {
                int capturedBefore = countP1CapturedTiles();
                if(cellValue == 4){
                    player1.x = newX;
                    player1.y = newY;
                }

                player1.dx = player1.dy = 0;
                p1WasMoving = false;
                
                // Drop flood fill from enemies
                for (int i = 0; i < enemyCount; i++) {
                    dropP1(enemies[i].y / MP_TILE_SIZE, enemies[i].x / MP_TILE_SIZE);
                }
                
                // Capture all remaining empty spaces
                for (int i = 0; i < MP_GRID_ROWS; i++) {
                    for (int j = 0; j < MP_GRID_COLS; j++) {
                        if (grid[i][j] == -1) grid[i][j] = 0;
                        else if (grid[i][j] != 1 && grid[i][j] != 3 && grid[i][j] != 5) {
                            grid[i][j] = 4;
                        }
                    }
                }
                
                awardPointsP1(capturedBefore);
            }
            // Hit opponent trail - player dies
            else if (cellValue == 3) {
                player1.isAlive = false;
            }
            // Hit own trail - player dies
            else if (cellValue == 2) {
                player1.isAlive = false;
            }
            // Hit own captured tile - one tile per press, no trail
            /*else if (cellValue == 4) {
                player1.x = newX;
                player1.y = newY;
                // Don't leave trail on captured tiles
                // Reset movement flag to require new key press
                p1WasMoving = false;
            }*/
            // Empty space - move and leave trail (continuous movement)
            else if (cellValue == 0) {
                player1.x = newX;
                player1.y = newY;
                grid[newY][newX] = 2;
                player1.isConstructing = true;
            }
        }
        
        // PLAYER 2 MOVEMENT LOGIC
        if (player2.isAlive && p2WasMoving && !player2.frozen) {
            int newX = player2.x + player2.dx;
            int newY = player2.y + player2.dy;
            
            // Clamp to bounds
            if (newX < 0) newX = 0;
            if (newX >= MP_GRID_COLS) newX = MP_GRID_COLS - 1;
            if (newY < 0) newY = 0;
            if (newY >= MP_GRID_ROWS) newY = MP_GRID_ROWS - 1;
            
            int cellValue = grid[newY][newX];
            
            // Hit wall or opponent captured area - complete construction
            if (cellValue == 1 || cellValue == 4 || cellValue == 5) {
                int capturedBefore = countP2CapturedTiles();
                if(cellValue == 5){
                    player2.x = newX;
                    player2.y = newY;
                }
                player2.dx = player2.dy = 0;
                p2WasMoving = false;
                
                // Drop flood fill from enemies
                for (int i = 0; i < enemyCount; i++) {
                    dropP2(enemies[i].y / MP_TILE_SIZE, enemies[i].x / MP_TILE_SIZE);
                }
                
                // Capture all remaining empty spaces
                for (int i = 0; i < MP_GRID_ROWS; i++) {
                    for (int j = 0; j < MP_GRID_COLS; j++) {
                        if (grid[i][j] == -2) grid[i][j] = 0;
                        else if (grid[i][j] != 1 && grid[i][j] != 2 && grid[i][j] != 4) {
                            grid[i][j] = 5;
                        }
                    }
                }
                
                awardPointsP2(capturedBefore);
            }
            // Hit opponent trail - player dies
            else if (cellValue == 2) {
                player2.isAlive = false;
            }
            // Hit own trail - player dies
            else if (cellValue == 3) {
                player2.isAlive = false;
            }
            // Hit own captured tile - one tile per press, no trail
            /*else if (cellValue == 5) {
                player2.x = newX;
                player2.y = newY;
                // Don't leave trail on captured tiles
                // Reset movement flag to require new key press
                p2WasMoving = false;
            }*/
            // Empty space - move and leave trail (continuous movement)
            else if (cellValue == 0) {
                player2.x = newX;
                player2.y = newY;
                grid[newY][newX] = 3;
                player2.isConstructing = true;
            }
        }
        
        // Check collisions
        checkCollisions();
        
        // Move enemies
        if (!player1.frozen && !player2.frozen) {
            for (int i = 0; i < enemyCount; i++) {
                enemies[i].move(grid, MP_TILE_SIZE);
            }
        }
        
        // Check game over
        checkGameOver();
        
        moveTimer = 0;
    }
}

// ...existing code...

void MultiplayerScreen::drawGameBoard(sf::RenderWindow& window) {
    for (int i = 0; i < MP_GRID_ROWS; i++) {
        for (int j = 0; j < MP_GRID_COLS; j++) {
            if (grid[i][j] == 0) continue;
            
            if (grid[i][j] == 1) {
                tileSpriteSheet.setTextureRect(sf::IntRect(0, 0, MP_TILE_SIZE, MP_TILE_SIZE));
                tileSpriteSheet.setColor(sf::Color::White);
            } else if (grid[i][j] == 2) {
                tileSpriteSheet.setTextureRect(sf::IntRect(54, 0, MP_TILE_SIZE, MP_TILE_SIZE));
                tileSpriteSheet.setColor(sf::Color(100, 200, 255));  // Light blue trail (Player 1)
            } else if (grid[i][j] == 3) {
                tileSpriteSheet.setTextureRect(sf::IntRect(54, 0, MP_TILE_SIZE, MP_TILE_SIZE));
                tileSpriteSheet.setColor(sf::Color(255, 100, 100));  // Light red trail (Player 2)
            } else if (grid[i][j] == 4) {
                tileSpriteSheet.setTextureRect(sf::IntRect(72, 0, MP_TILE_SIZE, MP_TILE_SIZE));
                tileSpriteSheet.setColor(sf::Color(0, 100, 255));  // Blue captured (Player 1)
            } else if (grid[i][j] == 5) {
                tileSpriteSheet.setTextureRect(sf::IntRect(72, 0, MP_TILE_SIZE, MP_TILE_SIZE));
                tileSpriteSheet.setColor(sf::Color(255, 50, 50));  // Red captured (Player 2)
            }
            
            tileSpriteSheet.setPosition(MP_BOARD_X + j * MP_TILE_SIZE, MP_BOARD_Y + i * MP_TILE_SIZE);
            window.draw(tileSpriteSheet);
        }
    }
    tileSpriteSheet.setColor(sf::Color::White);
}

void MultiplayerScreen::drawPlayers(sf::RenderWindow& window) {
    if (player1.isAlive) {
        tileSpriteSheet.setTextureRect(sf::IntRect(36, 0, MP_TILE_SIZE, MP_TILE_SIZE));
        tileSpriteSheet.setColor(player1.color);
        tileSpriteSheet.setPosition(MP_BOARD_X + player1.x * MP_TILE_SIZE, MP_BOARD_Y + player1.y * MP_TILE_SIZE);
        window.draw(tileSpriteSheet);
    }

    if (player2.isAlive) {
        tileSpriteSheet.setTextureRect(sf::IntRect(36, 0, MP_TILE_SIZE, MP_TILE_SIZE));
        tileSpriteSheet.setColor(player2.color);
        tileSpriteSheet.setPosition(MP_BOARD_X + player2.x * MP_TILE_SIZE, MP_BOARD_Y + player2.y * MP_TILE_SIZE);
        window.draw(tileSpriteSheet);
    }

    tileSpriteSheet.setColor(sf::Color::White);
}

void MultiplayerScreen::drawEnemies(sf::RenderWindow& window) {
    static float rotation = 0;
    rotation += 10;
    
    for (int i = 0; i < enemyCount; i++) {
        enemySprite.setPosition(MP_BOARD_X + enemies[i].x, MP_BOARD_Y + enemies[i].y);
        enemySprite.setRotation(rotation);
        window.draw(enemySprite);
    }
}

void MultiplayerScreen::drawUI(sf::RenderWindow& window, sf::Font& font) {
    int bottomY = MP_BOARD_Y + MP_BOARD_HEIGHT + 10;
    
    sf::Text p1Score(player1.username + " score: " + std::to_string(player1.score), font, 18);
    p1Score.setPosition(MP_BOARD_X, bottomY);
    p1Score.setFillColor(player1.color);
    
    sf::Text p1Power("P1 Power-ups: " + std::to_string(player1.powerUps), font, 16);
    p1Power.setPosition(MP_BOARD_X, bottomY + 25);
    p1Power.setFillColor(sf::Color::Yellow);
    
    std::string p1FreezeStatus = player1.frozen ? "FROZEN!" : "Normal";
    sf::Color p1FreezeColor = player1.frozen ? sf::Color::Magenta : sf::Color::Green;
    sf::Text p1Freeze("P1 Status: " + p1FreezeStatus, font, 14);
    p1Freeze.setPosition(MP_BOARD_X, bottomY + 43);
    p1Freeze.setFillColor(p1FreezeColor);
    
    sf::Text p2Score(player2.username + " score: " + std::to_string(player2.score), font, 18);
    p2Score.setPosition(MP_BOARD_X + MP_BOARD_WIDTH - 200, bottomY);
    p2Score.setFillColor(player2.color);
    
    sf::Text p2Power("P2 Power-ups: " + std::to_string(player2.powerUps), font, 16);
    p2Power.setPosition(MP_BOARD_X + MP_BOARD_WIDTH - 200, bottomY + 25);
    p2Power.setFillColor(sf::Color::Yellow);
    
    std::string p2FreezeStatus = player2.frozen ? "FROZEN!" : "Normal";
    sf::Color p2FreezeColor = player2.frozen ? sf::Color::Magenta : sf::Color::Green;
    sf::Text p2Freeze("P2 Status: " + p2FreezeStatus, font, 14);
    p2Freeze.setPosition(MP_BOARD_X + MP_BOARD_WIDTH - 200, bottomY + 43);
    p2Freeze.setFillColor(p2FreezeColor);
    
    sf::Text controls("P1: Arrows+RShift | P2: WASD+LShift | ESC: Menu", font, 14);
    controls.setPosition(MP_BOARD_X + (MP_BOARD_WIDTH - controls.getLocalBounds().width) / 2, bottomY + 50);
    controls.setFillColor(sf::Color::Cyan);
    
    window.draw(p1Score);
    window.draw(p1Power);
    window.draw(p1Freeze);
    window.draw(p2Score);
    window.draw(p2Power);
    window.draw(p2Freeze);
    window.draw(controls);
}

void MultiplayerScreen::drawWaitingScreen(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape overlay(sf::Vector2f(MP_BOARD_WIDTH, MP_BOARD_HEIGHT));
    overlay.setPosition(MP_BOARD_X, MP_BOARD_Y);
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);
    
    sf::Text title("MULTIPLAYER MODE", font, 40);
    title.setFillColor(sf::Color::Cyan);
    int titleX = MP_BOARD_X + (MP_BOARD_WIDTH - title.getLocalBounds().width) / 2;
    title.setPosition(titleX, MP_BOARD_Y + 50);
    
    sf::Text player1Text(player1.username, font, 24);
    player1Text.setFillColor(player1.color);
    player1Text.setPosition(MP_BOARD_X + 50, MP_BOARD_Y + 120);
    
    sf::Text player2Text(player2.username, font, 24);
    player2Text.setFillColor(player2.color);
    player2Text.setPosition(MP_BOARD_X + 50, MP_BOARD_Y + 160);
    
    sf::Text controls("CONTROLS:", font, 20);
    controls.setFillColor(sf::Color::Yellow);
    controls.setPosition(MP_BOARD_X + 50, MP_BOARD_Y + 220);
    
    sf::Text p1Controls("Player 1: Arrow Keys + RShift", font, 18);
    p1Controls.setFillColor(player1.color);
    p1Controls.setPosition(MP_BOARD_X + 70, MP_BOARD_Y + 250);
    
    sf::Text p2Controls("Player 2: WASD + LShift", font, 18);
    p2Controls.setFillColor(player2.color);
    p2Controls.setPosition(MP_BOARD_X + 70, MP_BOARD_Y + 280);
    
    sf::Text startPrompt("Press ENTER to Start Game", font, 24);
    startPrompt.setFillColor(sf::Color::Green);
    int promptX = MP_BOARD_X + (MP_BOARD_WIDTH - startPrompt.getLocalBounds().width) / 2;
    startPrompt.setPosition(promptX, MP_BOARD_Y + 350);
    
    window.draw(title);
    window.draw(player1Text);
    window.draw(player2Text);
    window.draw(controls);
    window.draw(p1Controls);
    window.draw(p2Controls);
    window.draw(startPrompt);
}

void MultiplayerScreen::drawGameOverScreen(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape overlay(sf::Vector2f(MP_BOARD_WIDTH, MP_BOARD_HEIGHT));
    overlay.setPosition(MP_BOARD_X, MP_BOARD_Y);
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);
    
    sf::Text gameOverText("GAME OVER", font, 50);
    gameOverText.setFillColor(sf::Color::Red);
    int gameOverX = MP_BOARD_X + (MP_BOARD_WIDTH - gameOverText.getLocalBounds().width) / 2;
    gameOverText.setPosition(gameOverX, MP_BOARD_Y + 50);
    
    std::string winnerText;
    sf::Color winnerColor;
    
    if (player1.score > player2.score) {
        winnerText = player1.username + " WINS!";
        winnerColor = player1.color;
    } else if (player2.score > player1.score) {
        winnerText = player2.username + " WINS!";
        winnerColor = player2.color;
    } else {
        winnerText = "DRAW GAME!";
        winnerColor = sf::Color::White;
    }
    
    sf::Text winnerDisplay(winnerText, font, 36);
    winnerDisplay.setFillColor(winnerColor);
    int winnerX = MP_BOARD_X + (MP_BOARD_WIDTH - winnerDisplay.getLocalBounds().width) / 2;
    winnerDisplay.setPosition(winnerX, MP_BOARD_Y + 120);
    
    sf::Text p1Score(player1.username + ": " + std::to_string(player1.score), font, 24);
    p1Score.setFillColor(player1.color);
    p1Score.setPosition(MP_BOARD_X + 100, MP_BOARD_Y + 180);
    
    sf::Text p2Score(player2.username + ": " + std::to_string(player2.score), font, 24);
    p2Score.setFillColor(player2.color);
    p2Score.setPosition(MP_BOARD_X + 100, MP_BOARD_Y + 220);
    
    sf::Text restartOption("Press R to Restart", font, 24);
    restartOption.setFillColor(sf::Color::Yellow);
    int restartX = MP_BOARD_X + (MP_BOARD_WIDTH - restartOption.getLocalBounds().width) / 2;
    restartOption.setPosition(restartX, MP_BOARD_Y + 280);
    
    sf::Text menuOption("Press M for Main Menu", font, 24);
    menuOption.setFillColor(sf::Color::Yellow);
    int menuX = MP_BOARD_X + (MP_BOARD_WIDTH - menuOption.getLocalBounds().width) / 2;
    menuOption.setPosition(menuX, MP_BOARD_Y + 320);
    
    window.draw(gameOverText);
    window.draw(winnerDisplay);
    window.draw(p1Score);
    window.draw(p2Score);
    window.draw(restartOption);
    window.draw(menuOption);
}

void MultiplayerScreen::draw(sf::RenderWindow& window, sf::Font& font) {
    // Note: window.clear() and window.display() are handled by the main Application class
    // to prevent double-clearing which causes flickering
    
    window.draw(backgroundSprite);
    
    drawGameBoard(window);
    drawPlayers(window);
    drawEnemies(window);
    drawUI(window, font);
    
    if (!gameStarted) {
        drawWaitingScreen(window, font);
    } else if (gameOver) {
        drawGameOverScreen(window, font);
    }
}