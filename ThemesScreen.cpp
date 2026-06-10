#include "ThemesScreen.h"

ThemesScreen::ThemesScreen() {
    manager = nullptr;
    currentPlayer = nullptr;
    selected = 0;
}

ThemesScreen::ThemesScreen(ThemeManager* m, Player* p) {
    manager = m;
    currentPlayer = p;
    selected = 0;
}

void ThemesScreen::draw(sf::RenderWindow& window, sf::Font& font) {
    // Draw centered THEMES header at top
    sf::Text header("THEMES", font, 48);
    header.setFillColor(sf::Color::Magenta);
    header.setStyle(sf::Text::Bold);
    float headerX = (window.getSize().x - header.getLocalBounds().width) / 2;
    header.setPosition(headerX, 20);
    window.draw(header);

    Theme arr[20];
    int count = 0;
    manager->listThemes(arr, 20, count);

    for (int i = 0; i < count; i++) {
        sf::Texture tex;
        tex.loadFromFile(arr[i].imagePath);
        sf::Sprite preview(tex);
        preview.setScale(0.25f, 0.25f);
        preview.setPosition(50, 100 + i * 130);
        window.draw(preview);

        sf::Text name(arr[i].name, font, 26);
        name.setFillColor(i == selected ? sf::Color::Yellow : sf::Color::White);
        name.setPosition(300, 120 + i * 130);
        window.draw(name);
    }
}

void ThemesScreen::handleInput(sf::Event& event) {
    Theme arr[20];
    int count = 0;
    manager->listThemes(arr, 20, count);

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up && selected > 0)
            selected--;

        if (event.key.code == sf::Keyboard::Down && selected < count - 1)
            selected++;

        if (event.key.code == sf::Keyboard::Enter) {
            if (currentPlayer && manager) {
                // Set the theme in manager
                manager->setThemeById(arr[selected].id);
                
                // Save theme ID to player profile
                currentPlayer->setCurrentThemeId(arr[selected].id);
                
                // Apply theme to save
                manager->applyTheme();
                
                std::cout << "Theme '" << arr[selected].name << "' applied successfully!" << std::endl;
            }
        }
    }
}