#include "common/events.h"
#include <iostream>
#include <cmath>

bool g_mouseLeftDown{ false };

void Event::processEvents(sf::RenderWindow& window, const std::optional<sf::Event> event)
{
	if (event->is<sf::Event::Closed>())
	{
		window.close();
	}
}
