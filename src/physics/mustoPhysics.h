#ifndef MUSTO_PHYSICS_H
#define MUSTO_PHYSICS_H

#include "physics/solver.h"
#include "physics/renderer.h"
#include "physics/rigidDisk.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <utility>

class MustoPhysics
{
public:
	MustoPhysics(Solver& solver, Renderer& renderer);

	void update(const float dt);
	void draw(sf::RenderTarget& target);

	void addLetter(const char letter, const int tryNb, const int letterNb);
	void removeLetter(const int tryNb, const int letterNb);
	
	void changeLetterColor(const sf::Color& color, const int tryNb, const int letterNb);

	void cleanAll();

private:
	void generateLettersPos();
	
	Solver& m_solver;
	Renderer& m_renderer;

	std::vector<sf::Image> m_letters;
	std::vector<std::vector<std::pair<sf::Vector2f, sf::Color>>> m_lettersPos;
	std::vector<std::vector<DiskObject*>> m_objects;
};

#endif
