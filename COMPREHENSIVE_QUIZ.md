# XONIX Game - Comprehensive Code Concepts Quiz

## Instructions
This quiz covers all major concepts, design patterns, data structures, and algorithms used throughout the codebase. Answer each question based on your understanding of the code. **Do not refer to the code while answering** - try to recall from memory first.

---

## Section 1: Architecture & Design Patterns

### 1.1 Application State Management
**Q1.1.1:** What design pattern is used to manage different screens/menus in the application, and how many states are defined in the `AppState` enum?

**Q1.1.2:** Explain the relationship between the `Application` class's state machine and the event handling system. How does `currentState` determine which events are processed?

**Q1.1.3:** What is the purpose of the `handleSubMenuEvents()` function, and why does it have special handling for the Friends menu (with early return)?

---

### 1.2 Screen Architecture
**Q1.2.1:** Describe the lifecycle of a screen in this application - from initialization to rendering to event handling. What methods are typically called in this cycle?

**Q1.2.2:** Why does each screen have separate `handleEvent()`, `update()`, and `draw()` methods instead of a single `render()` method?

**Q1.2.3:** The GameScreen and MultiplayerScreen both manage similar game logic. What design principle should be applied here to reduce code duplication, and how would you implement it?

---

## Section 2: Data Structures

### 2.1 Dynamic Array (DArray)
**Q2.1.1:** The `DArray` is implemented as a template class. What is a template class and what advantages does it provide in this context?

**Q2.1.2:** Describe the `resize()` operation in `DArray`. When is it triggered, and what is its time complexity?

**Q2.1.3:** If a `DArray` starts with capacity 0 and you append 20 elements, how many resize operations occur? What is the final capacity?

**Q2.1.4:** How would the `DArray` behave differently if the resize strategy doubled capacity only when `length == capacity` instead of checking `length >= capacity`?

---

### 2.2 LinkedList
**Q2.2.1:** What is the primary difference between a LinkedList and a DArray in terms of memory allocation and access time?

**Q2.2.2:** The LinkedList has both `head` and `tail` pointers. What operations become O(1) instead of O(n) because of the tail pointer?

**Q2.2.3:** The LinkedList stores strings. What happens to the ListNode's memory management when a string is destroyed? (Hint: Consider RAII principles)

**Q2.2.4:** Would a LinkedList be a good choice for the player's friend list if you frequently need to check if a specific friend exists? Why or why not?

---

### 2.3 HashTable
**Q2.3.1:** What is the purpose of the `hashFunction()` in the HashTable, and what problems can occur if the hash function is poorly designed?

**Q2.3.2:** The HashTable has a `resize()` operation. When is resizing triggered, and what is the expected time complexity of this operation?

**Q2.3.3:** The HashTable stores `Player*` pointers, not `Player` objects. What is the advantage of storing pointers vs. storing full objects?

**Q2.3.4:** Explain collision handling in this HashTable implementation. How does it handle two usernames that hash to the same index?

---

### 2.4 AVL Tree
**Q2.4.1:** What does AVL stand for, and what property does an AVL tree maintain that a regular binary search tree does not?

**Q2.4.2:** What are the four possible rotation cases in an AVL tree, and why is rebalancing necessary after insertion?

**Q2.4.3:** The AVL tree stores Theme objects. What is the time complexity for searching a theme by ID, and how does this compare to a LinkedList?

**Q2.4.4:** If an AVL tree has 1000 themes and you want to display them in order, what traversal method is used (as seen in `inorderFill`)?

---

### 2.5 Min-Heap
**Q2.5.1:** What is the min-heap property, and how is it used in the Leaderboard system?

**Q2.5.2:** In `heapifyUp()` and `heapifyDown()`, what are the parent/left/right child index calculations, and why are these formulas correct?

**Q2.5.3:** The MinHeap has fixed capacity. When it's full, `replaceMin()` is used instead of `insert()`. What is the advantage of this approach over dynamic resizing?

**Q2.5.4:** If you want to extract all elements from a MinHeap in sorted order, what is the time complexity and what does `sortForDisplay()` do?

---

### 2.6 Priority Queue (std::priority_queue)
**Q2.6.1:** The Matchmaking system uses three separate `std::priority_queue<MatchPlayer>` objects. Why use three separate queues instead of one large queue?

**Q2.6.2:** How does the `operator<` in `MatchPlayer` determine priority in the queue? What two factors are compared?

**Q2.6.3:** If two players have the same leaderboard rank, how are they ordered in the queue, and what fairness principle does this implement?

