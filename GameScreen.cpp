#include "GameScreen.h"
#include "Leaderboard.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <iostream>

// Enemy implementation
Enemy::Enemy()
{
    x = (5 + rand() % (GRID_COLS - 10)) * TILE_SIZE;
    y = (5 + rand() % (GRID_ROWS - 10)) * TILE_SIZE;

    dx = 4 - rand() % 8;
    dy = 4 - rand() % 8;
    speedMultiplier = 1.0f; // Default speed
}

void Enemy::move(int grid[25][40], int tileSize)
{
    if (frozen)
        return;

    // Apply speed multiplier to movement
    x += (int)(dx * speedMultiplier);
    if (grid[y / tileSize][x / tileSize] == 1)
    {
        dx = -dx;
        x += (int)(dx * speedMultiplier);
    }

    y += (int)(dy * speedMultiplier);
    if (grid[y / tileSize][x / tileSize] == 1)
    {
        dy = -dy;
        y += (int)(dy * speedMultiplier);
    }
}

void Enemy::freeze()
{
    frozen = true;
    freezeTimer = 3.0f; // 3 seconds
}

void Enemy::unfreeze()
{
    frozen = false;
    freezeTimer = 0;
}

// GameScreen implementation
GameScreen::GameScreen()
    : score(0), powerUps(0), tilesCapturedThisMove(0), bonusCounter(0),
      gamePaused(false), gameOver(false), loadedFromSave(false), currentPlayer(nullptr), authSystem(nullptr),
      playerX(10), playerY(0), playerDX(0), playerDY(0), bonusOccurrences(0),
      moveTimer(0), moveDelay(0.07f), enemyCount(4), playerSpeed(0.07f), enemySpeed(4.0f),
      saveGameManager(nullptr), currentDifficulty(Levels::MEDIUM)
{

    initializeGrid();
    loadTextures();
    
    // Initialize save game manager
    saveGameManager = new SaveGameManager("saves");

    // Initialize enemies
    for (int i = 0; i < 10; i++)
    {
        enemies[i] = Enemy();
    }
}

GameScreen::~GameScreen()
{
    if (saveGameManager) {
        delete saveGameManager;
    }
}

bool GameScreen::loadTextures()
{
    if (!tileTexture.loadFromFile("images/tiles.png"))
    {
        std::cerr << "Error loading tiles texture" << std::endl;
        return false;
    }
    if (!enemyTexture.loadFromFile("images/enemy.png"))
    {
        std::cerr << "Error loading enemy texture" << std::endl;
        return false;
    }
    if (!gameoverTexture.loadFromFile("images/gameover.png"))
    {
        std::cerr << "Warning: Could not load gameover texture" << std::endl;
    }
    if (!backgroundTexture.loadFromFile("../images/beautiful-mountains-landscape.jpg"))
    {
        std::cerr << "Warning: Could not load background tree texture" << std::endl;
    }

    tileSpriteSheet.setTexture(tileTexture);
    enemySprite.setTexture(enemyTexture);
    gameoverSprite.setTexture(gameoverTexture);
    backgroundSprite.setTexture(backgroundTexture);

    // Position and scale background to fit the game board exactly
    backgroundSprite.setPosition(BOARD_X, BOARD_Y);
    backgroundSprite.setScale(
        (float)BOARD_WIDTH / backgroundTexture.getSize().x,
        (float)BOARD_HEIGHT / backgroundTexture.getSize().y);

    gameoverSprite.setPosition(BOARD_X, BOARD_Y);
    enemySprite.setOrigin(20, 20);

    // Load background music
    if (!backgroundMusic.openFromFile("audio/Sakura-Girl-Yay-chosic.com_.wav"))
    {
        std::cerr << "Warning: Could not load background music from audio/Sakura-Girl-Yay-chosic.com_.wav" << std::endl;
    }
    else
    {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50.0f);
        backgroundMusic.play();
        std::cout << "Background music loaded and playing" << std::endl;
    }

    return true;
}

