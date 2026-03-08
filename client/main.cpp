#include <SFML/Graphics.hpp>

int main(){
	const int tileSize = 80;  //640 % 8
	sf::RenderWindow window(sf::VideoMode(640,640), "Checkers Game");

	while(window.isOpen()){
	
		sf::Event event;
		while(window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			}
		}


		window.clear();
                
		for (int y = 0 ; y < 8 ; y++){ // y = rows
			for (int x = 0 ; x < 8 ; x++){  // x = colsi
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
		window.display();
  	}
}


