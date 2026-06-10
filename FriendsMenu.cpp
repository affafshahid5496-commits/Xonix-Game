#include "FriendsMenu.h"
#include <iostream>
#include <fstream>

FriendsMenu::FriendsMenu() : selectedItem(0), font(nullptr), currentPlayer(nullptr), auth(nullptr), popupActive(false), pendingAction(-1), requestCount(0), viewingFriends(false), friendsCount(0), viewingPendingRequests(false), displayedRequestsCount(0), selectedRequestIndex(0), shouldReturnToMenu(false)
{
    // Initialize menuTexts array
    for (int i = 0; i < 7; i++) {
        menuTexts[i] = sf::Text();
    }
}

void FriendsMenu::initialize(sf::Font& fontRef, Player* player, AuthSystem* authRef)
{
    font = &fontRef;
    currentPlayer = player;
    auth = authRef;

    titleText.setFont(fontRef);
    titleText.setString("FRIENDS MENU");
    titleText.setCharacterSize(55);
    titleText.setFillColor(sf::Color::Green);
    titleText.setPosition(600 - titleText.getLocalBounds().width / 2, 50);

    updateVisuals();
}

void FriendsMenu::updateVisuals()
{
    int startY = 180;
    int spacing = 45;

    // Menu items as fixed array
    const char* itemLabels[] = {
        "SEND FRIEND REQUEST",
        "ACCEPT FRIEND REQUEST",
        "DECLINE FRIEND REQUEST",
        "REMOVE FRIEND",
        "VIEW FRIENDS",
        "VIEW PENDING REQUESTS",
        "BACK"
    };

    for (int i = 0; i < 7; ++i)
    {
        sf::Text txt;
        txt.setFont(*font);

        if (i == selectedItem)
        {
            std::string str = std::string("> ") + itemLabels[i] + " <";
            txt.setString(str);
            txt.setCharacterSize(40);
            txt.setFillColor(sf::Color::Yellow);
        }
        else
        {
            txt.setString(itemLabels[i]);
            txt.setCharacterSize(34);
            txt.setFillColor(sf::Color::White);
        }

        sf::FloatRect b = txt.getLocalBounds();
        txt.setPosition(600 - b.width / 2, startY + i * spacing);

        menuTexts[i] = txt;
    }
}
void FriendsMenu::handleEvent(sf::Event& event, sf::RenderWindow& window)
{
    // If viewing friends, only allow ESC to go back
    if (viewingFriends)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            exitFriendsView();
        }
        return;
    }
    
    // If viewing pending requests, allow navigation and actions
    if (viewingPendingRequests)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape) {
                exitPendingRequestsView();
                return;
            }
            else if (event.key.code == sf::Keyboard::Up)
            {
                if (displayedRequestsCount > 0) {
                    selectedRequestIndex = (selectedRequestIndex - 1 + displayedRequestsCount) % displayedRequestsCount;
                }
                return;
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                if (displayedRequestsCount > 0) {
                    selectedRequestIndex = (selectedRequestIndex + 1) % displayedRequestsCount;
                }
                return;
            }
            else if (event.key.code == sf::Keyboard::Return && displayedRequestsCount > 0)
            {
                // Accept the selected pending request
                acceptRequestFrom(pendingRequests[selectedRequestIndex].sender);
                displayPendingRequestsOnScreen(); // Refresh the list
                return;
            }
        }
        return;
    }
    
    if (popupActive)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            popupActive = false;
            pendingAction = -1;
            return;
        }
        
        // Handle Enter key directly for popup submission
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
            std::string user = popupBuffer;
            // Trim the input
            size_t start = user.find_first_not_of(" ");
            size_t end = user.find_last_not_of(" ");
            if (start != std::string::npos) {
                user = user.substr(start, (end - start + 1));
            } else {
                user = "";
            }

            if (!user.empty()) {
                switch (pendingAction)
                {
                    case SEND_REQUEST:    sendRequestTo(user); break;
                    case ACCEPT_REQUEST:  acceptRequestFrom(user); break;
                    case DECLINE_REQUEST: declineRequestFrom(user); break;
                    case REMOVE_FRIEND:   auth->removeFriend(currentPlayer->getUsername(), user); break;
                }
            }
            popupActive = false;
            pendingAction = -1;
            return;
        }
        
        if (handlePopupEvent(event))
        {
            std::string user = popupBuffer;
            // Trim the input
            size_t start = user.find_first_not_of(" ");
            size_t end = user.find_last_not_of(" ");
            if (start != std::string::npos) {
                user = user.substr(start, (end - start + 1));
            } else {
                user = "";
            }

            if (!user.empty()) {
                switch (pendingAction)
                {
                    case SEND_REQUEST:    sendRequestTo(user); break;
                    case ACCEPT_REQUEST:  acceptRequestFrom(user); break;
                    case DECLINE_REQUEST: declineRequestFrom(user); break;
                    case REMOVE_FRIEND:   auth->removeFriend(currentPlayer->getUsername(), user); break;
                }
            }
            popupActive = false;
            pendingAction = -1;
        }
        return;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape) {
            selectedItem = BACK; // Set to BACK option
            return;
        }
        else if (event.key.code == sf::Keyboard::Up)
        {
            selectedItem = (selectedItem - 1 + 7) % 7;
            updateVisuals();
            return;
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            selectedItem = (selectedItem + 1) % 7;
            updateVisuals();
            return;
        }
        else if (event.key.code == sf::Keyboard::Return)
        {
            switch (selectedItem)
            {
                case SEND_REQUEST:
                    openPopup("Enter username to send request:", SEND_REQUEST); break;

                case ACCEPT_REQUEST:
                    openPopup("Enter username to accept:", ACCEPT_REQUEST); break;

                case DECLINE_REQUEST:
                    openPopup("Enter username to decline:", DECLINE_REQUEST); break;

                case REMOVE_FRIEND:
                    openPopup("Enter username to remove:", REMOVE_FRIEND); break;

                case VIEW_FRIENDS:
                    displayFriendsOnScreen(); break;

                case VIEW_PENDING:
                    displayPendingRequestsOnScreen(); break;

                case BACK:
                    shouldReturnToMenu = true;
                    break;
            }
        }
    }
}
void FriendsMenu::draw(sf::RenderWindow& window)
{
    if (viewingPendingRequests)
    {
        // Draw pending requests view
        sf::Text titleText;
        titleText.setFont(*font);
        titleText.setString("PENDING REQUESTS");
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::Magenta);
        titleText.setStyle(sf::Text::Bold);
        int titleX = 600 - titleText.getLocalBounds().width / 2;
        titleText.setPosition(titleX, 30);
        window.draw(titleText);
        
        if (displayedRequestsCount == 0)
        {
            sf::Text noRequests;
            noRequests.setFont(*font);
            noRequests.setString("You have no pending friend requests.\nCheck back later!");
            noRequests.setCharacterSize(24);
            noRequests.setFillColor(sf::Color::Yellow);
            int noRequestsX = 600 - noRequests.getLocalBounds().width / 2;
            noRequests.setPosition(noRequestsX, 200);
            window.draw(noRequests);
        }
        else
        {
            // Draw pending requests list with selection highlight
            for (int i = 0; i < displayedRequestsCount; i++)
            {
                if (i == selectedRequestIndex)
                {
                    // Draw highlight background for selected item
                    sf::RectangleShape highlight(sf::Vector2f(500, 45));
                    highlight.setFillColor(sf::Color(255, 215, 0, 100)); // Gold with transparency
                    highlight.setPosition(350, 120 + (i * 50) - 5);
                    window.draw(highlight);
                }
                window.draw(pendingRequestsList[i]);
            }
        }
        
        // Draw instructions
        sf::Text instructions;
        instructions.setFont(*font);
        instructions.setString("UP/DOWN: Navigate | ENTER: Accept | ESC: Back");
        instructions.setCharacterSize(16);
        instructions.setFillColor(sf::Color::Cyan);
        int instrX = 600 - instructions.getLocalBounds().width / 2;
        instructions.setPosition(instrX, 550);
        window.draw(instructions);
    }
    else if (viewingFriends)
    {
        // Draw friends list view
        sf::Text titleText;
        titleText.setFont(*font);
        titleText.setString("MY FRIENDS");
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::Cyan);
        titleText.setStyle(sf::Text::Bold);
        int titleX = 600 - titleText.getLocalBounds().width / 2;
        titleText.setPosition(titleX, 30);
        window.draw(titleText);
        
        if (friendsCount == 0)
        {
            sf::Text noFriends;
            noFriends.setFont(*font);
            noFriends.setString("You have no friends yet.\nAdd some friends to get started!");
            noFriends.setCharacterSize(24);
            noFriends.setFillColor(sf::Color::Yellow);
            int noFriendsX = 600 - noFriends.getLocalBounds().width / 2;
            noFriends.setPosition(noFriendsX, 200);
            window.draw(noFriends);
        }
        else
        {
            // Draw friends list
            for (int i = 0; i < friendsCount; i++)
            {
                window.draw(friendsList[i]);
            }
        }
        
        // Draw back instruction
        sf::Text backText;
        backText.setFont(*font);
        backText.setString("Press ESC to return to Friends Menu");
        backText.setCharacterSize(18);
        backText.setFillColor(sf::Color::Green);
        int backX = 600 - backText.getLocalBounds().width / 2;
        backText.setPosition(backX, 550);
        window.draw(backText);
    }
    else
    {
        // Draw normal menu
        window.draw(titleText);

        for (int i = 0; i < 7; i++)
            window.draw(menuTexts[i]);

        if (popupActive)
            drawPopup(window);

        if (font)
        {
            sf::Text info;
            info.setFont(*font);
            info.setString("UP/DOWN to navigate  |  ENTER to select  |  ESC to return");
            info.setCharacterSize(18);
            info.setFillColor(sf::Color::Cyan);
            info.setPosition(600 - info.getLocalBounds().width / 2, 550);
            window.draw(info);
        }
    }
}