void GameScreen::initialize(Player *player, AuthSystem* auth, Levels::Difficulty difficulty)
{
    currentPlayer = player;
    authSystem = auth;
    applyDifficultySettings(difficulty);
    reset();
}

void GameScreen::applyDifficultySettings(Levels::Difficulty difficulty)
{
    // Set enemy count based on difficulty
    enemyCount = Levels::getEnemyCount(difficulty);
    
    // Set player speed (move delay - lower delay = faster)
    playerSpeed = Levels::getPlayerSpeed(difficulty);
    moveDelay = playerSpeed;
    
    // Set enemy speed
    enemySpeed = Levels::getEnemySpeed(difficulty);
    
    // Calculate speed multiplier (normalized to medium = 1.0)
    float speedMultiplier = enemySpeed / 4.0f; // Medium difficulty has speed 4.0
    
    // Apply speed multiplier to all enemies
    for (int i = 0; i < 10; i++)
    {
        enemies[i].setSpeedMultiplier(speedMultiplier);
    }
    
    // Set initial power-ups
    powerUps = Levels::getInitialPowerUps(difficulty);
}

void GameScreen::setBackgroundImage(const std::string& imagePath)
{
    backgroundImagePath = imagePath;
    
    // Load the theme background texture
    if (!backgroundTexture.loadFromFile(imagePath))
    {
        std::cerr << "Warning: Could not load background image from: " << imagePath << std::endl;
        // Keep the old background if load fails
        if (!backgroundTexture.loadFromFile("../images/beautiful-mountains-landscape.jpg"))
        {
            std::cerr << "Warning: Could not load fallback background" << std::endl;
        }
    }
    
    // Update the sprite with the new texture
    backgroundSprite.setTexture(backgroundTexture);
    
    // Position and scale background to fit the game board exactly
    backgroundSprite.setPosition(BOARD_X, BOARD_Y);
    backgroundSprite.setScale(
        (float)BOARD_WIDTH / backgroundTexture.getSize().x,
        (float)BOARD_HEIGHT / backgroundTexture.getSize().y);
}

void GameScreen::reset()
{
    initializeGrid();
    score = 0;
    powerUps = 0;
    tilesCapturedThisMove = 0;
    bonusCounter = 0;
    gamePaused = false;
    gameOver = false;
    loadedFromSave = false;  // Reset flag when starting new game
    playerX = 10;
    playerY = 0;
    playerDX = 0;
    playerDY = 0;
    lastPlayerX = 10;
    lastPlayerY = 0;
    wallCollisionProcessed = false;
    moveTimer = 0;
    gameTimer = 0;  // Reset game timer

    for (int i = 0; i < enemyCount; i++)
    {
        enemies[i] = Enemy();
    }
}

void GameScreen::initializeGrid()
{
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            if (i == 0 || j == 0 || i == GRID_ROWS - 1 || j == GRID_COLS - 1)
            {
                grid[i][j] = 1; // Border
            }
            else
            {
                grid[i][j] = 0; // Empty
            }
        }
    }
}

void GameScreen::drop(int startY, int startX)
{
    // Iterative flood-fill using a queue instead of recursion to avoid lag
    if (grid[startY][startX] != 0) return;
    
    // Use a simple queue to avoid stack overflow and lag from deep recursion
    int queue[GRID_ROWS * GRID_COLS][2];
    int front = 0, rear = 0;
    
    queue[rear][0] = startY;
    queue[rear][1] = startX;
    rear++;
    
    grid[startY][startX] = -1;
    
    while (front < rear) {
        int y = queue[front][0];
        int x = queue[front][1];
        front++;
        
        // Check all 4 directions
        int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        
        for (int d = 0; d < 4; d++) {
            int ny = y + dirs[d][0];
            int nx = x + dirs[d][1];
            
            if (ny >= 0 && ny < GRID_ROWS && nx >= 0 && nx < GRID_COLS && grid[ny][nx] == 0) {
                grid[ny][nx] = -1;
                queue[rear][0] = ny;
                queue[rear][1] = nx;
                rear++;
            }
        }
    }
}

