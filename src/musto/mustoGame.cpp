#include "musto/mustoGame.h"

MustoGame::MustoGame(MustoPhysics& mustoPhysics) : m_physics {mustoPhysics}
{
	generateWordList("res/liste_francais.txt");
	pickWord();
	clearGuess();
}
void MustoGame::update(const float dt)
{
	m_physics.update(dt);
}

void MustoGame::draw(sf::RenderTarget& target)
{
	m_physics.draw(target);
}

void MustoGame::processEvents(std::optional<sf::Event> event, sf::RenderWindow& window)
{
	if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
	{
		if (textEntered->unicode < 128)
		{
			const char letter = static_cast<char>(textEntered->unicode);
			if (Config::validCharacters.contains(letter))
			{
				addLetterToGuess(letter);
			}
			else if (textEntered->unicode == 8) //BACKSPACE
			{
				removeLetterFromGuess();
			}
		}
	}
	else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
	{
		if (keyReleased->scancode == sf::Keyboard::Scancode::Enter && m_word.size() == m_currentGuess.size())
			submitGuess();
		else if (keyReleased->scancode == sf::Keyboard::Scancode::G) m_physics.addLetter('g',
			{ static_cast<float>(Random::get(0, 1000)), static_cast<float>(Random::get(0, 1000))});
	}


}

void MustoGame::generateWordList(const char filename[])
{
	std::ifstream listWords{ filename };

	assert(listWords && "Impossible to load file containing words to generate from");

	std::string word{};

	while (std::getline(listWords, word))
	{
		m_wordList.push_back(word);
	}
}

void MustoGame::pickWord()
{
	m_word = m_wordList[Random::get(0, m_wordList.size() - 1)];
}

void MustoGame::addLetterToGuess(const char l)
{
	if (m_currentGuess.size() < m_word.size()) m_currentGuess.push_back(l);
}
void MustoGame::removeLetterFromGuess()
{
	if (m_currentGuess.size() <= 1) return;

	m_currentGuess.pop_back();
}

void MustoGame::evaluateGuess()
{
	std::string fictiveWord{ m_word };

	std::vector<std::pair<char, LetterState>> evaluatedGuess;

	for (int i{}; i < m_currentGuess.size(); i++)
	{
		const char l{ m_currentGuess[i] };

		std::string::size_type letterPos{ fictiveWord.find(l) };

		if (letterPos != std::string::npos)
		{
			if (fictiveWord[i] == l) evaluatedGuess.push_back({ l, correct });
			else evaluatedGuess.push_back({ l, misplaced });

			fictiveWord.erase(letterPos, 1);
		}
		else 
		{
			evaluatedGuess.push_back({ l, wrong });
		}
	}

	m_evaluatedGuesses.push_back(evaluatedGuess);
}

void MustoGame::checkStatus()
{
	bool win{ true };
	for (const auto& pair : m_evaluatedGuesses.back())
	{
		if (pair.second == correct) continue;

		win = false;
		break;
	}

	if (win) m_status = Won;
	else if (m_evaluatedGuesses.size() >= Config::nbTry) m_status = Lost;
	else m_status = InProgress;
	
}

void MustoGame::clearGuess()
{
	m_currentGuess.clear();
	m_currentGuess.push_back(m_word[0]);
}

void MustoGame::submitGuess()
{
	evaluateGuess();
	clearGuess();

	checkStatus();
}