void FriendsMenu::resetSelection()
{
    selectedItem = 0;
    updateVisuals();
}

void FriendsMenu::openPopup(const std::string& message, int action)
{
    popupActive = true;
    pendingAction = action;
    popupBuffer.clear();

    popupBox.setSize({500, 200});
    popupBox.setFillColor(sf::Color(0, 0, 0, 220));
    popupBox.setOutlineColor(sf::Color::Yellow);
    popupBox.setOutlineThickness(3);
    popupBox.setPosition(350, 200);

    popupLabel.setFont(*font);
    popupLabel.setString(message);
    popupLabel.setFillColor(sf::Color::White);
    popupLabel.setCharacterSize(20);
    popupLabel.setPosition(370, 220);

    popupInput.setFont(*font);
    popupInput.setString("_"); 
    popupInput.setFillColor(sf::Color::Cyan);
    popupInput.setCharacterSize(24);
    popupInput.setPosition(370, 280);
}

bool FriendsMenu::handlePopupEvent(sf::Event& event)
{
    if (!popupActive) return false;

    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == '\b') // Backspace
        {
            if (!popupBuffer.empty())
                popupBuffer.pop_back();
        }
        else if (event.text.unicode == '\r') // Enter - only return true if input is not empty
        {
            if (popupBuffer.empty()) {
                return false;
            }
            return true;
        }
        else if (event.text.unicode == 27) // Escape
        {
            popupActive = false;
            pendingAction = -1;
            return false;
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 127) // Printable characters
        {
            if (popupBuffer.length() < 50) { // Limit input length to 50 characters
                popupBuffer += static_cast<char>(event.text.unicode);
            }
        }

        popupInput.setString(popupBuffer + "_"); // Add cursor
    }

    return false;
}

