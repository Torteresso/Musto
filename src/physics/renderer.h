#ifndef RENDERER_H
#define RENDERER_H

#include "SFML/Graphics.hpp"
#include <utility>
#include "physics/solver.h"

class Renderer
{
public:
	Renderer() = delete;
	Renderer(Solver& solver) : m_solver {solver}, m_vertices{sf::PrimitiveType::Triangles}
	{
		if (!m_texture.loadFromFile("res/circle_and_box.png")) return;
		m_texture.setSmooth(true);
	}

	void render(sf::RenderTarget& target, const std::vector<std::vector<std::pair<sf::Vector2f, sf::Color>>>& lettersPos = {});

private:
	void updateVertices(const std::vector<std::vector<std::pair<sf::Vector2f, sf::Color>>>& lettersPos = {});
	Solver& m_solver;

	sf::VertexArray m_vertices;
	sf::Texture m_texture;
};

#endif