void GameScreen::updateScore(int tilesCaptured)
{
    if (tilesCaptured == 0)
        return;
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

    if (currentPlayer)
    {
        currentPlayer->updateScore(points);
    }
}

void GameScreen::usePowerUp()
{
    if (powerUps > 0)
    {
        powerUps--;

        for (int i = 0; i < enemyCount; i++)
        {
            enemies[i].freeze();
        }
    }
}

void GameScreen::checkGameOver()
{
    // Check collision with enemies
    for (int i = 0; i < enemyCount; i++)
    {
        if (grid[enemies[i].y / TILE_SIZE][enemies[i].x / TILE_SIZE] == 2)
        {
            gameOver = true;
        }
    }
}

void GameScreen::handleEvent(sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Left)
        {
            playerDX = -1;
            playerDY = 0;
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            playerDX = 1;
            playerDY = 0;
        }
        else if (event.key.code == sf::Keyboard::Up)
        {
            playerDX = 0;
            playerDY = -1;
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            playerDX = 0;
            playerDY = 1;
        }
        else if (event.key.code == sf::Keyboard::Space)
        {
            usePowerUp();
        }
    }
}
void GameScreen::update(float deltaTime)
{
    if (gameOver || gamePaused)
        return;

    moveTimer += deltaTime;

    // --- Move player ---
    if (moveTimer >= moveDelay)
    {
        playerX += playerDX;
        playerY += playerDY;

        // Clamp player inside grid
        if (playerX < 0)
            playerX = 0;
        if (playerX >= GRID_COLS)
            playerX = GRID_COLS - 1;
        if (playerY < 0)
            playerY = 0;
        if (playerY >= GRID_ROWS)
            playerY = GRID_ROWS - 1;

        // Collision with player path
        if (grid[playerY][playerX] == 2)
        {
            gameOver = true;
        }

        // Leave trail
        if (grid[playerY][playerX] == 0)
        {
            grid[playerY][playerX] = 2;
            tilesCapturedThisMove++;
        }

        moveTimer -= moveDelay;  // Subtract instead of reset for smoother timing
    }

    // --- Player hits wall (only process if player actually moved) ---
    if (grid[playerY][playerX] == 1 && (playerX != lastPlayerX || playerY != lastPlayerY))
    {
        playerDX = playerDY = 0;

        // Update score for captured tiles
        updateScore(tilesCapturedThisMove);
        tilesCapturedThisMove = 0;

        // Flood-fill from all enemies
        for (int i = 0; i < enemyCount; i++)
        {
            drop(enemies[i].y / TILE_SIZE, enemies[i].x / TILE_SIZE);
        }

        // Fill enclosed area: -1 = outside, 0 or 2 = inside
        for (int i = 0; i < GRID_ROWS; i++)
        {
            for (int j = 0; j < GRID_COLS; j++)
            {
                if (grid[i][j] == -1)
                    grid[i][j] = 0; // outside, stay empty
                else if (grid[i][j] == 0 || grid[i][j] == 2)
                    grid[i][j] = 1; // fill inside
            }
        }
    }
    
    // Update position tracking
    lastPlayerX = playerX;
    lastPlayerY = playerY;

    // --- Update game timer ---
    gameTimer += deltaTime;

    // --- Move enemies ---
    for (int i = 0; i < enemyCount; i++)
    {
        enemies[i].move(grid, TILE_SIZE);

        if (enemies[i].frozen)
        {
            enemies[i].freezeTimer -= deltaTime;
            if (enemies[i].freezeTimer <= 0)
                enemies[i].unfreeze();
        }
    }

    // --- Check if enemies touch player path ---
    checkGameOver();
}

