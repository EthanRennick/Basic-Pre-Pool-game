// Author: Dr Noel O'Hara
// Top down game starter
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
// I use a couple of h files from thor library.
//https://github.com/Bromeon/Thor
#include "VectorAlgebra2D.h"


#include <iostream>
#include <stdlib.h> 
#include <time.h> 
#include <list>
enum  PlayerState { ready, drawingLine, Moving };
class Game
{
public:
	//create Window
	sf::RenderWindow window;
	sf::View view;
	float randomNum = 0;

	sf::CircleShape playerShape;
	
	PlayerState playerState = ready;

	sf::Vector2f velocity = { 0,0 };
	float playerMaxSpeed = 400;

	sf::CircleShape target;




	
	
	sf::Vector2f normalised ;
	

	Game()
	{
		window.create(sf::VideoMode(800, 600), "Bouncy offy wally");
	}
	
	void init()
	{

		view = window.getDefaultView();
		playerShape.setRadius(20);
		playerShape.setPosition(160, 500);
		playerShape.setOrigin(sf::Vector2f(20, 20));
		sf::Vector2f normalised = thor::unitVector(playerShape.getPosition());
		
		float r = thor::toDegree(10.0);
		
		thor::setLength(normalised, 100.0f);
		thor::rotate(normalised, 10.0f);

		target.setFillColor(sf::Color::Yellow);
		target.setPosition(400, 300);
		target.setRadius(8);
	}
	void run()
	{
	
		sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);


		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		
		sf::Clock clock;

		clock.restart();

		while (window.isOpen())
		{
			
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}
			
			timeSinceLastUpdate += clock.restart();

			if (timeSinceLastUpdate > timePerFrame)
			{

				if (playerState == ready)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						playerState = drawingLine;
					}
				}
				if (playerState == drawingLine)
				{
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						sf::Vector2f localPosition = (sf::Vector2f)sf::Mouse::getPosition(window);
						sf::Vector2f newVelocity;
						newVelocity = localPosition - playerShape.getPosition();

						if (thor::length(newVelocity) < playerMaxSpeed)
						{
							velocity = newVelocity;
						}
						else {
							thor::setLength(newVelocity, playerMaxSpeed);
							velocity = newVelocity;
						}

						playerState = Moving;
						
					}
				}
				if (playerState == Moving)
				{	
					playerState = ready;
				}
		
				playerShape.move(velocity.x * timeSinceLastUpdate.asSeconds(), velocity.y * timeSinceLastUpdate.asSeconds());

				if (playerShape.getPosition().y > 600 - playerShape.getRadius())
				{
					playerShape.setPosition(playerShape.getPosition().x, 600 - playerShape.getRadius());
					velocity.y = velocity.y * -1;
				}
				if (playerShape.getPosition().y < 0 + playerShape.getRadius())
				{
					playerShape.setPosition(playerShape.getPosition().x, 0 + playerShape.getRadius());
					velocity.y = velocity.y * -1;
				}
				if (playerShape.getPosition().x < 0 + playerShape.getRadius())
				{
					playerShape.setPosition(0 + playerShape.getRadius(), playerShape.getPosition().y);

					velocity.x = velocity.x * -1;

				}
				if (playerShape.getPosition().x > 800 - playerShape.getRadius())
				{
					playerShape.setPosition(800 - playerShape.getRadius(), playerShape.getPosition().y);
					velocity.x = velocity.x * -1;

				}
				if (playerShape.getGlobalBounds().intersects(target.getGlobalBounds()))
				{
					target.setPosition(sf::Vector2f(rand() % 800 + 1, rand() % 600 + 1));
				}
				velocity.y = velocity.y * .995;
				velocity.x = velocity.x * .995;

				window.clear();
				if (playerState == drawingLine)
				{
					sf::Vector2i localPosition = sf::Mouse::getPosition(window);
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(localPosition.x, localPosition.y)),
						sf::Vertex(sf::Vector2f(playerShape.getPosition().x, playerShape.getPosition().y))
					};

					window.draw(line, 2, sf::Lines);
				}
				window.draw(playerShape);
				window.draw(target);
				window.display();
				
				timeSinceLastUpdate = sf::Time::Zero;
			}
		}
	}
};


int main()
{


	Game game;
	

	game.init();

	game.run();



	return 0;
}