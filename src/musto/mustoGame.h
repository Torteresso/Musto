#ifndef MUSTO_GAME_H
#define MUSTO_GAME_H

#include <string_view>
#include <vector>
#include <cassert>
#include "common/random.h"
#include <fstream>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <utility>

class MustoGame
{
	enum LetterState
	{
		correct,
		misplaced,
		wrong,
	};

	enum Status
	{
		Lost,
		Won,
		InProgress,
	};

public:
	MustoGame();

	void update(const float dt);
	void draw(sf::RenderTarget& target);

	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window);

private:
	void generateWordList(const char filename[]);
	void pickWord();

	void addLetterToGuess(const char l);
	void removeLetterFromGuess();

	void evaluateGuess();
	void clearGuess();
	void checkStatus();
	void submitGuess();

	std::string_view m_word;
	std::vector<std::string> m_wordList;
	
	std::vector<char> m_currentGuess;
	std::vector<std::vector<std::pair<char, LetterState>>> m_evaluatedGuesses;

	Status m_status{ InProgress };

	static constexpr std::string_view m_validCharacters{ "abcdefghijklmnopqrstuvwxyz'-. " };
	static constexpr int m_nbTry{ 6 };
};

#endif