void GameScreen::drawGameBoard(sf::RenderWindow &window)
{
    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            if (grid[i][j] == 0)
                continue;
            if (grid[i][j] == 1)
                tileSpriteSheet.setTextureRect(sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
            if (grid[i][j] == 2) {
                tileSpriteSheet.setTextureRect(sf::IntRect(54, 0, TILE_SIZE, TILE_SIZE));
                tileSpriteSheet.setColor(sf::Color(200, 200, 200));  // Lighter trail color
            } else {
                tileSpriteSheet.setColor(sf::Color::White);  // Reset to normal color
            }
            tileSpriteSheet.setPosition(BOARD_X + j * TILE_SIZE, BOARD_Y + i * TILE_SIZE);
            window.draw(tileSpriteSheet);
        }
    }
}

void GameScreen::drawPlayer(sf::RenderWindow &window)
{
    tileSpriteSheet.setTextureRect(sf::IntRect(36, 0, TILE_SIZE, TILE_SIZE));
    tileSpriteSheet.setPosition(BOARD_X + playerX * TILE_SIZE, BOARD_Y + playerY * TILE_SIZE);
    window.draw(tileSpriteSheet);
}

void GameScreen::drawEnemies(sf::RenderWindow &window)
{
    static float rotation = 0;
    rotation += 10;

    for (int i = 0; i < enemyCount; i++)
    {
        enemySprite.setPosition(BOARD_X + enemies[i].x, BOARD_Y + enemies[i].y);
        enemySprite.setRotation(rotation);
        window.draw(enemySprite);
    }
}

