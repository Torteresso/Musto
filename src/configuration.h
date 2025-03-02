#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SFML/System/Vector2.hpp>
#include <string_view>
#include <utility>
#include <array>
#include <map>
#include <cassert>

namespace Config
{
	constexpr sf::Vector2u windowSize{ 1920, 1200 };
	constexpr sf::Vector2f windowSizef{ static_cast<sf::Vector2f>(windowSize) };
	int const maxFrameRate{ 60 };

	const sf::Font font{ sf::Font("res/pixel_font.otf") };
	constexpr std::string_view validCharacters{ "abcdefghijklmnopqrstuvwxyz'-." };

	constexpr sf::Vector2f						gravity{ 0.f, 1000.f };
	constexpr float								diskRadius{ 2 };

	constexpr std::pair<int, int>		extremumNbTry{ 1, 12 };
	constexpr std::pair<int, int>		extremumNbLetters{ 2, 14};
	inline int									nbTry{ 6 };
	inline int									nbLetters{ 7 };

	constexpr bool								showFps{ true };

	inline std::string_view						language{ "fr" };

}

#endif