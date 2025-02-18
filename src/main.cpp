#include <SFML/Graphics.hpp>
#include <iostream>
#include "common/events.h"
#include "configuration.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);

    sf::View mainView = window.getDefaultView();

	sf::CircleShape circle{ 11 };
	circle.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        Event::processEvents(window, mainView);
        Event::moveView(window, mainView);

        window.clear();
        
        window.setView(mainView);
        window.draw(circle);

        window.display();
    }
}