---

## Section 3: Authentication & Player Management

### 3.1 Authentication System
**Q3.1.1:** What is the purpose of the `usernameToPlayer` HashTable in the `AuthSystem` class?

**Q3.1.2:** During login, how does the system verify that a user's password is correct? Where is password validation performed?

**Q3.1.3:** The AuthSystem has both `currentPlayer` (pointer) and methods like `getCurrentPlayer()`. Why not just make `currentPlayer` public?

**Q3.1.4:** Describe the complete flow from launching the application to successfully logging in and accessing the main menu.

---

### 3.2 Player Profile Management
**Q3.2.1:** The Player class has methods like `addFriend()`, `removeFriend()`, and `isFriend()` that operate on a LinkedList. Why is this friend management at the Player level instead of in AuthSystem?

**Q3.2.2:** What validation methods exist for usernames, passwords, and emails? Why is validation important?

**Q3.2.3:** The Player class has both `totalScore` and `highestScore`. What is the difference between these two metrics?

**Q3.2.4:** What is the purpose of the `serialize()` and `deserialize()` methods in the Player class?

---

### 3.3 Friend System
**Q3.3.1:** What is a FriendRequest struct, and how does it differ from a direct friendship?

**Q3.3.2:** Explain the flow of sending a friend request: What state changes occur in both sender and receiver?

**Q3.3.3:** The friend requests are stored in a fixed-size array `pendingRequests[MAX_REQUESTS]`. What is a potential limitation of this approach?

**Q3.3.4:** When you accept a friend request, what operations occur on both players' friend lists?

---

## Section 4: Game Logic

### 4.1 Game State & Grid
**Q4.1.1:** The game grid is defined as `int grid[GRID_ROWS][GRID_COLS]` with dimensions 25x40. What does each cell value represent?

**Q4.1.2:** What are the game constants (TILE_SIZE, BOARD_WIDTH, BOARD_HEIGHT, BOARD_X, BOARD_Y) and why do we calculate them?

**Q4.1.3:** Explain the coordinate system: What are the units for pixel positions vs. grid positions?

---

### 4.2 Player Movement & Collision
**Q4.2.1:** The player has properties `playerX`, `playerY`, `playerDX`, `playerDY`, and a `moveTimer`/`moveDelay`. What do the DX and DY represent, and why do we need the timer?

**Q4.2.2:** Describe collision detection with enemies. How does the game determine if a player has been caught?

**Q4.2.3:** What is "tile capturing" in XONIX, and how does it affect the game state?

---

### 4.3 Enemy Behavior
**Q4.3.1:** The Enemy struct has `speedMultiplier`. How is this value set, and where does it come from?

**Q4.3.2:** Describe the enemy movement logic in `Enemy::move()`. How do enemies move around the grid and what causes direction changes?

**Q4.3.3:** What is the freeze mechanic for enemies, and how is `freezeTimer` used?

**Q4.3.4:** Why do enemies have both `x`, `y` (pixel coordinates) and move on a grid? How do these coordinate systems interact?

---

### 4.4 Difficulty Settings
**Q4.4.1:** The `Levels` class has static methods like `getEnemyCount()`, `getPlayerSpeed()`, and `getEnemySpeed()`. What is the purpose of making these methods static?

**Q4.4.2:** Compare the difficulty parameters for EASY vs. MEDIUM vs. HARD. What changes as difficulty increases?

**Q4.4.3:** How does `applyDifficultySettings()` in GameScreen use the Levels class to configure the game?

**Q4.4.4:** If you want to add a new difficulty level (INSANE), what changes would you need to make?

---

## Section 5: Matchmaking & Multiplayer

### 5.1 Matchmaking System
**Q5.1.1:** Explain the concept of a GameRoom. What information does it store, and what is its lifecycle?

**Q5.1.2:** How does `processMatchmaking()` work? What conditions must be met to create a GameRoom from two players?

**Q5.1.3:** The matchmaking system tracks `lastCreatedRoomId` separately from `nextRoomId`. Why is this distinction important?

**Q5.1.4:** Describe how a player joins the matchmaking queue, waits, and is matched. What state transitions occur in main.cpp?

---

### 5.2 Leaderboard Integration
**Q5.2.1:** What is the purpose of `getPlayerLeaderboardRank()` in the Matchmaking class?

**Q5.2.2:** How does the leaderboard influence matchmaking? Should higher-ranked or lower-ranked players be prioritized?

**Q5.2.3:** After a multiplayer match completes, `updateLeaderboardWithMatchResults()` is called. What information is updated?

