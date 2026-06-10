#include "theme_manager.h"
#include <iostream>

void ThemeManager::initializeDefaultThemes() {
    // Classic theme
    themeTree.insert(Theme(1, "Classic", "blue", "red", "lightblue", "black", "white", "../images/beautiful-mountains-landscape.jpg"));
    
    // Forest theme
    themeTree.insert(Theme(2, "Forest", "green", "brown", "lightgreen", "darkgreen", "white", "../images/fantasy-scene-anime-style.jpg"));
    
    // Ocean theme
    themeTree.insert(Theme(3, "Ocean", "cyan", "orange", "lightcyan", "darkblue", "white", "../images/futuristic-moon-background.jpg"));
    
    // Neon theme
    themeTree.insert(Theme(4, "Neon", "magenta", "yellow", "pink", "black", "gray", "../images/fantasy-endless-hole-landscape.jpg"));
    
  
}

ThemeManager::ThemeManager() {
    initializeDefaultThemes();
    loadUserTheme();
}

void ThemeManager::saveUserTheme() {
    std::ofstream file("user_theme.dat", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&currentTheme.id), sizeof(currentTheme.id));
        file.close();
    }
}

void ThemeManager::loadUserTheme() {
    std::ifstream file("user_theme.dat", std::ios::binary);
    if (file.is_open()) {
        int savedId;
        file.read(reinterpret_cast<char*>(&savedId), sizeof(savedId));
        file.close();
        
        Theme theme = themeTree.search(savedId);
        if (theme.id != 0) {  // Check if valid theme (id != 0)
            currentTheme = theme;
        }
    }
}

void ThemeManager::displayThemeMenu() {
    std::cout << "\n=== THEME SELECTION ===\n";
    std::cout << "1. Browse all themes\n";
    std::cout << "2. Search theme by ID\n";
    std::cout << "3. Search theme by name\n";
    std::cout << "4. Show current theme\n";
    std::cout << "5. Apply theme\n";
    std::cout << "6. Exit theme menu\n";
    std::cout << "Current Theme: " << currentTheme.name << " (ID: " << currentTheme.id << ")\n";
    std::cout << "=======================\n";
}

void ThemeManager::browseThemes() {
    Theme arr[MAX_THEMES];
    int count = 0;
    themeTree.getInOrderList(arr, MAX_THEMES, count);
    
    std::cout << "\n=== All Themes ===\n";
    for (int i = 0; i < count; i++) {
        std::cout << "ID: " << arr[i].id << ", Name: " << arr[i].name << std::endl;
    }
    std::cout << "==================\n";
}

void ThemeManager::searchThemeById() {
    int id;
    std::cout << "Enter theme ID to search: ";
    std::cin >> id;
    
    Theme theme = themeTree.search(id);
    if (theme.id != 0) {
        std::cout << "Theme found:\n";
        theme.display();
    } else {
        std::cout << "Theme with ID " << id << " not found.\n";
    }
}

void ThemeManager::searchThemeByName() {
    // Simplified - browse all themes since AVLTree doesn't have searchByName
    browseThemes();
}

void ThemeManager::applyTheme() {
    // No need to ask for ID - just apply currentTheme
    if (currentTheme.id != 0) {
        saveUserTheme();
        std::cout << "Theme '" << currentTheme.name << "' applied successfully!\n";
    }
}

void ThemeManager::setThemeById(int themeId) {
    Theme theme = themeTree.search(themeId);
    if (theme.id != 0) {
        currentTheme = theme;
    }
}

void ThemeManager::showCurrentTheme() {
    std::cout << "\n=== Current Theme ===\n";
    std::cout << "ID: " << currentTheme.id << "\n";
    std::cout << "Name: " << currentTheme.name << "\n";
    std::cout << "Player Color: " << currentTheme.playerColor << "\n";
    std::cout << "Enemy Color: " << currentTheme.enemyColor << "\n";
    std::cout << "Trail Color: " << currentTheme.trailColor << "\n";
    std::cout << "Background Color: " << currentTheme.backgroundColor << "\n";
    std::cout << "Line Color: " << currentTheme.lineColor << "\n";
    std::cout << "=====================\n";
}

void ThemeManager::runThemeMenu() {
    int choice;
    do {
        displayThemeMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1: browseThemes(); break;
            case 2: searchThemeById(); break;
            case 3: searchThemeByName(); break;
            case 4: showCurrentTheme(); break;
            case 5: applyTheme(); break;
            case 6: std::cout << "Exiting theme menu...\n"; break;
            default: std::cout << "Invalid choice!\n";
        }
    } while (choice != 6);
}

const Theme& ThemeManager::getCurrentTheme() const { return currentTheme; }

Theme ThemeManager::getThemeById(int themeId) {
    // Get all themes and search for the one with matching ID
    Theme themes[50];
    int count = 0;
    listThemes(themes, 50, count);
    
    for (int i = 0; i < count; i++) {
        if (themes[i].id == themeId) {
            return themes[i];
        }
    }
    
    // If not found, return default theme (Classic - ID 1)
    return Theme(1, "Classic", "blue", "red", "lightblue", "black", "white", "images/theme_classic.png");
}

void ThemeManager::listThemes(Theme* arr, int maxSize, int& count) {
    themeTree.getInOrderList(arr, maxSize, count);
}