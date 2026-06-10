# Theme System - Quick Reference

## 🎨 What Changed?

**Themes now work end-to-end:** When you select a theme → It saves → When you play a game → The theme's background appears!

---

## 📋 Changes Made

| File | Changes | Purpose |
|------|---------|---------|
| `GameScreen.h` | Added `setBackgroundImage()` method | Allow dynamic background loading |
| `GameScreen.cpp` | Implemented `setBackgroundImage()` | Load theme background from file path |
| `ThemeManager.h` | Added `setThemeById()` method | Switch theme by ID |
| `ThemeManager.cpp` | Enhanced `applyTheme()` | Removed user input, just apply current theme |
| `ThemesScreen.cpp` | Enhanced `handleInput()` | Save theme ID and apply background |
| `main.cpp` | Enhanced `startGame()` | Apply selected theme background to game |

---

## 🔄 How It Works

### Selecting a Theme
```
1. Player presses UP/DOWN in Themes menu
2. Player presses ENTER to select
3. ThemesScreen calls:
   - manager->setThemeById(id)      ✓ Changes current theme
   - player->setCurrentThemeId(id)  ✓ Saves to player profile
   - manager->applyTheme()          ✓ Persists theme to disk
```

### Playing with Theme
```
1. Player selects difficulty and starts game
2. main.cpp startGame() function:
   - Gets current theme from ThemeManager
   - Calls gameScreen.setBackgroundImage(imagePath)
3. GameScreen loads the background image
4. Game renders with themed background
```

---

## 🎮 Available Themes

| Theme | ID | Colors | Background |
|-------|----|---------| ----------- |
| Classic | 1 | Blue/Red | Beautiful Mountains |
| Forest | 2 | Green/Brown | Fantasy Anime Scene |
| Ocean | 3 | Cyan/Orange | Futuristic Moon |
| Neon | 4 | Magenta/Yellow | Fantasy Endless Hole |

---

## 📁 File Structure

```
Theme System Components:
├── ThemeManager
│   ├── themeTree (AVL Tree of all themes)
│   ├── currentTheme (active theme)
│   └── user_theme.dat (persisted preference)
│
├── Theme struct
│   ├── id (unique identifier)
│   ├── name (display name)
│   ├── colors (player, enemy, trail, background, line)
│   └── imagePath (background image file)
│
└── GameScreen
    ├── backgroundTexture (loaded from theme)
    ├── backgroundSprite (rendered to screen)
    └── setBackgroundImage() (loads new theme)
```

---

## 🔧 How to Add a New Theme

### Step 1: Add Image File
Place your background image in `../images/` directory:
- Example: `../images/my_awesome_background.jpg`

### Step 2: Add to ThemeManager
Edit `theme_manager.cpp` in `initializeDefaultThemes()`:

```cpp
themeTree.insert(Theme(
    5,                                      // Unique ID
    "My Theme",                            // Display name
    "blue",                                // Player color
    "red",                                 // Enemy color  
    "lightblue",                           // Trail color
    "black",                               // Background color
    "white",                               // Line color
    "../images/my_awesome_background.jpg" // Image path
));
```

### Step 3: Recompile
```bash
cd build
make -j4
./xonix
```

### Step 4: Test
1. Open Themes menu
2. Your theme should appear at the bottom
3. Select it with ENTER
4. Start a game and see your background!

---

## 💾 Theme Persistence

**Where themes are saved:**
- `user_theme.dat` - Binary file storing current theme ID
- `players_data.txt` - Player profile includes `currentThemeId`

**What happens on startup:**
- ThemeManager loads `user_theme.dat`
- If file exists, applies saved theme
- If not, defaults to Classic (ID: 1)

**What happens on game start:**
- main.cpp gets current theme
- Calls `gameScreen.setBackgroundImage(imagePath)`
- Background loads before game starts

---

## ⚠️ Error Handling

**If theme image fails to load:**
- GameScreen attempts fallback: `../images/beautiful-mountains-landscape.jpg`
- If fallback fails, keeps previous background
- Game continues normally

