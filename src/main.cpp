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


    MustoApplication* mustoApplication{ new MustoApplication(new MenuState) };
    
    const float dt = 1.f / static_cast<float>(Config::maxFrameRate);


    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            Event::processEvents(window, event);
            mustoApplication->processEvents(event, window);
        }
        window.clear({20, 20, 17});

        mustoApplication->update(dt);

        mustoApplication->draw(window);

        window.display();
    }
}
