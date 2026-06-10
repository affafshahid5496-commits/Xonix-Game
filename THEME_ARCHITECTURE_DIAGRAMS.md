# Theme System - Visual Architecture

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         XONIX Application                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │               Theme Management System                    │  │
│  ├──────────────────────────────────────────────────────────┤  │
│  │                                                          │  │
│  │  ┌─────────────────┐      ┌─────────────────────────┐   │  │
│  │  │  ThemeManager   │      │   Available Themes      │   │  │
│  │  ├─────────────────┤      ├─────────────────────────┤   │  │
│  │  │ • currentTheme  │──→   │ 1. Classic              │   │  │
│  │  │ • themeTree     │      │ 2. Forest               │   │  │
│  │  │ • loadUserTheme │      │ 3. Ocean                │   │  │
│  │  │ • saveUserTheme │      │ 4. Neon                 │   │  │
│  │  │ • applyTheme    │      │                         │   │  │
│  │  │ • setThemeById  │      │ Each theme has:         │   │  │
│  │  └────────┬────────┘      │ • ID                    │   │  │
│  │           │               │ • Name                  │   │  │
│  │           │               │ • Colors (7 properties) │   │  │
│  │           │               │ • Image path            │   │  │
│  │           ▼               └─────────────────────────┘   │  │
│  │      user_theme.dat                                     │  │
│  │    (Binary theme ID)                                    │  │
│  │                                                          │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                 │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │            Game Screen Theme Application                │  │
│  ├──────────────────────────────────────────────────────────┤  │
│  │                                                          │  │
│  │  ┌─────────────────┐      ┌─────────────────────────┐   │  │
│  │  │   GameScreen    │←──┤  │ ThemeManager::          │   │  │
│  │  ├─────────────────┤   │  │ getCurrentTheme()       │   │  │
│  │  │ setBackground   │───┘  └─────────────────────────┘   │  │
│  │  │ Image()         │                                      │  │
│  │  ├─────────────────┤                                      │  │
│  │  │ background      │                                      │  │
│  │  │ Texture         │ ← Loaded from theme imagePath       │  │
│  │  │ background      │                                      │  │
│  │  │ Sprite          │ ← Positioned & scaled               │  │
│  │  └────────┬────────┘                                      │  │
│  │           │                                              │  │
│  │           ▼                                              │  │
│  │      draw() method                                       │  │
│  │      Renders theme background in game                   │  │
│  │                                                          │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## 🔄 Data Flow Diagram

```
╔════════════════════════════════════════════════════════════════╗
║                      THEME SELECTION FLOW                      ║
╚════════════════════════════════════════════════════════════════╝

     ┌─────────────────────┐
     │  ThemesScreen       │
     │  (User Interface)   │
     └──────────┬──────────┘
                │
                │ handleInput(KeyPress::Enter)
                │
                ▼
     ┌─────────────────────────────────────────────┐
     │ manager->setThemeById(selectedThemeId)      │
     │ • Search AVL tree for theme                 │
     │ • Update ThemeManager.currentTheme          │
     └──────────┬──────────────────────────────────┘
                │
                ▼
     ┌─────────────────────────────────────────────┐
     │ player->setCurrentThemeId(themeId)          │
     │ • Update Player.currentThemeId              │
     │ • Saves to players_data.txt                 │
     └──────────┬──────────────────────────────────┘
                │
                ▼
     ┌─────────────────────────────────────────────┐
     │ manager->applyTheme()                       │
     │ • Write currentTheme.id to user_theme.dat   │
     │ • Display success message                   │
     └──────────┬──────────────────────────────────┘
                │
                ▼
     ┌──────────────────────┐
     │ Theme Applied! ✓     │
     │ (Saved to Disk)      │
     └──────────────────────┘


╔════════════════════════════════════════════════════════════════╗
║                       GAME START FLOW                          ║
╚════════════════════════════════════════════════════════════════╝

     ┌──────────────────────┐
     │ main.cpp::startGame()│
     └──────────┬───────────┘
                │
                ├─ gameScreen.initialize()
                │
                ▼
     ┌─────────────────────────────────────────────┐
     │ themeManager.getCurrentTheme()              │
     │ • Read user_theme.dat                       │
     │ • Search AVL tree                           │
     │ • Return Theme object with imagePath        │
     └──────────┬──────────────────────────────────┘
                │
                ▼
     ┌─────────────────────────────────────────────┐
     │ if (theme.id != 0 && !imagePath.empty())    │
     │ gameScreen.setBackgroundImage(imagePath)    │
     └──────────┬──────────────────────────────────┘
                │
                ▼
     ┌─────────────────────────────────────────────┐
     │ GameScreen::setBackgroundImage()            │
     │ • Load texture from imagePath               │
     │ • On error: Load fallback image             │
     │ • Update backgroundSprite                   │
     │ • Position and scale sprite                 │
     └──────────┬──────────────────────────────────┘
                │
                ▼
     ┌──────────────────────┐
     │ Background Ready! ✓  │
     │ (In Memory & Ready)  │
     └──────────┬───────────┘
                │
                ▼
     ┌──────────────────────┐
     │ Game Rendering Loop  │
     │ (60 FPS)             │
     └──────────┬───────────┘
                │
                ├─ gameScreen.update(deltaTime)
                │
                ├─ gameScreen.draw(window, font)
                │  ├─ Draw backgroundSprite ← Theme Background!
                │  ├─ Draw game board
                │  ├─ Draw player
                │  ├─ Draw enemies
                │  └─ Draw UI
                │
                ▼
     ┌──────────────────────┐
     │ Theme Game Live! 🎮 │
     │ Beautiful Background │
     └──────────────────────┘
```

