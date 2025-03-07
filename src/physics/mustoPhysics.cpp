#include "physics/mustoPhysics.h"

MustoPhysics::MustoPhysics(Solver& solver, Renderer& renderer) : m_solver {solver}, m_renderer {renderer}
{
	const sf::Image allLetters("res/letters.png"); //each letter in letters.png are of size 50*50 pixels

	const int letterSize{ static_cast<int>(allLetters.getSize().y) };

	for (int i{}; i < Config::validCharacters.size() - 1; i++) //Last caracter is space, we skip it
	{
		sf::Image letter(sf::Vector2u{ static_cast<unsigned>(letterSize), static_cast<unsigned>(letterSize) });
		if (!letter.copy(allLetters, { 0, 0 }, { {i * letterSize, 0}, { letterSize, letterSize } }))
			assert("error in copying letters.png");

		m_letters.push_back(letter);
	}

	generateObjects();

	generateLettersPos();
}

void MustoPhysics::generateObjects()
{
	m_objects.clear();

	for (int i{}; i < m_nbTry; i++)
	{
		std::vector<DiskObject*> currentObjs;
		currentObjs.resize(m_nbLetters, nullptr);

		m_objects.push_back(currentObjs);
	}

}

void MustoPhysics::update(const float dt)
{
	m_solver.update(dt);
}

void MustoPhysics::draw(sf::RenderTarget& target)
{
	m_renderer.render(target, m_lettersPos);
}


void MustoPhysics::addLetter(const char letter, const int tryNb, const int letterNb)
{
	if (m_objects[tryNb][letterNb] != nullptr) removeLetter(tryNb, letterNb);

	const sf::Vector2f& pos{ m_lettersPos[tryNb][letterNb].first };

	std::string::size_type letterPos{ Config::validCharacters.find(letter) };

	assert(letterPos != std::string::npos);

	const sf::Image& image{ m_letters[letterPos] };

	float ratioX{ Config::windowSizef.x * Config::windowGameSizeRatio / (image.getSize().x * m_nbLetters * 1.5f) };
	float ratioY{ Config::windowSizef.y * Config::windowGameSizeRatio / (image.getSize().y * m_nbTry * 1.5f) };

	float ratio{ std::min(ratioX, ratioY) };

	const int cellSize{ m_solver.getGrid().getCellSize() };


	const int imgNbRow{ static_cast<int>(image.getSize().y * ratio / cellSize) };
	const int imgNbCol{ static_cast<int>(image.getSize().x * ratio / cellSize) };

	for (int i{}; i < imgNbRow; i++)
	{
		for (int j{}; j < imgNbCol; j++)
		{
			const sf::Vector2u imageCoord{ static_cast<unsigned>((j + 0.5f) * cellSize / ratio),
										   static_cast<unsigned>((i + 0.5f) * cellSize / ratio) };

			const sf::Color pixelColor{ image.getPixel(imageCoord) };

			if (pixelColor.toInteger() == 0) continue;

			sf::Vector2f currentPos{ pos.x + (j + 0.5f) * cellSize, pos.y + (i + 0.5f) * cellSize };

			m_solver.addDiskForObject(Config::diskRadius, currentPos, currentPos, sf::Color::White);
		}
	}

	m_objects[tryNb][letterNb] = m_solver.addObject();

}

void MustoPhysics::generateLettersPos()
{
	m_lettersPos.clear();

	const float letterSize{ std::min(Config::windowSizef.x * Config::windowGameSizeRatio / (m_nbLetters * 1.5f), 
							Config::windowSizef.y * Config::windowGameSizeRatio / (m_nbTry * 1.5f)) };
	
	for (int nTry{}; nTry < m_nbTry; nTry++)
	{
		std::vector<std::pair<sf::Vector2f, sf::Color>> currentTryLettersPos;
		for (int nLetter{}; nLetter < m_nbLetters; nLetter++)
		{
			const float startingX{Config::windowSizef.x / 2 - letterSize * 1.5f * m_nbLetters / 2 + letterSize / 4};
			const float x{ startingX + letterSize * nLetter * 1.5f };

			const float startingY{Config::windowSizef.y / 2 - letterSize * 1.5f * m_nbTry / 2 + letterSize/4};
			const float y{ startingY + letterSize * nTry * 1.5f };

			currentTryLettersPos.push_back({ { x, y }, {48,143,245} });
		}

		m_lettersPos.push_back(currentTryLettersPos);
	}

}



void MustoPhysics::removeLetter(const int tryNb, const int letterNb)
{
	if (m_objects[tryNb][letterNb] == nullptr) return;
	m_objects[tryNb][letterNb]->explode();
	m_objects[tryNb][letterNb] = nullptr;
}

void MustoPhysics::changeLetterColor(const sf::Color& color, const int tryNb, const int letterNb)
{
	assert(m_objects[tryNb][letterNb] != nullptr);
	m_lettersPos[tryNb][letterNb].second = color;
}

void MustoPhysics::cleanAll()
{
	for (int nTry{}; nTry < m_nbTry; nTry++)
	{
		for (int nLetter{}; nLetter < m_nbLetters; nLetter++)
		{
			m_lettersPos[nTry][nLetter].second = { 48,143,245 };
			if (m_objects[nTry][nLetter] != nullptr) m_objects[nTry][nLetter]->remove();
			m_objects[nTry][nLetter] = nullptr;
		}
	}
}

void MustoPhysics::reconfigure(const int nbTry, const int nbLetters)
{
	m_nbTry = nbTry;
	m_nbLetters = nbLetters;
	generateLettersPos();
	for (auto& objectList : m_objects)
	{
		for (auto& object : objectList)
		{
			if (object != nullptr) object->remove();
			object = nullptr;
		}
	}
	generateObjects();
}
