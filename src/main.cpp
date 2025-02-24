#include <SFML/Graphics.hpp>
#include <iostream>
#include "common/events.h"
#include "configuration.h"
#include "musto/mustoApplication.h"
#include "musto/state.h"
#include "common/utility.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({Config::windowSize.x, Config::windowSize.y}), "CMake SFML Project", 
                                   sf::Style::Default, sf::State::Fullscreen);

    window.setFramerateLimit(Config::maxFrameRate);


    MustoApplication* mustoApplication{ new MustoApplication };
    
    sf::Clock clock;
    const float dt = 1.f / static_cast<float>(Config::maxFrameRate);
        
    std::vector<float> fpsVector{};
    if (Config::showFps) fpsVector.reserve(1000);

        
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            Event::processEvents(window, event);
            mustoApplication->processEvents(event, window);
        }

        sf::Time elapsed = clock.restart();

        if (Config::showFps)
        {
            float fps{ 1.0f / elapsed.asSeconds() };
            if (fps <= Config::maxFrameRate)
                fpsVector.push_back(fps);

            if (fpsVector.size() > 50)
            {
				std::cout << "FPS : " << Utility::average(fpsVector) << "\n";
                fpsVector.clear();
            }

        }


        window.clear({20, 20, 17});

        mustoApplication->update(dt);

        mustoApplication->draw(window);

        window.display();
    }
}
