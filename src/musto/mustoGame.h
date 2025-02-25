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
#include "configuration.h"
#include "physics/mustoPhysics.h"
#include <algorithm>
#include <iterator>

class MustoGame
{
	enum LetterState
	{
		correct,
		misplaced,
		wrong,
		unEvaluated,
	};

public:

	enum Status
	{
		Lost,
		Won,
		InProgress,
	};

	MustoGame(MustoPhysics& mustophysics);

	void configureNewGame();

	void update(const float dt);
	void draw(sf::RenderTarget& target);

	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window);

	const Status getStatus() const { return m_status; }
	const std::string_view getWord() const { return m_word; }

private:
	void generateWordList(std::vector<std::string>& wordList, const char filename[]);
	void pickWord();

	void addKnownLetters();
	void removeKnownLetters();
	void addLetterToGuess(const char l);
	void removeLetterFromGuess();

	void evaluateGuess();
	void initGuess();
	void resetGuess();
	void checkStatus();
	void submitGuess();
	const bool guessIsValid() const;

	void cleanAll();

	std::string_view m_word{ "" };
	std::vector<std::string> m_wordList;
	std::vector<std::string> m_allWords;
	
	std::string m_currentGuess;
	std::string m_closestGuess;

	std::vector<std::vector<std::pair<char, LetterState>>> m_evaluatedGuesses;

	Status m_status{ InProgress };

	MustoPhysics& m_physics;
};

#endif