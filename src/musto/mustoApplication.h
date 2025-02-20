#ifndef MUSTO_APPLICATION_H
#define MUSTO_APPLICATION_H

#include <SFML/Graphics.hpp>
#include "musto/state.h"

class State;

class MustoApplication
{
public:
	MustoApplication(State* state);
	~MustoApplication();

	void update();
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window);
	void transitionTo(State* state);

private:
	State* m_state;

};

#endif