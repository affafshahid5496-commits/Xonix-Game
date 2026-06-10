#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

struct Theme {
    int id;
    std::string name;
    std::string playerColor;
    std::string enemyColor;
    std::string trailColor;
    std::string backgroundColor;
    std::string lineColor;
    std::string imagePath;

    Theme(int i = 0, const std::string& n = "", const std::string& pc = "", 
          const std::string& ec = "", const std::string& tc = "", 
          const std::string& bc = "", const std::string& lc = "", 
          const std::string& path = "")
        : id(i), name(n), playerColor(pc), enemyColor(ec), trailColor(tc), 
          backgroundColor(bc), lineColor(lc), imagePath(path) {}
    
    void display() const {
        std::cout << "Theme ID: " << id << ", Name: " << name << std::endl;
        std::cout << "  Player Color: " << playerColor << ", Enemy Color: " << enemyColor << std::endl;
        std::cout << "  Background: " << backgroundColor << std::endl;
    }
    
    // Convert color string to sf::Color
    static sf::Color stringToColor(const std::string& colorStr) {
        if (colorStr == "blue") return sf::Color::Blue;
        if (colorStr == "red") return sf::Color::Red;
        if (colorStr == "green") return sf::Color::Green;
        if (colorStr == "yellow") return sf::Color::Yellow;
        if (colorStr == "cyan") return sf::Color::Cyan;
        if (colorStr == "magenta") return sf::Color::Magenta;
        if (colorStr == "white") return sf::Color::White;
        if (colorStr == "black") return sf::Color::Black;
        if (colorStr == "lightblue") return sf::Color(173, 216, 230);
        if (colorStr == "darkgreen") return sf::Color(0, 100, 0);
        if (colorStr == "lightgreen") return sf::Color(144, 238, 144);
        if (colorStr == "brown") return sf::Color(165, 42, 42);
        if (colorStr == "lightcyan") return sf::Color(224, 255, 255);
        if (colorStr == "darkblue") return sf::Color(0, 0, 139);
        if (colorStr == "orange") return sf::Color(255, 165, 0);
        if (colorStr == "pink") return sf::Color(255, 192, 203);
        if (colorStr == "gray") return sf::Color(128, 128, 128);
        if (colorStr == "lightyellow") return sf::Color(255, 255, 224);
        return sf::Color::White; // Default
    }
    
    sf::Color getPlayerColor() const { return stringToColor(playerColor); }
    sf::Color getEnemyColor() const { return stringToColor(enemyColor); }
    sf::Color getTrailColor() const { return stringToColor(trailColor); }
    sf::Color getBackgroundColor() const { return stringToColor(backgroundColor); }
    sf::Color getLineColor() const { return stringToColor(lineColor); }
};