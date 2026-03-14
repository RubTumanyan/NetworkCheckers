#include <SFML/Graphics.hpp>
#include "../common/board.h"
#include "graphics.h"

int main()
{
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

                handleClick(mouseX, mouseY);
            }
        }

        window.clear();

        drawBoard(window, tileSize);
        drawPieces(window, tileSize);

        window.display();
    }
}
