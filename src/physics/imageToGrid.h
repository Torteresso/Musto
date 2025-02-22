#ifndef IMAGE_TO_GRID_H
#define IMAGE_TO_GRID_H

#include <vector>
#include <string_view>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physics/grid.h"

namespace ImageToGrid
{	
	inline void imageToLetter(std::string_view filename, Solver& solver, const sf::Vector2f& pos, const float ratio)
	{
		sf::Image image{ filename };

		const int cellSize{ solver.getGrid().getCellSize() };

		const int imgNbRow{ static_cast<int>(image.getSize().y * ratio / cellSize) };
		const int imgNbCol{ static_cast<int>(image.getSize().x * ratio / cellSize) };

		for (int i{}; i < imgNbRow; i++)
		{
			for (int j{}; j < imgNbCol; j++)
			{
				const sf::Vector2u imageCoord{ static_cast<unsigned>((j + 0.5f) * cellSize / ratio), 
											   static_cast<unsigned>((i + 0.5f) * cellSize / ratio)};

				const sf::Color pixelColor{ image.getPixel(imageCoord) };

				if (pixelColor == sf::Color::Black) continue;

				sf::Vector2f currentPos{ pos.x + (j + 0.5f) * cellSize, pos.y + (i + 0.5f) * cellSize };

				solver.addDiskForObject(Config::diskRadius, currentPos, currentPos, sf::Color::White);
			}
		}

		solver.addObject();
	}
}

#endif