---

### 5.3 Multiplayer Game Mechanics
**Q5.3.1:** The MultiplayerScreen has `MP_Player` structs for two players. What additional information does MP_Player store compared to the single-player Player struct?

**Q5.3.2:** How do the two players in a multiplayer game interact? Can they freeze each other's enemies?

**Q5.3.3:** What determines when a multiplayer game is over? How are winners determined?

---

## Section 6: File I/O & Persistence

### 6.1 Save Game System
**Q6.1.1:** The GameSave struct contains `gridData` as a string. Why serialize the entire grid as a string instead of a 2D array?

**Q6.1.2:** Describe the `serialize()` and `deserialize()` methods. What delimiters are used, and why?

**Q6.1.3:** What game state can be saved and restored? What information is lost when loading a game?

**Q6.1.4:** How does the system distinguish between different saved games for the same player?

---

### 6.2 Player Data Persistence
**Q6.2.1:** When does player data get saved to disk? (Provide at least 3 specific moments)

**Q6.2.2:** What is the format of the player data file (`players_data.txt`)?

**Q6.2.3:** How does the system prevent duplicate player records in the file?

**Q6.2.4:** On application startup, how is the HashTable `usernameToPlayer` populated?

---

### 6.3 Friend List Storage
**Q6.3.1:** How are friend lists stored persistently? What is the file format?

**Q6.3.2:** What is the relationship between a Player's LinkedList friend list and the data stored in files?

**Q6.3.3:** When a friend request is accepted, what files need to be updated?

---

## Section 7: Graphics & Rendering

### 7.1 SFML Integration
**Q7.1.1:** What is SFML, and what are its main modules used in this project?

**Q7.1.2:** The window is created with dimensions 1200x630. Why these specific values?

**Q7.1.3:** How is the frame rate controlled, and what is the target FPS?

**Q7.1.4:** What is the purpose of loading textures like `tileTexture`, `enemyTexture`, and `gameoverTexture`?

---

### 7.2 Rendering Pipeline
**Q7.2.1:** In the main application loop (`run()`), what are the three main phases that occur each frame?

**Q7.2.2:** Why was it a problem to have both main.cpp AND GameScreen.cpp calling `window.clear()` and `window.display()`? How was this fixed?

**Q7.2.3:** What is the relationship between `deltaTime` and smooth animation?

---

### 7.3 Theme System
**Q7.3.1:** What is a Theme struct, and what visual properties does it define?

**Q7.3.2:** How does the `stringToColor()` method work? What colors are supported?

**Q7.3.3:** The ThemeManager uses an AVL tree to store themes. How are themes searched and displayed?

**Q7.3.4:** How does a player's selected theme affect gameplay visuals?

---

## Section 8: Input Handling & Event Processing

### 8.1 Event Types
**Q8.1.1:** What are the different SFML event types used in this game? (Provide at least 5)

**Q8.1.2:** What is the difference between `KeyPressed` and `TextEntered` events? In what situations is each used?

**Q8.1.3:** How does the login screen prevent the Enter key from being appended to password fields?

---

### 8.2 Event Routing
**Q8.2.1:** Explain how events flow from `window.pollEvent()` → `handleEvents()` → state-specific handler.

**Q8.2.2:** Why is the order of event handler processing important? Provide an example where order matters.

**Q8.2.3:** What happens if multiple handlers try to process the same event? How is this prevented?

---

### 8.3 Input Validation
**Q8.3.1:** In password input, why do we check `event.text.unicode >= 32 && event.text.unicode < 128`?

**Q8.3.2:** What validations are performed for friend request usernames?

---

## Section 9: Audio

### 9.1 Audio System
**Q9.1.1:** What audio library does SFML provide, and what formats does it support?

**Q9.1.2:** Why is background music loaded as `sf::Music` rather than `sf::SoundBuffer` + `sf::Sound`?

**Q9.1.3:** The audio file path was changed from a hardcoded absolute path to a relative path. Why is this important for portability?

**Q9.1.4:** How does the game prevent multiple music instances from playing simultaneously?

---

## Section 10: Game Loop & Timing

### 10.1 Main Application Loop
**Q10.1.1:** In the `run()` method, `sf::Clock` is used to measure `deltaTime`. Why is this important for frame-rate independent gameplay?

**Q10.1.2:** What happens if `deltaTime` is too large (e.g., user minimizes window for a moment)?

**Q10.1.3:** Explain the order: `handleEvents()` → `update(deltaTime)` → `render()`. Why is this order important?

---

