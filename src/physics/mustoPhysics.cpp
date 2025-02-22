#include "physics/mustoPhysics.h"

MustoPhysics::MustoPhysics()
{
	const sf::Image allLetters("res/letters.png"); //each letter in letters.png are of size 50*50 pixels

	const int letterSize{ static_cast<int>(allLetters.getSize().y) };

	for (int i{}; i < Config::validCharacters.size() - 1; i++) //Last caracter is space, we skip it
	{
		sf::Image letter(sf::Vector2u{ static_cast<unsigned>(letterSize), static_cast<unsigned>(letterSize) });
		assert(letter.copy(allLetters, { 0, 0 }, { {i * letterSize, 0}, { letterSize, letterSize } }));

		m_letters.push_back(letter);
	}
}

void MustoPhysics::update(const float dt)
{
	m_solver.update(dt);
}

void MustoPhysics::draw(sf::RenderTarget& target)
{
	m_renderer.render(target);
}


void MustoPhysics::addLetter(const char letter, const sf::Vector2f& pos)
{
	std::string::size_type letterPos{ Config::validCharacters.find(letter)};

	assert(letterPos != std::string::npos);

	const sf::Image& image{ m_letters[letterPos] };
	float ratio{ Config::windowSizef.y / (image.getSize().y * Config::nbTry)};

	const int cellSize{ m_solver.getGrid().getCellSize() };


	const int imgNbRow{ static_cast<int>(image.getSize().y *  ratio / cellSize) };
	const int imgNbCol{ static_cast<int>(image.getSize().x * ratio / cellSize) };

	for (int i{}; i < imgNbRow; i++)
	{
		for (int j{}; j < imgNbCol; j++)
		{
			const sf::Vector2u imageCoord{ static_cast<unsigned>((j + 0.5f) * cellSize / ratio), 
										   static_cast<unsigned>((i + 0.5f) * cellSize / ratio)};

			const sf::Color pixelColor{ image.getPixel(imageCoord) };

			if (pixelColor.toInteger() == 0) continue;

			sf::Vector2f currentPos{ pos.x + (j + 0.5f) * cellSize, pos.y + (i + 0.5f) * cellSize };

			m_solver.addDiskForObject(Config::diskRadius, currentPos, currentPos, sf::Color::White);
		}
	}

	m_solver.addObject();

}
