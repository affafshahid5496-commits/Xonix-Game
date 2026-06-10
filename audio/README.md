# Audio Files

This directory contains audio files for the XONIX game.

## Required Files

- `background.wav` - Background music for the game (looped, 50% volume)
  - Format: WAV file
  - Recommended: Loop-friendly, upbeat background music
  - Size: Any reasonable size (will be loaded as streaming audio via SFML Music)

## How to Add Audio

1. Place your audio file as `background.wav` in this directory
2. Supported formats by SFML: WAV, OGG, FLAC
3. The game will automatically attempt to load and play the music
4. If the file is not found, a warning will be printed and the game will continue without music

## Audio Properties

- Volume: 50% (adjustable in GameScreen.cpp)
- Loop: Enabled (plays continuously)
- Type: SFML Music (streaming, good for long tracks)

## Example Sources

You can find free game audio at:
- https://freepd.com/ (Free music for games)
- https://opengameart.org/ (Game assets including music)
- https://freesound.org/ (Sound effects and music)

Make sure to check licensing before using!
