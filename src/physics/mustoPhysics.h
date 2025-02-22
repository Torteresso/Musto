#ifndef MUSTO_PHYSICS_H
#define MUSTO_PHYSICS_H

#include "physics/solver.h"
#include "physics/renderer.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

class MustoPhysics
{
public:
	MustoPhysics();

	void update(const float dt);
	void draw(sf::RenderTarget& target);

	void addLetter(const char letter, const sf::Vector2f& pos);

private:
	Solver m_solver{};
	Renderer m_renderer{m_solver};

	std::vector<sf::Image> m_letters;

};

#endif
