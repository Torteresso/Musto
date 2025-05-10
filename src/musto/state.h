#ifndef STATE_H
#define STATE_H

#include "musto/mustoApplication.h"
#include <iostream>
#include <array>
#include <vector>
#include <string_view>
#include <SFML/Graphics.hpp>
#include "configuration.h"
#include "common/translation.h"

class MustoApplication;

class State
{
public:
	State(MustoApplication* mustoApplication) : m_mustoApplication {mustoApplication} {}
	virtual ~State() {}

	void setMustoApplication(MustoApplication* mustoApplication) { m_mustoApplication = mustoApplication; }

	virtual void update(const float dt) = 0;
	virtual void draw(sf::RenderTarget& target) = 0;
	virtual void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) = 0;

protected:
	MustoApplication* m_mustoApplication;
};

class MenuState;
class PauseState;

class PlayingState : public State
{
public:
	PlayingState(MustoApplication* mustoApplication);

	void update(const float dt) override;
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override;
};

class PauseState : public State
{
public:
	PauseState(MustoApplication* mustoApplication);
	void update(const float dt) override;
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override;

private:
	void upSelection();
	void downSelection();

	static constexpr std::array<std::string_view, 2> m_options {"Continuer", "Menu"};

	std::vector<sf::Text> m_texts;

	int m_selection{};

	sf::RectangleShape m_background;
	sf::Text m_pauseText{ sf::Text(Config::font) };
};

class EndGameState : public State
{
public:
	EndGameState(MustoApplication* mustoApplication);
	void update(const float dt) override;
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override;

private:
	void upSelection();
	void downSelection();

	static constexpr std::array<std::string_view, 2> m_options {"Rejouer", "Menu"};

	std::vector<sf::Text> m_texts;

	int m_selection{};

	sf::RectangleShape m_background;
	sf::Text m_endGameText{ sf::Text(Config::font) };

};

class OptionState : public State
{
public:
	OptionState(MustoApplication* mustoApplication);
	void update(const float dt) override;
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override;

private:
	void upSelection();
	void downSelection();
	void increaseSelection(const int optionNumber);
	void decreaseSelection(const int optionNumber);

	static constexpr std::array<std::string_view, 4> m_options {"Nombre de lettres : ", "Nombre d'essais : ", "Langue : ", "Retour"};

	std::vector<sf::Text> m_texts;

	int m_selection{};

	sf::RectangleShape m_background;
	sf::Text m_optionText{ sf::Text(Config::font) };

};


class MenuState : public State
{
public:
	MenuState(MustoApplication* mustoApplication);

	void update(const float dt) override;
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override;
private:
	void upSelection();
	void downSelection();

	static constexpr std::array<std::string_view, 4> m_options {"Jouer", "Mot du jour", "Options", "Quitter"};

	std::vector<sf::Text> m_texts;

	sf::Text m_title{sf::Text(Config::font)};

	sf::RectangleShape m_background;
	sf::Shader m_backgroundShader;

	int m_selection{};
};


#endif