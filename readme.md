# Block Game

A simple block-based game inspired by classic falling block puzzle games like Tetris. This project is implemented in C++ and uses an LCD device for rendering.

## Features

- **Random Object Generation**: Different types of blocks (loong, square, chair, hook, Rchar, Rhook, triangle) are generated randomly.
- **Rotation**: Blocks can be rotated right and left.
- **Movement**: Blocks can be moved left, right, or made to fall faster.
- **Line Clearing**: Completed lines are automatically cleared from the board.
- **Full Drop**: Blocks can be dropped instantly to the bottom.
- **Screen Refresh**: The LCD screen is refreshed in real-time to show the current state of the game.
- **Threading**: The game utilizes multi-threading for handling block falling and user input simultaneously.

## Setup

### Prerequisites

- A C++ compiler supporting C++11 or later.
- A working LCD device with appropriate drivers.
- A touchscreen device for user interaction.