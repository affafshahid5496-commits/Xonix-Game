cmake_minimum_required(VERSION 3.10)
project(Xonix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where Homebrew installs packages
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar")

# Find SFML on macOS
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Copy images folder to build
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

# Copy audio folder to build
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/audio" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

# Include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# Source files
set(SOURCES
    main.cpp
    authentication.cpp
    DArray.cpp
    Player.cpp
    LinkedList.cpp
    gui.cpp
    HashTable.cpp 
    GameScreen.cpp
    LoginScreen.cpp
    MainMenu.cpp
    PauseMenu.cpp
    SaveGame.cpp
    SavedGamesScreen.cpp
    Multiplayer.cpp
    FriendsMenu.cpp
    Leaderboard.cpp
    Levels.cpp
    Matchmaking.cpp
    MatchmakingDifficulty.cpp
    AVLTREE.cpp
    theme_manager.cpp
    ThemesScreen.cpp
    ProfileScreen.cpp
)

# Executable
add_executable(xonix ${SOURCES})

# Link libraries
target_link_libraries(xonix PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
