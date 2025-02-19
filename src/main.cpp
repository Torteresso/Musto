#include <SFML/Graphics.hpp>
#include <iostream>
#include "common/events.h"
#include "configuration.h"
#include "musto/mustoApplication.h"
#include "musto/state.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);

	sf::CircleShape circle{ 12 };
	circle.setFillColor(sf::Color::Green);

    MustoApplication* mustoApplication{ new MustoApplication(new MenuState) };

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            Event::processEvents(window, event);
            mustoApplication->processEvents(event, window);
        }
        window.clear();

        mustoApplication->update();
        
        window.draw(circle);

        window.display();
    }
}