---

## 🎨 Class Relationships

```
┌──────────────────────────────────┐
│          Application             │
│  (main.cpp - Orchestrator)       │
└────────────┬─────────────────────┘
             │
             ├─ owns ──────┐
             │             │
             ▼             ▼
    ┌──────────────┐  ┌──────────────────┐
    │ ThemeManager │  │   GameScreen     │
    ├──────────────┤  ├──────────────────┤
    │ • themeTree  │  │ • background     │
    │ • currentT   │  │   Texture        │
    │   Theme      │  │ • background     │
    │ • methods    │  │   Sprite         │
    └──────┬───────┘  └────────┬─────────┘
           │                   │
           │ setThemeById()    │ setBackgroundImage()
           │              ┌────┘
           │              │
           ▼              ▼
    ┌───────────────────────────────┐
    │     Theme (struct)            │
    ├───────────────────────────────┤
    │ • id          (int)           │
    │ • name        (string)        │
    │ • playerColor (string)        │
    │ • enemyColor  (string)        │
    │ • trailColor  (string)        │
    │ • backgroundColor(string)     │
    │ • lineColor   (string)        │
    │ • imagePath   (string)        │
    └───────────────────────────────┘

    ┌───────────────────────────────┐
    │    ThemesScreen (UI)          │
    ├───────────────────────────────┤
    │ • manager (ThemeManager*)     │
    │ • currentPlayer (Player*)     │
    │ • selected (int - index)      │
    │ • handleInput()               │
    └───────────────────────────────┘
             │
             │ Calls manager->setThemeById()
             │            &
             │      player->setCurrentThemeId()
             │
             ▼
    ┌───────────────────────────────┐
    │     Player (Profile)          │
    ├───────────────────────────────┤
    │ • currentThemeId (int)        │
    │ • currentThemeId getter       │
    │ • currentThemeId setter       │
    └───────────────────────────────┘
```

---

## 💾 File Structure & Persistence

```
Project Root Directory
│
├── /build/
│   ├── xonix (executable)
│   └── CMakeFiles/
│
├── /images/
│   ├── beautiful-mountains-landscape.jpg (Theme 1: Classic)
│   ├── fantasy-scene-anime-style.jpg     (Theme 2: Forest)
│   ├── futuristic-moon-background.jpg    (Theme 3: Ocean)
│   └── fantasy-endless-hole-landscape.jpg(Theme 4: Neon)
│
├── /audio/
│   └── Sakura-Girl-Yay-chosic.com_.wav
│
├── Theme System Source Files:
│   ├── Theme.h               (Theme struct definition)
│   ├── theme_manager.h       (ThemeManager class)
│   ├── theme_manager.cpp     (Implementation)
│   ├── ThemesScreen.h        (UI for theme selection)
│   └── ThemesScreen.cpp      (Implementation)
│
├── Game Source Files:
│   ├── GameScreen.h          (Modified: added setBackgroundImage)
│   ├── GameScreen.cpp        (Modified: implemented setBackgroundImage)
│   └── main.cpp              (Modified: apply theme on startGame)
│
├── Data Files (Generated at Runtime):
│   ├── user_theme.dat        (Binary file: current theme ID)
│   ├── players_data.txt      (Contains player profiles + currentThemeId)
│   └── friend_requests.txt
│
└── Documentation:
    ├── THEME_SYSTEM_GUIDE.md
    ├── THEME_QUICK_REFERENCE.md
    └── THEME_IMPLEMENTATION_SUMMARY.md
```

---

## 🔄 Theme Lifecycle

