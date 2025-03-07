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
#include <chrono>
#include <ctime>

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

	void configureNewGame(const bool daily=false);

	void update(const float dt);
	void draw(sf::RenderTarget& target);

	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window);

	const Status getStatus() const { return m_status; }
	const std::string_view getWord() const { return m_word; }

private:
	void generateWordList(std::vector<std::string>& wordList, const std::string& filename);
	void pickWord();
	void generateDailyWord();

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
	void reconfigure(const int nbTry, const int nbLetters);

	std::string_view m_word{ "" };
	std::vector<std::string> m_wordList;
	std::vector<std::string> m_allWords;

	std::string m_dailyWord{""};
	
	std::string m_currentGuess;
	std::string m_closestGuess;

	int m_gameNbTry{ Config::nbTry };
	int m_gameNbLetters{ Config::nbLetters };
	std::string_view m_gameLanguage{ Config::language };

	std::vector<std::vector<std::pair<char, LetterState>>> m_evaluatedGuesses;

	Status m_status{ InProgress };

	MustoPhysics& m_physics;
};

#endif