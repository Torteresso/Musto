#ifndef STATE_H
#define STATE_H

#include "musto/mustoApplication.h"
#include <iostream>
#include <array>
#include <vector>
#include <string_view>
#include <SFML/Graphics.hpp>
#include "configuration.h"

class MustoApplication;

class State
{
public:
	void setMustoApplication(MustoApplication* mustoApplication);
	virtual ~State() {}

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
	void update(const float dt) override;
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override;
};

class PauseState : public State
{
public:
	PauseState();
	void update(const float dt) override;
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override;

private:
	void upSelection();
	void downSelection();

	static constexpr std::array<std::string_view, 2> m_options {"Continue", "Menu"};

	std::vector<sf::Text> m_texts;
	const sf::Font m_font{ sf::Font("res/pixel_font.otf") };

	int m_selection{};

	sf::RectangleShape m_background;
	sf::Text m_pauseText{ sf::Text(m_font) };
};

class MenuState : public State
{
public:
	MenuState();

	void update(const float dt) override;
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window) override;
private:
	void upSelection();
	void downSelection();

	static constexpr std::array<std::string_view, 2> m_options {"Play", "Quit"};

	std::vector<sf::Text> m_texts;
	const sf::Font m_font{ sf::Font("res/pixel_font.otf") };

	sf::Text m_title{sf::Text(m_font)};

	int m_selection{};
};


#endif