void GameScreen::drawUI(sf::RenderWindow& window, sf::Font& font) {
    // Draw timer at the top center of the game box
    int timerX = BOARD_X + BOARD_WIDTH / 2;
    int timerY = BOARD_Y - 40;
    
    int seconds = (int)gameTimer % 60;
    int minutes = (int)gameTimer / 60;
    
    char timerText[16];
    snprintf(timerText, sizeof(timerText), "%02d:%02d", minutes, seconds);
    
    sf::Text timerDisplay(timerText, font, 28);
    timerDisplay.setFillColor(sf::Color::White);
    timerDisplay.setStyle(sf::Text::Bold);
    
    // Center the text
    sf::FloatRect timerBounds = timerDisplay.getLocalBounds();
    timerDisplay.setPosition(timerX - timerBounds.width / 2, timerY);
    window.draw(timerDisplay);
    
    // Draw player profile on the left side of the game box
    int leftX = BOARD_X - 200;
    int topY = BOARD_Y + 70;
    
    if (currentPlayer) {
        // Player profile header
        sf::Text profileHeader("PLAYER PROFILE", font, 26);
        profileHeader.setPosition(leftX, topY);
        profileHeader.setFillColor(sf::Color::Yellow);
        window.draw(profileHeader);
        
        int currentY = topY + 30;
        
        // Username
        sf::Text usernameText("Username:", font, 22);
        usernameText.setPosition(leftX, currentY);
        window.draw(usernameText);
        
        sf::Text usernameValue(currentPlayer->getUsername(), font, 22);
        usernameValue.setPosition(leftX + 100, currentY);
        window.draw(usernameValue);
        
        // Nickname
        currentY += 25;
        sf::Text nicknameText("Nickname:", font, 22);
        nicknameText.setPosition(leftX, currentY);
        window.draw(nicknameText);
        
        sf::Text nicknameValue(currentPlayer->getNickname(), font, 22);
        nicknameValue.setPosition(leftX + 100, currentY);
        window.draw(nicknameValue);
        
        // Current Score
        currentY += 25;
        sf::Text scoreText("Current Score:", font, 22);
        scoreText.setPosition(leftX, currentY);
        window.draw(scoreText);
        
        sf::Text scoreValue(std::to_string(score), font, 22);
        scoreValue.setPosition(leftX + 130, currentY);
        window.draw(scoreValue);
        
        // Highest Score
        currentY += 25;
        int highestScore = currentPlayer->getHighestScore();
        sf::Text highestText("Highest Score:", font, 22);
        highestText.setPosition(leftX, currentY);
        window.draw(highestText);
        
        sf::Text highestValue(std::to_string(highestScore), font, 22);
        highestValue.setPosition(leftX + 130, currentY);
        window.draw(highestValue);
        
        // Power-ups
        currentY += 25;
        sf::Text powerUpText("Power-ups:", font, 22);
        powerUpText.setPosition(leftX, currentY);
        window.draw(powerUpText);
        
        sf::Text powerUpValue(std::to_string(powerUps), font, 22);
        powerUpValue.setPosition(leftX + 100, currentY);
        window.draw(powerUpValue);
        
        // Games Played
        currentY += 25;
        sf::Text gamesPlayedText("Games Played:", font, 22);
        gamesPlayedText.setPosition(leftX, currentY);
                window.draw(gamesPlayedText);
        
        sf::Text gamesPlayedValue(std::to_string(currentPlayer->getGamesPlayed()), font, 22);
        gamesPlayedValue.setPosition(leftX + 130, currentY);
        window.draw(gamesPlayedValue);
        
        // Games Won
        currentY += 25;
        sf::Text gamesWonText("Games Won:", font, 22);
        gamesWonText.setPosition(leftX, currentY);
        window.draw(gamesWonText);
        
        sf::Text gamesWonValue(std::to_string(currentPlayer->getGamesWon()), font, 22);
        gamesWonValue.setPosition(leftX + 100, currentY);
        window.draw(gamesWonValue);
        
        // Win Rate
        currentY += 25;
        sf::Text winRateText("Win Rate:", font, 22);
        winRateText.setPosition(leftX, currentY);
        window.draw(winRateText);
        
        float winRate = currentPlayer->getWinRate();
        std::string winRateStr = std::to_string((int)(winRate * 100)) + "%";
        sf::Text winRateValue(winRateStr, font, 22);
        winRateValue.setPosition(leftX + 100, currentY);
        window.draw(winRateValue);
        
        // Email
        currentY += 25;
        sf::Text emailText("Email:", font, 22);
        emailText.setPosition(leftX, currentY);
        window.draw(emailText);
        
        std::string emailDisplay = currentPlayer->getEmail().empty() ? "N/A" : currentPlayer->getEmail();
        sf::Text emailValue(emailDisplay, font, 20);
        emailValue.setPosition(leftX + 60, currentY+3);
        window.draw(emailValue);
        
        // Total Score
        currentY += 25;
        sf::Text totalScoreText("Total Score:", font, 22);
        totalScoreText.setPosition(leftX, currentY);
        window.draw(totalScoreText);
        
        sf::Text totalScoreValue(std::to_string(currentPlayer->getTotalScore()), font, 22);
        totalScoreValue.setPosition(leftX + 110, currentY);
        window.draw(totalScoreValue);
    }
    
    // Draw controls at the bottom
    int bottomY = BOARD_Y + BOARD_HEIGHT + 10;
    sf::Text controls("ESC: Menu  SPACE: Power-up", font, 22);
    controls.setPosition(BOARD_X + (BOARD_WIDTH - controls.getLocalBounds().width) / 2, bottomY);
    controls.setFillColor(sf::Color::Cyan);
    window.draw(controls);
}
void GameScreen::drawGameOverScreen(sf::RenderWindow& window, sf::Font& font) {
    sf::RectangleShape overlay(sf::Vector2f(BOARD_WIDTH, BOARD_HEIGHT));
    overlay.setPosition(BOARD_X, BOARD_Y);
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);
    
    sf::Text gameOverText("GAME OVER", font, 50);
    gameOverText.setFillColor(sf::Color::Red);
    int gameOverX = BOARD_X + (BOARD_WIDTH - gameOverText.getLocalBounds().width) / 2;
    int gameOverY = BOARD_Y + 50;
    gameOverText.setPosition(gameOverX, gameOverY);
    
    sf::Text finalScore("Final Score: " + std::to_string(score), font, 30);
    finalScore.setFillColor(sf::Color::White);
    int scoreX = BOARD_X + (BOARD_WIDTH - finalScore.getLocalBounds().width) / 2;
    int scoreY = BOARD_Y + 130;
    finalScore.setPosition(scoreX, scoreY);
    
    // Add personal best information
    if (currentPlayer) {
        int highest = currentPlayer->getHighestScore();
        bool isNewBest = (score > highest);
        
        sf::Text bestScore("Personal Best: " + std::to_string(highest), font, 24);
        bestScore.setFillColor(isNewBest ? sf::Color::Yellow : sf::Color::Cyan);
        int bestX = BOARD_X + (BOARD_WIDTH - bestScore.getLocalBounds().width) / 2;
        int bestY = BOARD_Y + 180;
        bestScore.setPosition(bestX, bestY);
        window.draw(bestScore);
        
        if (isNewBest) {
            sf::Text newRecord("NEW RECORD!", font, 28);
            newRecord.setFillColor(sf::Color::Yellow);
            int recordX = BOARD_X + (BOARD_WIDTH - newRecord.getLocalBounds().width) / 2;
            int recordY = BOARD_Y + 220;
            newRecord.setPosition(recordX, recordY);
            window.draw(newRecord);
        }
    }
    
    sf::Text restartOption("Press R to Restart", font, 24);
    restartOption.setFillColor(sf::Color::Yellow);
    int restartX = BOARD_X + (BOARD_WIDTH - restartOption.getLocalBounds().width) / 2;
    int restartY = BOARD_Y + (currentPlayer ? 270 : 220);
    restartOption.setPosition(restartX, restartY);
    
    sf::Text menuOption("Press M for Main Menu", font, 24);
    menuOption.setFillColor(sf::Color::Yellow);
    int menuX = BOARD_X + (BOARD_WIDTH - menuOption.getLocalBounds().width) / 2;
    int menuY = BOARD_Y + (currentPlayer ? 320 : 270);
    menuOption.setPosition(menuX, menuY);
    
    window.draw(gameOverText);
    window.draw(finalScore);
    window.draw(restartOption);
    window.draw(menuOption);
}