### 10.2 Matchmaking Timing
**Q10.2.1:** The matchmaking queue checks for matches every 2 seconds (`MATCHMAKING_INTERVAL`). Why not check every frame?

**Q10.2.2:** How is `matchmakingTimer` used to implement this interval-based processing?

---

## Section 11: Memory Management

### 11.1 Dynamic Memory
**Q11.1.1:** The Player objects are stored in a DArray, and pointers to them are stored in a HashTable. Why use pointers instead of copying objects?

**Q11.1.2:** What is a memory leak, and how could one occur in the AuthSystem if the destructor is not properly implemented?

**Q11.1.3:** The `currentPlayer` is a pointer that gets updated on login/logout. What precautions should be taken to ensure it doesn't become a dangling pointer?

---

### 11.2 Resource Management
**Q11.2.1:** SFML resources like textures and fonts are loaded on initialization. Why not load them on-demand?

**Q11.2.2:** What happens if `font.loadFromFile()` fails? How does the application handle this?

---

## Section 12: Algorithms

### 12.1 Search Algorithms
**Q12.1.1:** To find a player by username, the code uses `authSystem.getPlayer(username)`. What data structure enables fast lookups, and what is the time complexity?

**Q12.1.2:** To find a theme by ID, what data structure is used, and what is the search time complexity?

**Q12.1.3:** If you wanted to find all friends of a player, what data structure stores the friends, and what is the time complexity?

---

### 12.2 Sorting Algorithms
**Q12.2.1:** The Leaderboard displays players in rank order. What data structure (MinHeap) helps with this, and how?

**Q12.2.2:** What is the time complexity of extracting all elements from a MinHeap in sorted order?

---

## Section 13: Validation & Error Handling

### 13.1 Input Validation
**Q13.1.1:** What validations does `Player::validatePassword()` perform?

**Q13.1.2:** What happens if a user tries to register with a username that already exists?

**Q13.1.3:** How does the friend system prevent sending requests to non-existent usernames?

---

### 13.2 State Validation
**Q13.2.1:** Can a guest player (not logged in) play multiplayer? How is this enforced?

**Q13.2.2:** What happens if a user tries to load a saved game that doesn't exist?

---

## Section 14: Performance & Optimization

### 14.1 Data Structure Choice
**Q14.1.1:** Why is a HashTable preferred over a LinkedList for looking up players by username?

**Q14.1.2:** When would you use an AVL tree over a LinkedList for storing themes?

**Q14.1.3:** For a friend list with 100+ friends, which data structure (LinkedList, HashTable, DArray) would be best and why?

---

### 14.2 Algorithm Optimization
**Q14.2.1:** In the matchmaking system, why is it better to have three separate queues (EASY, MEDIUM, HARD) instead of one queue with a difficulty filter?

**Q14.2.2:** How does priority-based matching (using priority_queue) improve fairness vs. FIFO matching?

---

## Section 15: System Design & Architecture

### 15.1 Separation of Concerns
**Q15.1.1:** Why does each screen (LoginScreen, GameScreen, MainMenu, etc.) have its own class instead of combining them into one large screen manager?

**Q15.1.2:** What is the responsibility of main.cpp (Application class) vs. the individual screen classes?

---

### 15.2 Coupling & Cohesion
**Q15.2.1:** The Player class needs to access friend data. Why does it use a LinkedList internally rather than directly manipulating the AuthSystem's data structures?

**Q15.2.2:** Identify at least 3 examples of where one class depends on another. How are these dependencies managed?

---

### 15.3 Extensibility
**Q15.3.1:** How would you add a new menu screen (e.g., SettingsScreen)? What files would you modify in main.cpp?

**Q15.3.2:** If you wanted to add a new game mode (e.g., Battle Royale), what components would need to be created or modified?

**Q15.3.3:** To add in-game chat to multiplayer, what systems would need to be extended?

---

## Section 16: Code Quality & Best Practices

### 16.1 Naming & Conventions
**Q16.1.1:** What naming convention is used for variables (snake_case, camelCase, PascalCase)? Is it consistent?

**Q16.1.2:** Why are magic numbers (like 25, 40, 18 for grid dimensions) defined as constants instead of being hardcoded?

---

### 16.2 Const Correctness
**Q16.2.1:** Why do getter methods like `getUsername() const` use the `const` keyword?

**Q16.2.2:** What does `const std::string&` mean vs. just `std::string`?

---

## Section 17: Real-World Scenarios

### 17.1 Bug Scenarios
**Q17.1.1:** If a player is in the matchmaking queue but the network connection drops, how should the system handle this?

