// Author: Dr Noel O'Hara
//co author: ethan rennick C00240102
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

	sf::Vector2f velocity = { -4,-400 };
	float playerMaxSpeed = 400;

	sf::CircleShape target;

	bool almostStopped = true;

	sf::RectangleShape wally;

	sf::VertexArray lines{ sf::Lines };
	
	
	
	sf::Vector2f normalised ;
	

	Game()
	{
		window.create(sf::VideoMode(800, 600), "Bouncy offy wally");
	}
	
	void init()
	{
		wally.setFillColor(sf::Color::Blue);
		wally.setPosition(600, 400);
		wally.setSize(sf::Vector2f(100,100));

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

		lines.append(sf::Vertex(sf::Vector2f(100, 100)));
		lines.append(sf::Vertex(sf::Vector2f(250, 250)));
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

				//line ball collision
				//new p1
				sf::Vector2f newP1 = { 0, 0 };
				//new p2
				sf::Vector2f newP2 = { 150, 150 };

				//new c
				sf::Vector2f newC1 = { playerShape.getPosition().x - 100, playerShape.getPosition().y - 100 };
				float angle = -thor::polarAngle(newP2); //store angle

				thor::rotate(newP2, angle); //rotate line

				sf::Vector2f tempVel = newC1 + velocity; //store velocity

				thor::rotate(newC1, angle); //rotate ball?
				thor::rotate(tempVel, angle); //rotate velocity

				tempVel = tempVel - newC1; //do something to velocity

				bool collision = false;

	
				if (tempVel.y < 0)
				{
					if (newC1.x - playerShape.getRadius() > newP2.x)
					{
						//std::cout << "non Collision";
					}
					else if (newC1.x + playerShape.getRadius() < newP1.x)
					{
						//std::cout << "no Collision";
					}
					else if (newC1.y + playerShape.getRadius() < newP1.y)
					{
						//std::cout << "No collision";
					}
					else if (newC1.y - playerShape.getRadius() > newP1.y)
					{
						//std::cout << "Collision nada";
					}
					else
					{
						collision = true;

					}
				}
				if (collision == true)
				{
					collision = false;
					newC1.y = 20;
					tempVel.y = -tempVel.y;
					tempVel = tempVel + newC1;
					thor::rotate(newC1, -angle);
					thor::rotate(tempVel, -angle);
					tempVel = tempVel - newC1;
					playerShape.setPosition(newC1.x + 100, newC1.y + 100);
					velocity = tempVel;

				}
				// && newC1.x + playerShape.getRadius() < newP2.x

				if (playerState == ready)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && almostStopped)
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
						almostStopped = false;
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
				velocity.y = velocity.y * .99;
				velocity.x = velocity.x * .99;
				if (velocity.x < 10 && velocity.y <10 && velocity.x > -10 && velocity.y > -10)
				{
					almostStopped = true;
				}

				if (playerShape.getGlobalBounds().intersects(wally.getGlobalBounds()))
				{
					velocity.x = velocity.x * -1;

				}
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

				

				window.draw(lines);
				window.draw(playerShape);
				window.draw(wally);
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