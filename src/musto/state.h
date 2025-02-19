#ifndef STATE_H
#define STATE_H

#include "musto/mustoApplication.h"
#include <iostream>

class MustoApplication;

class State
{
public:
	void setTusmo(MustoApplication* tusmo) 
	{ 
		m_tusmo = tusmo; 
	}

	virtual ~State() {}

	virtual void update() = 0;
	virtual void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) = 0;

protected:
	MustoApplication* m_tusmo;
};

class PlayingState : public State
{
public:
	void update() override
	{

	}

	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override
	{

	}
};


class PauseState : public State
{
public:
	void update() override
	{

	}
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override
	{

	}
};

class MenuState : public State
{
public:
	void update() override
	{

	}

	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override
	{
		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyReleased->scancode == sf::Keyboard::Scancode::I)
				std::cout << "INFORMATION : Your are in state MENU\n";
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) window.close();
		}
	}

};


#endif