void GameScreen::draw(sf::RenderWindow &window, sf::Font &font)
{
    // Note: window.clear() and window.display() are handled by the main Application class
    // to prevent double-clearing which causes flickering

    // Draw background
    window.draw(backgroundSprite);

    drawGameBoard(window);
    drawPlayer(window);
    drawEnemies(window);
    drawUI(window, font);

    if (gameOver)
    {
        drawGameOverScreen(window, font);
    }
}

void GameScreen::onGameComplete(int finalScore)
{
    if (!currentPlayer)
        return;

    // Get current player
    int currentPlayerId = currentPlayer->getPlayerId();
    std::string currentPlayerUsername = currentPlayer->getUsername();

    // Update leaderboard with username
    Leaderboard leaderboard;
    leaderboard.updatePlayerScore(currentPlayerId, finalScore, currentPlayerUsername);

    // Update player's highest score if this is a new record
    bool isNewPersonalBest = (finalScore > currentPlayer->getHighestScore());
    if (isNewPersonalBest) {
        currentPlayer->updateHighestScore(finalScore);
        std::cout << "New Personal Best! Highest Score: " << finalScore << std::endl;
    }

    // Check if it's a global high score
    bool isTopGlobalScore = leaderboard.isTopPlayer(finalScore);
    if (isTopGlobalScore) {
        std::cout << "New High Score! You're on the leaderboard!" << std::endl;
    }

    // Increment games played
    int currentGames = currentPlayer->getGamesPlayed();
    currentPlayer->setGamesPlayed(currentGames + 1);

    // Update games won - assuming game completion counts as a win if score > 0
    // You can modify this logic based on your actual win conditions
    bool gameWon = (finalScore > currentPlayer->getHighestScore()); // Modify this condition as needed
    if (gameWon) {
        int currentWins = currentPlayer->getGamesWon();
        currentPlayer->setGamesWon(currentWins + 1);
    }

    // Save player data to file immediately after updates
    if (authSystem) {
        authSystem->savePlayerData(currentPlayer->getUsername());
        std::cout << "Player stats updated in players_data.txt" << std::endl;
        std::cout << "Games Played: " << currentPlayer->getGamesPlayed() 
                  << " | Games Won: " << currentPlayer->getGamesWon() 
                  << " | Highest Score: " << currentPlayer->getHighestScore() << std::endl;
    }
    
    // Delete the saved game if it was loaded from save
    if (loadedFromSave && saveGameManager) {
        saveGameManager->deleteSave(currentPlayerUsername);
        saveGameManager->deleteGameRecord(currentPlayerUsername);
        std::cout << "Saved game and record deleted after completion for " << currentPlayerUsername << std::endl;
    }
}

