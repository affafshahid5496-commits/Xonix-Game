#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include "authentication.h"

struct FriendRequestLocal {
    std::string receiver;
    std::string sender;
};

class FriendsMenu {
private:
    // Popup
    bool popupActive;
    sf::RectangleShape popupBox;
    sf::Text popupLabel;
    sf::Text popupInput;
    std::string popupBuffer;
    int pendingAction;

    // Menu items and text (using fixed array)
    sf::Text menuTexts[7];    // stores 7 menu options
    int selectedItem;

    sf::Text titleText;

    // Game state
    Player* currentPlayer;
    AuthSystem* auth;
    sf::Font* font;

    // Friend request storage (fixed array)
    FriendRequestLocal pendingRequests[100];  // increased to 100 to match AuthSystem
    int requestCount;
    const std::string requestsFile = "friend_requests.txt"; // made const
    
    // Friends display members
    bool viewingFriends;
    sf::Text friendsList[100];  // Display up to 100 friends
    int friendsCount;
    
    // Pending requests display members
    bool viewingPendingRequests;
    sf::Text pendingRequestsList[100];  // Display pending requests
    int displayedRequestsCount;
    int selectedRequestIndex;
    
    // Navigation state
    bool shouldReturnToMenu;

public:
    enum Action {
        NONE = -1,
        SEND_REQUEST = 0,
        ACCEPT_REQUEST = 1,
        DECLINE_REQUEST = 2,
        REMOVE_FRIEND = 3,
        VIEW_FRIENDS = 4,
        VIEW_PENDING = 5,
        BACK = 6
    };

    FriendsMenu();

    void initialize(sf::Font& fontRef, Player* player, AuthSystem* authRef);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    int getSelectedAction() const;
    void resetSelection();

    // Popup
    void openPopup(const std::string& message, int action);
    bool handlePopupEvent(sf::Event& event);
    void drawPopup(sf::RenderWindow& window);

    // Friend request operations
    void loadRequests();
    void saveRequests();
    void sendRequestTo(const std::string& user);
    void acceptRequestFrom(const std::string& user);
    void declineRequestFrom(const std::string& user);
    void printPendingRequests();
    
    // Friends display
    void displayFriendsOnScreen();
    void exitFriendsView();
    bool isViewingFriends() const;
    
    // Pending requests display
    void displayPendingRequestsOnScreen();
    void exitPendingRequestsView();
    bool isViewingPendingRequests() const;
    
    // Navigation helpers
    bool shouldReturn() const { return shouldReturnToMenu; }
    void resetReturnFlag() { shouldReturnToMenu = false; }

private:
    void updateVisuals();
};