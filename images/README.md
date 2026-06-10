# Game Images

This directory contains image assets for the XONIX game.

## Required Files

### tree.png - Background Image (NEW)
- Format: PNG image
- Size: Should match game window (720x450) or larger for tiling
- Purpose: Background sprite rendered behind all game elements
- Recommendation: Tree-themed image for the game aesthetics

### tiles.png - Game Tiles
- Contains sprite sheet with 3 tile types (TILE_SIZE x TILE_SIZE each at 18x18 pixels)
  - TextureRect (0, 0, 18, 18): Border/wall tiles
  - TextureRect (54, 0, 18, 18): Captured/owned tiles  
  - TextureRect (36, 0, 18, 18): Player sprite

### enemy.png - Enemy Sprite
- Format: PNG with transparency
- Size: ~40x40 pixels recommended
- Purpose: Enemy sprite that rotates during gameplay
- Features: Will be rotated 10 degrees each frame for animation effect

### gameover.png - Game Over Screen (Optional)
- Format: PNG image
- Purpose: Overlay image for game over screen
- Status: Optional (warning printed if not found, game continues)

## How to Add Images

1. Save PNG files (preferred format with transparency support)
2. Place them in this directory
3. Game will load them automatically on startup
4. If files are missing, warnings will print to stderr but game will continue

## Image Asset Sources

You can find free game assets at:
- https://opengameart.org/ (Free game graphics)
- https://itch.io/ (Asset packs)
- https://www.kenney.nl/assets (Free game assets)

Make sure to check licensing before using!