```
┌─────────────────────────────────────────────────────────────────┐
│                    THEME LIFECYCLE                              │
└─────────────────────────────────────────────────────────────────┘

    PHASE 1: INITIALIZATION
    ─────────────────────────
    Application Startup
         │
         ├─ ThemeManager constructor
         │  ├─ initializeDefaultThemes()  ← Load 4 built-in themes
         │  │  └─ Insert into AVL tree
         │  │
         │  └─ loadUserTheme()            ← Load saved theme
         │     ├─ Read user_theme.dat
         │     ├─ Search AVL tree
         │     └─ Set as currentTheme
         │
         ▼
    ThemeManager Ready with currentTheme loaded

    PHASE 2: USER INTERACTION
    ───────────────────────────
    User Opens Themes Menu
         │
         ├─ ThemesScreen displays all themes
         │  └─ User navigates with UP/DOWN
         │
         ├─ User presses ENTER
         │  ├─ manager->setThemeById()    ← Update currentTheme
         │  ├─ player->setCurrentThemeId()← Save to player profile
         │  ├─ manager->applyTheme()      ← Write to disk
         │  └─ Display success message
         │
         ▼
    Theme Selection Saved to Disk

    PHASE 3: GAME START
    ──────────────────────
    User Clicks START GAME
         │
         ├─ main.cpp startGame()
         │  ├─ gameScreen.initialize()
         │  ├─ Get currentTheme from manager
         │  └─ Call setBackgroundImage(imagePath)
         │
         ├─ GameScreen loads texture
         │  ├─ Load from imagePath
         │  ├─ On error: use fallback
         │  └─ Update backgroundSprite
         │
         ▼
    Game Ready with Themed Background

    PHASE 4: GAMEPLAY
    ────────────────────
    Game Loop (60 FPS)
         │
         ├─ handleEvents()
         ├─ update(deltaTime)
         │
         ├─ gameScreen.draw()
         │  ├─ Draw backgroundSprite  ← Theme Background!
         │  ├─ Draw grid
         │  ├─ Draw player
         │  ├─ Draw enemies
         │  └─ Draw UI
         │
         └─ window.display()
             ▼
         Game Renders with Beautiful Themed Background!

    PHASE 5: GAME END
    ──────────────────────
    Player Finishes Game
         │
         ├─ gameScreen.isGameOver() == true
         ├─ Display game over screen
         │  └─ Still showing themed background
         │
         ├─ Player presses keys
         │  └─ Return to main menu
         │
         ▼
    Theme Selection Persists (saved to disk)

    PHASE 6: SESSION END
    ──────────────────────
    Player Closes Game
         │
         ├─ Application destructor
         ├─ authSystem.saveSystemState()
         │  └─ Save player profiles with currentThemeId
         │
         ├─ user_theme.dat already saved
         │
         ▼
    Theme Preference Persisted for Next Session
```

---

## 🎯 Component Interaction Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│  HOW COMPONENTS WORK TOGETHER                                   │
└─────────────────────────────────────────────────────────────────┘

        User Input (Keyboard)
              │
              ▼
        ┌──────────────────┐
        │  ThemesScreen    │ ← Display themes with previews
        └────────┬─────────┘
                 │
        Enter key pressed
                 │
                 ├──────────────────────────────────┐
                 │                                  │
                 ▼                                  ▼
        ┌─────────────────────────┐    ┌──────────────────────┐
        │  ThemeManager           │    │  Player              │
        │  setThemeById(id)       │    │  setCurrentThemeId() │
        │  • Search AVL tree      │    │  • Update profile    │
        │  • Update currentTheme  │    │  • Mark as dirty     │
        └────────┬────────────────┘    └──────────────────────┘
                 │                              │
                 │ applyTheme()                 │
                 │ • Save to user_theme.dat     │
                 │                              ▼
                 │                    ┌─────────────────────┐
                 │                    │ players_data.txt    │
                 │                    │ (Persisted Disk)    │
                 │                    └─────────────────────┘
                 │
                 └──────────────────────────────────┐
                                                    │
                    (Later) User starts game        │
                                                    │
                                                    ▼
                                        ┌──────────────────────┐
                                        │ user_theme.dat       │
                                        │ (Theme ID persisted) │
                                        └────────┬─────────────┘
                                                 │
                                                 ▼
                                    ┌──────────────────────────┐
                                    │ main.cpp startGame()     │
                                    │ • Get currentTheme       │
                                    │ • Call setBackgroundImage
                                    └────────┬─────────────────┘
                                             │
                                             ▼
                                    ┌──────────────────────────┐
                                    │ GameScreen               │
                                    │ setBackgroundImage()     │
                                    │ • Load texture           │
                                    │ • Update sprite          │
                                    └────────┬─────────────────┘
                                             │
                                             ▼
                                    ┌──────────────────────────┐
                                    │ ../images/               │
                                    │ [theme-background.jpg]   │
                                    │ (Texture file loaded)    │
                                    └────────┬─────────────────┘
                                             │
                                             ▼
                                    ┌──────────────────────────┐
                                    │ GameScreen.draw()        │
                                    │ • Render backgroundSprite│
                                    │ • Render game elements   │
                                    └────────┬─────────────────┘
                                             │
                                             ▼
                                    ┌──────────────────────────┐
                                    │ Game Window              │
                                    │ Themed Background! ✨    │
                                    └──────────────────────────┘