void FriendsMenu::drawPopup(sf::RenderWindow& window)
{
    window.draw(popupBox);
    window.draw(popupLabel);
    window.draw(popupInput);
}

void FriendsMenu::loadRequests()
{
    requestCount = 0;

    std::ifstream file(requestsFile);
    if (!file.is_open()) return;

    std::string receiver, sender;

    while (file >> receiver >> sender && requestCount < 50)
    {
        pendingRequests[requestCount].receiver = receiver;
        pendingRequests[requestCount].sender = sender;
        requestCount++;
    }

    file.close();
}

void FriendsMenu::saveRequests()
{
    std::ofstream file(requestsFile, std::ios::trunc);
    if (!file.is_open()) return;

    for (int i = 0; i < requestCount; i++)
    {
        file << pendingRequests[i].receiver << " " << pendingRequests[i].sender << "\n";
    }

    file.close();
}
void FriendsMenu::sendRequestTo(const std::string& toUser)
{
    std::string trimmedUser = toUser;
    // Trim leading spaces
    trimmedUser.erase(0, trimmedUser.find_first_not_of(" "));
    // Trim trailing spaces
    trimmedUser.erase(trimmedUser.find_last_not_of(" ") + 1);
    
    if (trimmedUser.empty())
    {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }
    
    if (trimmedUser == currentPlayer->getUsername())
    {
        std::cout << "Error: Cannot send request to yourself.\n";
        return;
    }

    // Use AuthSystem's sendFriendRequest method which handles all logic and saves to file
    if (auth->sendFriendRequest(currentPlayer->getUsername(), trimmedUser)) {
        std::cout << "Friend request sent to: " << trimmedUser << std::endl;
    } else {
        std::cout << "Error: Could not send friend request to: " << trimmedUser << std::endl;
    }
}

void FriendsMenu::printPendingRequests()
{
    loadRequests();

    std::string me = currentPlayer->getUsername();
    bool found = false;

    std::cout << "\nPending Friend Requests:\n";

    for (int i = 0; i < requestCount; i++)
    {
        if (pendingRequests[i].receiver == me)
        {
            found = true;
            std::cout << "- " << pendingRequests[i].sender << "\n";
        }
    }

    if (!found)
        std::cout << "No pending requests.\n";
}
void FriendsMenu::acceptRequestFrom(const std::string& user)
{
    std::string trimmedUser = user;
    // Trim leading spaces
    trimmedUser.erase(0, trimmedUser.find_first_not_of(" "));
    // Trim trailing spaces
    trimmedUser.erase(trimmedUser.find_last_not_of(" ") + 1);
    
    if (trimmedUser.empty())
    {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }
    
    // Use AuthSystem's acceptFriendRequest method which handles all logic and saves to files
    if (auth->acceptFriendRequest(currentPlayer->getUsername(), trimmedUser)) {
        std::cout << "Friend request from " << trimmedUser << " accepted.\n";
    } else {
        std::cout << "Error: Could not accept friend request from " << trimmedUser << "\n";
    }
}

