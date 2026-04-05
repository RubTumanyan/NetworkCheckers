#include <SFML/Graphics.hpp>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../common/board.h"
#include "../common/move.h"
#include "graphics.h"
#include "../common/move.h"

int myPlayer = 2;      // my piecies
int currentPlayer = 2; // for right piecies access 

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) { std::cerr << "Socket creation failed\n"; return 1; }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n"; return 1;
    }

    recv(sock, &myPlayer, sizeof(myPlayer), 0);   // server -> send myPlayer
    std::cout << "I am player: " << myPlayer << std::endl;


    std::cout << "Connected to server\n";

    const int tileSize = 80;

// --- Create the SFML window ---
    sf::RenderWindow window(sf::VideoMode(640, 640), "Checkers");
    window.setFramerateLimit(60); // limit to 60 FPS if we have more it sleep
 
 
    while(window.isOpen())
    {
        sf::Event event;

        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)window.close();

            if(event.type == sf::Event::MouseButtonPressed)
            {    
		std::cout << "Click detected! My ID: " << myPlayer << " Current Turn: " << currentPlayer << std::endl;
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;

      if(currentPlayer == myPlayer){
    Move move = handleScreenClick(mouseX, mouseY, tileSize);

    if(move.fromX != -1) {
    
        applyMove(move, myPlayer);

        bool isCapture = abs(move.toX - move.fromX) > 1;
        send(sock, &move, sizeof(move), 0);

        if(isCapture && canCaptureAgain(move.toX, move.toY, myPlayer)) {
            // CONTINUE TURN → do not switch player
            selectedX = move.toX;
            selectedY = move.toY;
        }
        else {
            // TURN FINISHED → switch player
            currentPlayer = (myPlayer == 1) ? 2 : 1;
            selectedX = -1;
            selectedY = -1;
        }
    }
}
}
}
// --- Receive opponent move ---
Move oppMove;
int bytes = recv(sock, &oppMove, sizeof(oppMove), MSG_DONTWAIT); // non-blocking
if(bytes > 0) {
 
    int opponent = (myPlayer == 1) ? 2 : 1;
    applyMove(oppMove, opponent);

    bool isCapture = abs(oppMove.toX - oppMove.fromX) > 1;

    if(!(isCapture && canCaptureAgain(oppMove.toX, oppMove.toY, opponent))) {
        // only switch turn if opponent cannot capture again
	selectedX = -1;
	selectedY = -1;
        currentPlayer = myPlayer;
    }else{
	    currentPlayer = opponent;
}
}
        window.clear(sf::Color::Black);
        drawBoard(window, tileSize);
        drawPieces(window, tileSize);
        window.display();
        window.setFramerateLimit(60);
	
    }
   close(sock);
}