GameSave GameScreen::createGameSave() const {
    GameSave save;
    if (currentPlayer) {
        save.username = currentPlayer->getUsername();
    }
    save.score = score;
    save.playerX = playerX;
    save.playerY = playerY;
    save.playerDX = playerDX;
    save.playerDY = playerDY;
    save.powerUps = powerUps;
    save.enemyCount = enemyCount;
    save.difficulty = (int)currentDifficulty;
    
    // Save enemy positions
    for (int i = 0; i < 10; i++) {
        save.enemyX[i] = enemies[i].x;
        save.enemyY[i] = enemies[i].y;
    }
    
    // Serialize grid data (simplified - store as string of 0s and 1s)
    save.gridData = "";
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            save.gridData += (grid[i][j] == 0 ? '0' : '1');
        }
    }
    
    return save;
}

bool GameScreen::loadGameSave(const GameSave& save) {
    score = save.score;
    playerX = save.playerX;
    playerY = save.playerY;
    playerDX = save.playerDX;
    playerDY = save.playerDY;
    powerUps = save.powerUps;
    enemyCount = save.enemyCount;
    currentDifficulty = (Levels::Difficulty)save.difficulty;
    
    // Restore grid data
    if (save.gridData.length() == GRID_ROWS * GRID_COLS) {
        for (int i = 0; i < GRID_ROWS; i++) {
            for (int j = 0; j < GRID_COLS; j++) {
                grid[i][j] = (save.gridData[i * GRID_COLS + j] == '1' ? 1 : 0);
            }
        }
    }
    
    // Restore enemy positions
    for (int i = 0; i < 10; i++) {
        enemies[i].x = save.enemyX[i];
        enemies[i].y = save.enemyY[i];
    }
    
    // Reinitialize enemies with current difficulty settings
    applyDifficultySettings(currentDifficulty);
    
    gameOver = false;
    gamePaused = false;
    loadedFromSave = true;  // Mark that this game was loaded from save
    
    std::cout << "Game loaded for " << save.username << std::endl;
    return true;
}

bool GameScreen::saveCurrentGame() {
    if (!currentPlayer || !saveGameManager) {
        std::cout << "Error: Cannot save game. Player or SaveGameManager is null." << std::endl;
        return false;
    }
    
    GameSave save = createGameSave();
    return saveGameManager->saveGame(save);
}

bool GameScreen::loadSavedGame(const std::string& username) {
    if (!saveGameManager) {
        std::cout << "Error: SaveGameManager is null." << std::endl;
        return false;
    }
    
    GameSave save;
    if (saveGameManager->loadGame(username, save)) {
        return loadGameSave(save);
    }
    return false;
}

bool GameScreen::hasSavedGame(const std::string& username) const {
    if (!saveGameManager) return false;
    return saveGameManager->hasSave(username);
}

bool GameScreen::deleteSavedGame(const std::string& username) {
    if (!saveGameManager) {
        std::cout << "Error: SaveGameManager is null." << std::endl;
        return false;
    }
    return saveGameManager->deleteSave(username);
}

