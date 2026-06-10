#pragma once
#include "Theme.h"
#include "AVLTREE.h"
#include <fstream>

class ThemeManager {
private:
    AVLTree themeTree;
    Theme currentTheme;
    static const int MAX_THEMES = 50;
    
    void initializeDefaultThemes();

public:
    ThemeManager();
    
    void saveUserTheme();
    void loadUserTheme();
    void displayThemeMenu();
    void browseThemes();
    void searchThemeById();
    void searchThemeByName();
    void applyTheme();
    void setThemeById(int themeId);  // Set theme by ID (used by UI)
    void showCurrentTheme();
    void runThemeMenu();
    
    const Theme& getCurrentTheme() const;
    
    Theme getThemeById(int themeId);
    void listThemes(Theme* arr, int maxSize, int& count);
};

