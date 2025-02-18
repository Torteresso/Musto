#ifndef EVENTS_H
#define EVENTS_H

#include <SFML/Graphics.hpp>

namespace Event
{
	void processEvents(sf::RenderWindow& window, sf::View& view);

	void moveView(sf::RenderWindow& window, sf::View& view);
}

#endif