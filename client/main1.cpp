#include <SFML/Graphics.hpp>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../common/board.h"
#include "../common/move.h"
#include "graphics.h"
#include "../common/move.h"
int main()
{
    // --- TCP Client Setup ---
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) { std::cerr << "Socket creation failed\n"; return 1; }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(54000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    if(connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n"; return 1;
    }

    std::cout << "Connected to server\n";

    const int tileSize = 80;
    sf::RenderWindow window(sf::VideoMode(640,640), "Checkers");

    while(window.isOpen())
    {
        sf::Event event;

        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed)
            {
                int mouseX = event.mouseButton.x / tileSize;
                int mouseY = event.mouseButton.y / tileSize;

                Move move = handleClick(mouseX, mouseY); // Modify handleClick to return Move when valid
                if(move.fromX != -1) {
                    send(sock, &move, sizeof(move), 0);
                }
            }
        }

        // --- Receive opponent move ---
        Move oppMove;
        int bytes = recv(sock, &oppMove, sizeof(oppMove), MSG_DONTWAIT); // non-blocking
        if(bytes > 0) {
            // update board with opponent move
            applyMove(oppMove);
        }

        window.clear();
        drawBoard(window, tileSize);
        drawPieces(window, tileSize);
        window.display();
    }

    close(sock);
}