```

---

## 🎯 State Diagram

```
┌────────────────────────────────────────────────────────────────┐
│              THEME SYSTEM STATE DIAGRAM                        │
└────────────────────────────────────────────────────────────────┘

        ┌──────────────┐
        │ Start-up     │
        │ DEFAULT      │
        │ Classic (ID1)│
        └────────┬─────┘
                 │
                 ├─ Load user_theme.dat?
                 │
        ┌────────┴──────────────┐
        │                       │
        YES                     NO
        │                       │
        ▼                       ▼
    ┌─────────┐        ┌──────────────┐
    │ LOADED  │        │ DEFAULT      │
    │ FROM    │        │ Classic (ID1)│
    │ DISK    │        └──────┬───────┘
    └────┬────┘               │
         │                    │
         └────────┬───────────┘
                  │
                  ▼
         ┌────────────────────┐
         │ currentTheme Ready │
         │ (In ThemeManager)  │
         └────────┬───────────┘
                  │
              User Opens Themes Menu
                  │
                  ▼
         ┌────────────────────┐
         │ VIEWING THEMES     │
         │ • Navigate UP/DOWN │
         │ • Show previews    │
         └────────┬───────────┘
                  │
             User Presses ENTER
                  │
                  ▼
         ┌────────────────────┐
         │ APPLYING THEME     │
         │ • setThemeById()   │
         │ • applyTheme()     │
         │ • Save to disk     │
         └────────┬───────────┘
                  │
                  ▼
         ┌────────────────────┐
         │ THEME SELECTED     │
         │ & PERSISTED        │
         └────────┬───────────┘
                  │
             User Starts Game
                  │
                  ▼
         ┌────────────────────┐
         │ APPLYING TO GAME   │
         │ • Get currentTheme │
         │ • LoadTexture      │
         │ • Update sprite    │
         └────────┬───────────┘
                  │
                  ▼
         ┌────────────────────┐
         │ THEME ACTIVE       │
         │ IN GAME            │
         │ Background renders │
         └────────────────────┘
```

---

## 📊 Memory Layout

```
┌─────────────────────────────────────────────────────────────────┐
│                  MEMORY ALLOCATION                              │
└─────────────────────────────────────────────────────────────────┘

    Stack Memory
    ┌──────────────────────────────────┐
    │ Application (main object)        │
    ├──────────────────────────────────┤
    │ • themeManager (ThemeManager)   │ ~5KB
    │ • gameScreen (GameScreen)       │ ~10KB
    │ • ... other screens            │
    └──────────────────────────────────┘

    Heap Memory
    ┌──────────────────────────────────┐
    │ ThemeManager.themeTree (AVL)    │ ~2KB (4 themes)
    │ ├─ Theme 1: Classic            │
    │ ├─ Theme 2: Forest             │
    │ ├─ Theme 3: Ocean              │
    │ └─ Theme 4: Neon               │
    └──────────────────────────────────┘
    
    GPU VRAM (Active)
    ┌──────────────────────────────────┐
    │ backgroundTexture                │ 1-5 MB
    │ (Currently loaded theme image)   │
    └──────────────────────────────────┘

    Disk Storage
    ┌──────────────────────────────────┐
    │ user_theme.dat                   │ 10 bytes
    │ (Binary: theme ID only)          │
    ├──────────────────────────────────┤
    │ players_data.txt                 │ ~1KB per player
    │ (Text: includes currentThemeId)  │
    ├──────────────────────────────────┤
    │ ../images/ [theme backgrounds]   │ 100KB-1MB each
    └──────────────────────────────────┘
```

---

This architecture ensures:
✅ **Clean separation of concerns** - Each class has single responsibility
✅ **Efficient memory usage** - Only active theme texture loaded
✅ **Persistent storage** - Theme choice saved and restored
✅ **Easy extensibility** - New themes added without code changes
✅ **Error resilience** - Graceful fallback on errors