**If invalid theme ID:**
- `setThemeById()` searches AVL tree
- If not found, keeps current theme
- No error shown (graceful handling)

---

## 🧪 Testing Scenarios

### Scenario 1: New Player
```
1. Launch game → Default theme is Classic
2. Go to Themes → See all 4 themes
3. Select Forest → Background changes
4. Play game → Enjoy forest background
```

### Scenario 2: Returning Player
```
1. Previous session: Selected Ocean theme
2. Launch game → Ocean theme auto-loads
3. Play game → Ocean background appears
4. Switch theme to Neon
5. Close and reopen → Neon theme persists
```

### Scenario 3: Missing Image
```
1. Theme points to non-existent image
2. Game attempts to load → Fails
3. Game uses fallback beautiful-mountains
4. Player sees default background
5. No crash, game continues
```

---

## 📊 Code Flow Diagram

```
┌─────────────────────────────────────┐
│      Player Selects Theme           │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│   ThemesScreen::handleInput()        │
│   - setThemeById(id)                │
│   - setCurrentThemeId(id)           │
│   - applyTheme()                    │
└──────────────┬──────────────────────┘
               │
               ▼ (saved to user_theme.dat)
               │
┌─────────────────────────────────────┐
│      Application Main Menu          │
│   Player clicks "Start Game"        │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│    main.cpp::startGame()            │
│   - getCurrentTheme()               │
│   - setBackgroundImage(imagePath)   │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│   GameScreen::setBackgroundImage()  │
│   - Load texture from imagePath     │
│   - Update backgroundSprite         │
│   - Position and scale              │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│    GameScreen::draw()               │
│   - Render themed background        │
│   - Render game elements            │
│   - Display to window               │
└─────────────────────────────────────┘
```

---

## 🎯 Key Functions

### Apply Theme in Game
**When**: Player starts a game
**Where**: `main.cpp::startGame()`
```cpp
Theme currentTheme = themeManager.getCurrentTheme();
if (currentTheme.id != 0 && !currentTheme.imagePath.empty()) {
    gameScreen.setBackgroundImage(currentTheme.imagePath);
}
```

### Load Theme Background
**When**: Theme applied or game starts
**Where**: `GameScreen::setBackgroundImage()`
```cpp
if (!backgroundTexture.loadFromFile(imagePath)) {
    // Fallback to default
    backgroundTexture.loadFromFile("../images/beautiful-mountains-landscape.jpg");
}
backgroundSprite.setTexture(backgroundTexture);
```

### Save Theme Selection
**When**: Player presses ENTER in Themes menu
**Where**: `ThemesScreen::handleInput()`
```cpp
manager->setThemeById(arr[selected].id);
currentPlayer->setCurrentThemeId(arr[selected].id);
manager->applyTheme();  // Saves to user_theme.dat
```

---

## 🚀 Usage Examples

### Example 1: Play with Forest Theme
```
1. Launch XONIX
2. Press 6 (THEMES from main menu)
3. Press DOWN arrow to Forest
4. Press ENTER
5. Go back to main menu
6. Press 1 (START GAME)
7. Select EASY, MEDIUM, or HARD
8. Forest background appears! 🌲
```

### Example 2: Create Ocean Theme Game Session
```
1. From main menu press 6 (THEMES)
2. Press DOWN twice to Ocean
3. Press ENTER
4. Return to main menu
5. Press 1 (START GAME)
6. Select HARD difficulty
7. Play with Ocean theme background 🌊
8. Close game → Reopen → Ocean theme persists
```

### Example 3: Switch Between Themes
```
1. Currently using Classic theme
2. Go to THEMES menu
3. Select Neon theme → Save
4. Play a game (see Neon background)
5. Finish game, return to menu
6. Go to THEMES menu
7. Select Forest theme → Save
8. Play another game (see Forest background)
```

---

## 📝 Notes

- **Performance**: Minimal - background loads once per game start
- **Memory**: Each theme ~1-5MB (just one texture in memory at a time)
- **File Size**: `user_theme.dat` is binary, very small (~10 bytes)
- **Compatibility**: Themes persist across game sessions
- **Future**: Could expand to theme colors for game elements, not just background

