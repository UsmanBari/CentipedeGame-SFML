# Centipede Game 🎮

A modern recreation of the classic **Centipede** arcade game built using **C++** and **SFML**. This project features custom sound effects, music, and textures to deliver a retro gameplay experience with polished visuals and audio.

## 🛠 Features

- 🐛 Centipede movement and segment-based mechanics
- 🔫 Player shooting and scoring
- 🎵 Background music and classic sound effects
- 🎨 Custom textures and font
- 💀 Enemies like scorpions, fleas, and spiders
- 🎯 Increasing difficulty and level progression

## 📁 Project Structure

```
23i-0680_G_Project/
├── Centipede.cpp                         # Main game source code
├── sfml-app                              # Compiled game binary (Linux)
├── Fonts/                                # Custom game fonts
├── Music/                                # Background music files
├── Sound Effects/                        # In-game sound effects
├── Textures.zip                          # Game textures (extract before running)
├── Steps To Compile.txt                  # Build instructions


```

## 🚀 How to Run

### Prerequisites

- C++17 or later
- [SFML 2.5+](https://www.sfml-dev.org/)

### Linux Build Instructions

1. Ensure SFML is installed:
   ```bash
   sudo apt-get install libsfml-dev
   ```

2. Compile the game:
   ```bash
   g++ Centipede.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
   ```

3. Run the game:
   ```bash
   ./sfml-app
   ```

> ⚠️ **Note:** Make sure all assets (Fonts, Music, Sound Effects, Textures) are in the correct paths relative to the binary.



## 👨‍💻 Developer

- **Muhammad Usman Bari** 

## 📜 License

This project is for educational and personal use only. All rights to third-party assets (e.g., sound effects, music, fonts) belong to their respective owners.

