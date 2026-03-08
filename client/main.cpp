#include <SFML/Graphics.hpp>
#include <iostream>

int board[8][8] = {
    {0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0},
    {0,1,0,1,0,1,0,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {2,0,2,0,2,0,2,0},
    {0,2,0,2,0,2,0,2},
    {2,0,2,0,2,0,2,0}
};

int selectedX = -1;
int selectedY = -1;


int turn = 1;

int main(){
	const int tileSize = 80;  //640 % 8
	sf::RenderWindow window(sf::VideoMode(640,640), "Checkers Game");

	while(window.isOpen()){
	
		sf::Event event;

		while(window.pollEvent(event)){

			if (event.type == sf::Event::Closed){
				window.close();
			}

			if(event.type == sf::Event::MouseButtonPressed){

    				int mouseX = event.mouseButton.x / tileSize;
    				int mouseY = event.mouseButton.y / tileSize;

    				// first click → select piece
    				if(selectedX == -1 && selectedY == -1){

        			if(board[mouseY][mouseX] != 0){
            				selectedX = mouseX;
            				selectedY = mouseY;

            				std::cout << "Selected: "
                      			<< selectedX << "," << selectedY
                      			<< std::endl;
       			 }

    				}
    			else{
				int dx = mouseX - selectedX;
				int dy = mouseY - selectedY;

				if(turn == 1 && dy == 1 && abs(dx) == 1){


                                	//second click
        				board[mouseY][mouseX] = board[selectedY][selectedX];

        				board[selectedY][selectedX] = 0;
					turn = 2;
				}
				if (turn == 2 && dy == -1 && abs(dx) == 1){
					board[mouseY][mouseX] = board[selectedY][selectedX];
					board[selectedX][selectedY] = 0;
					turn = 1;
				}

				else if(turn == 1 && dy == 2 && abs(dx) == 2){

            				int midX = (selectedX + mouseX) / 2;
            				int midY = (selectedY + mouseY) / 2;

            				if(board[midY][midX] == 2){
                				board[mouseY][mouseX] = board[selectedY][selectedX];
                				board[selectedY][selectedX] = 0;
                				board[midY][midX] = 0;
               					 turn = 2;
            				}
        	               }

        			else if(turn == 2 && dy == -2 && abs(dx) == 2){

            			int midX = (selectedX + mouseX) / 2;
            			int midY = (selectedY + mouseY) / 2;

            				if(board[midY][midX] == 1){
              		      	 		board[mouseY][mouseX] = board[selectedY][selectedX];
                				board[selectedY][selectedX] = 0;
                				board[midY][midX] = 0;
                				turn = 1;
            				}
        			}


        			selectedX = -1;
        			selectedY = -1;
  			  }
			}
		}


		window.clear();
                
		for (int y = 0 ; y < 8 ; y++){ // y = rows
			for (int x = 0 ; x < 8 ; x++){  // x = cols
				sf::RectangleShape square(sf::Vector2f(tileSize,tileSize));
				square.setPosition(x * tileSize , y * tileSize);
		
				if((x + y) % 2 == 0){
					square.setFillColor(sf::Color::White);
					}else{
					square.setFillColor(sf::Color(139 , 69 , 19));
					}
					window.draw(square);
			}
		}

		for (int y = 0 ; y < 8 ; y++){
			for (int x = 0 ; x < 8 ; x++){
				if (board[y][x] == 1){
					sf::CircleShape piece(tileSize/2 - 10);
					
					if(x == selectedX && y == selectedY){
						piece.setFillColor(sf::Color(0,50,0));
					} else{
						piece.setFillColor(sf::Color(100 , 20 , 10));
					}

					piece.setPosition(x * tileSize + 10 , y * tileSize + 10);
					window.draw(piece);
				}

				if (board[y][x] == 2){
					sf::CircleShape piece(tileSize/2 - 10);


					if (x == selectedX && y == selectedY){
						piece.setFillColor(sf::Color(0,190,0));
					}

					else{
						piece.setFillColor(sf::Color::White);
					}

					piece.setPosition(x * tileSize + 10, y * tileSize + 10);
					window.draw(piece);
				}
			}
		}

		window.display();
		}
	}



