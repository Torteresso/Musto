#ifndef EVENTS_H
#define EVENTS_H

#include <SFML/Graphics.hpp>

namespace Event
{
	void processEvents(sf::RenderWindow& window, std::optional<sf::Event> event);
}

#endif