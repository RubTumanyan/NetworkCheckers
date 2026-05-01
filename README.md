 Network Checkers (C++ / SFML)
Overview

This project is a multiplayer checkers game written in C++ using SFML for graphics and TCP sockets for networking.

Two players can connect to a server and play checkers in real-time with full game logic including:

Piece movement
Capturing (including multiple captures)
King promotion
Flying kings (long diagonal moves)
 Features
 Classic checkers gameplay
Multiplayer using TCP sockets
 Turn-based logic
 Multi-capture (chain jumps)
King promotion and flying kings
 Graphical board using SFML
 Non-blocking network communication
Technologies Used
C++

SFML (Simple and Fast Multimedia Library)
POSIX sockets (TCP)
Epoll (server-side)
 Project Structure
/client
    main1.cpp
    graphics.cpp
    graphics.h

/common
    board.cpp
    board.h
    move.h

/server
    server.cpp
 How It Works
 Server
Uses EPOLL to handle multiple clients
Pairs two players into a game
Forwards moves between players
 Client
Connects to server via TCP
Receives player ID (1 or 2)
Sends moves to server
Renders board using SFML


How to Build & Run  
 Requirements  
C++17 compatible compiler (g++)  
CMake (>= 3.10)   
SFML (2.5+)   

Install SFML (Linux):  
sudo apt-get install libsfml-dev  

Build the Project   
mkdir build  
cd build  
cmake ..  
make   


in first process run  
./server   
and in two other process run       
./client  
and you can play

 Controls
 Mouse click to:
Select a piece
Choose destination tile
 Game Rules
Pieces move diagonally
Captures are mandatory
Multiple captures allowed in one turn
Pieces become kings upon reaching the opposite side
Kings can:
Move diagonally in all directions
Capture over long distances (flying kings)
 Known Issues
Turn synchronization may break if connection is unstable
No UI feedback for valid moves yet
No win/lose detection implemented
 Future Improvements
Highlight valid moves
Add win condition detection
Improve UI/UX (animations, sounds)
Add AI opponent
Better error handling for networking
 Author

Created by Ruben
Student of Applied Mathematics & Informatics

 Notes

This project was built for learning:

Networking (client-server architecture)
Game logic implementation
Real-time interaction
Graphics programming with SFML




