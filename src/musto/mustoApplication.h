#ifndef MUSTO_APPLICATION_H
#define MUSTO_APPLICATION_H

#include <SFML/Graphics.hpp>
#include "musto/state.h"
#include "mustoGame.h"

class State;

class MustoApplication
{
public:
	MustoApplication(State* state);
	~MustoApplication();

	void update(const float dt);
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window);
	void transitionTo(State* state);

	MustoGame m_mustoGame{};

private:
	State* m_state;


};

#endif