#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SFML/System/Vector2.hpp>
#include <string_view>

namespace Config
{
	sf::Vector2u const windowSize {1920, 1200};
	sf::Vector2f const windowSizef{ static_cast<sf::Vector2f>(windowSize) };
	int const maxFrameRate{ 120 };

	constexpr std::string_view fontFileName{ "res/pixel_font.otf" };
}

#endif