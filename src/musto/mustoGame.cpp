#include "musto/mustoGame.h"

MustoGame::MustoGame(MustoPhysics& mustoPhysics) : m_physics {mustoPhysics}
{
	generateWordList(m_wordList, "res/chosenWords_fr.txt");
	generateWordList(m_allWords, "res/allWords_fr.txt");
	configureNewGame();
}

void MustoGame::configureNewGame()
{
	cleanAll();
	pickWord();
	initGuess();
	addKnownLetters();
}

void MustoGame::cleanAll()
{
	m_closestGuess.clear();
	m_currentGuess.clear();
	m_evaluatedGuesses.clear();
	m_status = InProgress;
	m_physics.cleanAll();
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
	if (m_status != InProgress) return;

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
		if (keyReleased->scancode == sf::Keyboard::Scancode::Enter
			&& std::find(m_currentGuess.begin(), m_currentGuess.end(), '#') == m_currentGuess.end()
			&& guessIsValid())
			submitGuess();
	}


}

const bool MustoGame::guessIsValid() const
{
	return std::find(m_allWords.begin(), m_allWords.end(), m_currentGuess) != m_allWords.end();
	//return true;
}

void MustoGame::generateWordList(std::vector<std::string>& wordList, const char filename[])
{
	std::ifstream listWords{ filename };

	assert(listWords && "Impossible to load file containing words to generate from");

	std::string word{};

	while (std::getline(listWords, word))
	{
		if (word.size() == Config::nbLetters)
		{
			std::transform(word.begin(), word.end(), word.begin(), ::tolower);
			wordList.push_back(word);
		}
	}
}

void MustoGame::pickWord()
{
	m_word = m_wordList[Random::get(0, m_wordList.size() - 1)];
}

void MustoGame::addKnownLetters()
{
	for (int i{}; i < Config::nbLetters; i++)
	{
		if (m_closestGuess[i] == '#') continue;

		m_physics.addLetter(m_closestGuess[i], m_evaluatedGuesses.size(), i);
	}
}

void MustoGame::removeKnownLetters()
{
	for (int i{ 1 }; i < Config::nbLetters; i++)
	{
		if (m_closestGuess[i] == '#') continue;

		m_physics.removeLetter(m_evaluatedGuesses.size(), i);
	}
}

void MustoGame::addLetterToGuess(const char l)
{
	auto it{ std::find(m_currentGuess.begin(), m_currentGuess.end(), '#') };

	if (it == m_currentGuess.end()) return;

	if (it == m_currentGuess.begin() + 1) removeKnownLetters();

	size_t index{ static_cast<size_t>(std::distance(m_currentGuess.begin(), it)) };
	m_currentGuess[index] = l;

	m_physics.addLetter(l, m_evaluatedGuesses.size(), index);
}

void MustoGame::removeLetterFromGuess()
{
	auto it{ std::find(m_currentGuess.begin(), m_currentGuess.end(), '#') };

	if (it == m_currentGuess.begin() + 1)
	{
		addKnownLetters();
		return;
	}

	size_t index{ static_cast<size_t>(std::distance(m_currentGuess.begin(), it)) };
	
	m_physics.removeLetter(m_evaluatedGuesses.size(), index - 1);

	m_currentGuess[index - 1] = '#';
}

void MustoGame::evaluateGuess()
{
	std::string fictiveWord{ m_word };

	std::vector<std::pair<char, LetterState>> evaluatedGuess;

	evaluatedGuess.resize(Config::nbLetters, { '#', unEvaluated});

	for (int i{}; i < m_currentGuess.size(); i++)
	{
		const char l{ m_currentGuess[i] };

		if (m_word[i] == l)
		{
			evaluatedGuess[i] = { l, correct };
			m_closestGuess[i] = l;

			m_physics.changeLetterColor( {228,94,73}, m_evaluatedGuesses.size(), i);
			fictiveWord[i] = '#';
		}

	}

	for (int i{}; i < m_currentGuess.size(); i++)
	{
		if (evaluatedGuess[i].first != '#') continue;

		const char l{ m_currentGuess[i] };

		std::string::size_type letterPos{ fictiveWord.find(l) };

		if (letterPos != std::string::npos)
		{
			evaluatedGuess[i] = { l, misplaced };
			m_physics.changeLetterColor({255,189,39}, m_evaluatedGuesses.size(), i);
			fictiveWord[letterPos] = '#';
		}
		else 
		{
			evaluatedGuess[i] = { l, wrong };
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

void MustoGame::initGuess()
{
	m_currentGuess.resize(Config::nbLetters, '#');
	m_closestGuess.resize(Config::nbLetters, '#');

	m_currentGuess[0] = m_word[0];
	m_closestGuess[0] = m_word[0];
}

void MustoGame::resetGuess()
{
	std::fill(m_currentGuess.begin() + 1, m_currentGuess.begin() + Config::nbLetters, '#');
}

void MustoGame::submitGuess()
{
	evaluateGuess();
	checkStatus();

	if (m_status != InProgress) return;

	resetGuess();
	addKnownLetters();
}
