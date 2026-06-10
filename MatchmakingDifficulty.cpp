#include "MatchmakingDifficulty.h"
#include <iostream>

MatchmakingDifficulty::MatchmakingDifficulty()
    : selectedDifficulty(MEDIUM), currentFont(nullptr)
{
}

void MatchmakingDifficulty::initialize(sf::Font& font)
{
    currentFont = &font;
    
    titleText.setFont(font);
    titleText.setString("SELECT DIFFICULTY FOR MATCHMAKING");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(600 - titleText.getLocalBounds().width / 2, 50);
    
    updateVisualElements();
}

void MatchmakingDifficulty::updateVisualElements()
{
    if (!currentFont) return;
    
    const char* labels[] = { "EASY", "MEDIUM", "HARD" };
    const char* descriptions[] = {
        "Beginner difficulty - Good for warm-up",
        "Standard difficulty - Balanced challenge",
        "Expert difficulty - Highly competitive"
    };
    
    int startY = 200;
    int spacing = 80;
    
    for (int i = 0; i < 3; ++i)
    {
        sf::Text txt;
        txt.setFont(*currentFont);
        
        if (i == selectedDifficulty)
        {
            std::string str = std::string("> ") + labels[i] + " <";
            txt.setString(str);
            txt.setCharacterSize(50);
            txt.setFillColor(sf::Color::Yellow);
        }
        else
        {
            txt.setString(labels[i]);
            txt.setCharacterSize(40);
            txt.setFillColor(sf::Color::White);
        }
        
        sf::FloatRect b = txt.getLocalBounds();
        txt.setPosition(600 - b.width / 2, startY + i * spacing);
        
        difficultyTexts[i] = txt;
    }
}

void MatchmakingDifficulty::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Up)
        {
            selectedDifficulty = (selectedDifficulty - 1 + 3) % 3;
            updateVisualElements();
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            selectedDifficulty = (selectedDifficulty + 1) % 3;
            updateVisualElements();
        }
        else if (event.key.code == sf::Keyboard::Escape)
        {
            // Will be handled by main.cpp
        }
    }
}

void MatchmakingDifficulty::draw(sf::RenderWindow& window)
{
    window.draw(titleText);
    
    for (int i = 0; i < 3; ++i)
    {
        window.draw(difficultyTexts[i]);
    }
    
    // Draw description
    if (currentFont)
    {
        const char* descriptions[] = {
            "Beginner - Good for warm-up matches",
            "Standard - Balanced competitive play",
            "Expert - Highly skilled opponents"
        };
        
        sf::Text desc;
        desc.setFont(*currentFont);
        desc.setString(descriptions[selectedDifficulty]);
        desc.setCharacterSize(22);
        desc.setFillColor(sf::Color::Green);
        desc.setPosition(600 - desc.getLocalBounds().width / 2, 470);
        window.draw(desc);
        
        sf::Text instructions;
        instructions.setFont(*currentFont);
        instructions.setString("UP/DOWN: Select | ENTER: Confirm | ESC: Back");
        instructions.setCharacterSize(18);
        instructions.setFillColor(sf::Color::Cyan);
        instructions.setPosition(600 - instructions.getLocalBounds().width / 2, 550);
        window.draw(instructions);
    }
}

int MatchmakingDifficulty::getSelectedAction() const
{
    switch (selectedDifficulty)
    {
        case EASY: return SELECT_EASY;
        case MEDIUM: return SELECT_MEDIUM;
        case HARD: return SELECT_HARD;
        default: return NO_ACTION;
    }
}

void MatchmakingDifficulty::resetSelection()
{
    selectedDifficulty = MEDIUM;
    updateVisualElements();
}
