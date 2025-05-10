#ifndef MUSTO_APPLICATION_H
#define MUSTO_APPLICATION_H

#include <SFML/Graphics.hpp>
#include "musto/state.h"
#include "mustoGame.h"
#include "physics/mustoPhysics.h"
#include "physics/renderer.h"
#include "physics/solver.h"

class State;

class MustoApplication
{
public:
	MustoApplication();
	~MustoApplication();

	void update(const float dt);
	void draw(sf::RenderTarget& target);
	void processEvents(std::optional<sf::Event> event, sf::RenderWindow& window);
	void transitionTo(State* state);

	Solver m_solver{};
	Renderer m_renderer{m_solver};

	MustoPhysics m_mustoPhysics{m_solver, m_renderer};
	MustoGame m_mustoGame{m_mustoPhysics};

	sf::Clock m_clock;
private:
	State* m_state;



};

#endif