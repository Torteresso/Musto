#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SFML/Graphics.hpp>
#include <string_view>
#include <utility>
#include <array>
#include <map>
#include <cassert>

namespace Config
{
	const sf::Vector2u							windowSize					{ sf::VideoMode::getDesktopMode().size};
	const sf::Vector2f							windowSizef					{ static_cast<sf::Vector2f>(windowSize) };

	constexpr float								windowGameSizeRatio			{ 0.8f };

	int const									maxFrameRate				{ 60 };

	const sf::Font								font						{ sf::Font("res/pixel_font.otf") };
	constexpr std::string_view					validCharacters				{ "abcdefghijklmnopqrstuvwxyz'-." };

	constexpr sf::Vector2f						gravity						{ 0.f, 1000.f };
	constexpr float								diskRadius					{ 2 };

	constexpr std::pair<int, int>				extremumNbTry				{ 1, 12 };
	constexpr std::pair<int, int>				extremumNbLetters			{ 2, 14};
	inline int									nbTry						{ 6 };
	inline int									nbLetters					{ 7 };

	constexpr bool								showFps						{ false };

	inline std::string_view						language					{ "fr" };

}

#endif