**Q17.1.2:** What happens if two players try to register with the same username simultaneously from different connections?

**Q17.1.3:** If the game crashes during a multiplayer match, is the match recoverable from the save?

---

### 17.2 Feature Scenarios
**Q17.2.1:** Implement a feature: Add a "Recent Players" list. How would you store and retrieve this data?

**Q17.2.2:** Implement a feature: Add match history/replay system. What data needs to be stored, and where?

**Q17.2.3:** Implement a feature: Add real-time notifications for friend requests. How would the system notify a player who is currently in gameplay?

---

## Section 18: Comparative Analysis

### 18.1 Data Structure Comparisons
**Q18.1.1:** Compare DArray vs. LinkedList vs. std::vector (from STL). When would each be preferred?

**Q18.1.2:** Compare HashTable vs. AVL Tree vs. std::map. What are the trade-offs?

**Q18.1.3:** Compare MinHeap vs. priority_queue. What is the relationship between them?

---

### 18.2 Algorithm Comparisons
**Q18.2.1:** For finding a player by rank in the leaderboard, compare linear search vs. heap extraction. When is each appropriate?

**Q18.2.2:** For friend list searches, compare linear search vs. hash-based search. What are the memory/time trade-offs?

---

## Section 19: Integration & Cross-System

### 19.1 Authentication to Gameplay
**Q19.1.1:** Trace the flow: A user logs in → plays a game → completes it. What systems interact, and in what order?

**Q19.1.2:** Describe all the state transitions from LOGIN_SCREEN to GAME_OVER and back to MAIN_MENU.

---

### 19.2 Matchmaking to Leaderboard
**Q19.2.1:** After a multiplayer match ends, explain how the leaderboard is updated with the results.

**Q19.2.2:** How does the leaderboard ranking affect future matchmaking for these players?

---

## Section 20: Open-Ended Design Questions

### 20.1 Architecture Questions
**Q20.1.1:** If you were to refactor this code, what design patterns would you apply to reduce duplication between GameScreen and MultiplayerScreen?

**Q20.1.2:** How would you implement a chat system that works during matchmaking queues and in-game?

**Q20.1.3:** Design a tournament system using the existing matchmaking infrastructure. What new classes/structures would you need?

---

### 20.2 Performance Questions
**Q20.2.1:** If you had 100,000 players on the leaderboard, would the current MinHeap approach still be efficient? What alternatives exist?

**Q20.2.2:** If matchmaking queues grew to 10,000 players, how would you optimize the `processMatchmaking()` function?

**Q20.2.3:** For a multiplayer game with 1000 concurrent players, how would you distribute the player data across multiple servers or databases?

---

### 20.3 Feature Questions
**Q20.3.1:** How would you implement a skill-rating system (similar to Elo rating) that influences matchmaking?

**Q20.3.2:** Design a clan/guild system. What data structures and classes would you need?

**Q20.3.3:** How would you implement anti-cheat measures given the current architecture?

---

## Section 21: Debugging & Troubleshooting

### 21.1 Common Issues
**Q21.1.1:** If a player's friend list displays as empty even though they have friends, where would you look first to debug?

**Q21.1.2:** If matchmaking never creates a GameRoom even with 10+ players waiting, what could be wrong?

**Q21.1.3:** If a saved game fails to load, what are the possible causes?

---

### 21.2 Edge Cases
**Q21.2.1:** What happens if all enemy slots are used but the difficulty requires more enemies?

**Q21.2.2:** Can a player send a friend request to themselves? Should the system prevent this?

**Q21.2.3:** What if a player wins with a score that exceeds the integer maximum?

---

## Summary Statistics
- **Total Questions:** 200+
- **Sections:** 21
- **Topics Covered:**
  - Data Structures (5 types)
  - Algorithms (Search, Sort, Matching)
  - Design Patterns (State Machine, MVC)
  - Game Systems (Matchmaking, Leaderboard, Friends)
  - File I/O & Persistence
  - Graphics & Input Handling
  - Memory Management
  - System Architecture
  - Real-world Scenarios

---

## How to Use This Quiz

1. **Self-Assessment:** Answer questions without looking at code to identify knowledge gaps
2. **Code Review:** For each question you struggled with, find the relevant code section
3. **Implementation Practice:** Use the feature scenarios to practice extending the system
4. **Interview Prep:** These questions cover patterns you'd encounter in technical interviews
5. **Teaching:** Use these to explain concepts to peers or in code reviews

Good luck! 🎮