void FriendsMenu::declineRequestFrom(const std::string& user)
{
    std::string trimmedUser = user;
    // Trim leading spaces
    trimmedUser.erase(0, trimmedUser.find_first_not_of(" "));
    // Trim trailing spaces
    trimmedUser.erase(trimmedUser.find_last_not_of(" ") + 1);
    
    if (trimmedUser.empty())
    {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }
    
    // Use AuthSystem's declineFriendRequest method which handles all logic and saves to file
    if (auth->declineFriendRequest(currentPlayer->getUsername(), trimmedUser)) {
        std::cout << "Friend request from " << trimmedUser << " declined.\n";
    } else {
        std::cout << "Error: Could not decline friend request from " << trimmedUser << "\n";
    }
}

void FriendsMenu::displayFriendsOnScreen()
{
    if (!currentPlayer || !font || !auth)
    {
        std::cout << "Error: Player, font, or auth system not initialized.\n";
        return;
    }
    
    viewingFriends = true;
    friendsCount = 0;
    
    // Load friend list from file to ensure it's up to date
    auth->loadFriendListFromFile(currentPlayer->getUsername());
    
    // Get the friend list from current player
    LinkedList& friendsList_ref = currentPlayer->getFriendsList();
    
    if (friendsList_ref.isEmpty())
    {
        friendsCount = 0;
        return;
    }
    
    // Extract friends from linked list and display them
    ListNode* current = friendsList_ref.getHead();
    int startY = 120;
    int spacing = 45;
    int index = 0;
    
    while (current != nullptr && index < 100)
    {
        sf::Text friendText;
        friendText.setFont(*font);
        friendText.setString(std::to_string(index + 1) + ". " + current->data);
        friendText.setCharacterSize(32);
        friendText.setFillColor(sf::Color::White);
        
        // Alternate colors for better visual appeal
        if (index % 2 == 0)
        {
            friendText.setFillColor(sf::Color::Cyan);
        }
        else
        {
            friendText.setFillColor(sf::Color::Yellow);
        }
        
        int xPos = 250;
        int yPos = startY + (index * spacing);
        friendText.setPosition(xPos, yPos);
        
        friendsList[index] = friendText;
        friendsCount++;
        index++;
        current = current->next;
    }
}

void FriendsMenu::exitFriendsView()
{
    viewingFriends = false;
    friendsCount = 0;
    updateVisuals();
}

bool FriendsMenu::isViewingFriends() const
{
    return viewingFriends;
}

void FriendsMenu::displayPendingRequestsOnScreen()
{
    if (!currentPlayer || !font || !auth)
    {
        std::cout << "Error: Player, font, or auth system not initialized.\n";
        return;
    }
    
    viewingPendingRequests = true;
    displayedRequestsCount = 0;
    selectedRequestIndex = 0;
    
    // Get pending requests from AuthSystem
    std::string requests[100];
    int requestCount_temp = 0;
    auth->getPendingRequests(currentPlayer->getUsername(), requests, requestCount_temp);
    
    int startY = 120;
    int spacing = 50;
    
    // Display pending requests
    for (int i = 0; i < requestCount_temp && i < 100; i++)
    {
        sf::Text requestText;
        requestText.setFont(*font);
        requestText.setString("From: " + requests[i]);
        requestText.setCharacterSize(32);
        requestText.setFillColor(sf::Color::White);
        
        // Alternate colors for better visual appeal
        if (i % 2 == 0)
        {
            requestText.setFillColor(sf::Color::Cyan);
        }
        else
        {
            requestText.setFillColor(sf::Color::Yellow);
        }
        
        int xPos = 350;
        int yPos = startY + (i * spacing);
        requestText.setPosition(xPos, yPos);
        
        pendingRequestsList[i] = requestText;
        
        // Store the sender in pendingRequests for later use
        pendingRequests[i].sender = requests[i];
        pendingRequests[i].receiver = currentPlayer->getUsername();
        
        displayedRequestsCount++;
    }
}

void FriendsMenu::exitPendingRequestsView()
{
    viewingPendingRequests = false;
    displayedRequestsCount = 0;
    selectedRequestIndex = 0;
    updateVisuals();
}

bool FriendsMenu::isViewingPendingRequests() const
{
    return viewingPendingRequests;
}