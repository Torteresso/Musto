#include "physics/renderer.h"

void Renderer::render(sf::RenderTarget& target, const std::vector<std::vector<std::pair<sf::Vector2f, sf::Color>>>& lettersPos)
{
	updateVertices(lettersPos);
	target.draw(m_vertices, &m_texture);
}

void Renderer::updateVertices(const std::vector<std::vector<std::pair<sf::Vector2f, sf::Color>>>& lettersPos)
{
	const auto& disks{ m_solver.m_disks };
	const int nbDisks{ static_cast<int>(disks.size())};

	const sf::Vector2f& textureSize{ static_cast<sf::Vector2f>(m_texture.getSize()) };

	if (lettersPos.size() != 0) m_vertices.resize(nbDisks * 6 + lettersPos[0].size() * lettersPos.size() * 6);
	else m_vertices.resize(nbDisks * 6);

	if (lettersPos.size() != 0)
	{
		const float letterSize{ std::min(Config::windowSizef.x * Config::windowGameSizeRatio / (lettersPos[0].size() * 1.5f),
									Config::windowSizef.y * Config::windowGameSizeRatio/ (lettersPos.size() * 1.5f)) };


		const float spaceBetweenBoxes{ 0.9f };
		const float boxSize{ letterSize * 1.5f * spaceBetweenBoxes };

		for (int j{}; j < lettersPos.size() * lettersPos[0].size(); j++)
		{
			const int currentTryNb{ static_cast<int>(j / lettersPos[0].size())};
			const int currentLetterNb{ static_cast<int>(j- currentTryNb * lettersPos[0].size())};

			const sf::Vector2f pos{ lettersPos[currentTryNb][currentLetterNb].first 
												- sf::Vector2f{(boxSize - letterSize)/2.f, (boxSize - letterSize)/2.f} };
			const sf::Color& color{ lettersPos[currentTryNb][currentLetterNb].second };

			m_vertices[6 * j + 0].position = pos;
			m_vertices[6 * j + 1].position = pos + sf::Vector2f{ boxSize, 0.f };
			m_vertices[6 * j + 2].position = pos + sf::Vector2f{ boxSize, boxSize };
			m_vertices[6 * j + 3].position = pos;
			m_vertices[6 * j + 4].position = pos + sf::Vector2f{ 0.f, boxSize };
			m_vertices[6 * j + 5].position = pos + sf::Vector2f{ boxSize, boxSize };

			m_vertices[6 * j + 0].color = color;
			m_vertices[6 * j + 1].color = color;
			m_vertices[6 * j + 2].color = color;
			m_vertices[6 * j + 3].color = color;
			m_vertices[6 * j + 4].color = color;
			m_vertices[6 * j + 5].color = color;

			m_vertices[6 * j + 0].texCoords = { textureSize.x / 2.f, 0.f };
			m_vertices[6 * j + 1].texCoords = { textureSize.x, 0.f };
			m_vertices[6 * j + 2].texCoords = textureSize;
			m_vertices[6 * j + 3].texCoords = { textureSize.x / 2.f, 0.f};
			m_vertices[6 * j + 4].texCoords = { textureSize.x / 2.f, textureSize.y};
			m_vertices[6 * j + 5].texCoords = textureSize;
		}
	}

	for (int i{ static_cast<int>(lettersPos.size() * lettersPos[0].size())}; i < nbDisks + lettersPos.size() * lettersPos[0].size(); i++)
	{
		const auto& disk{ *disks[i - lettersPos.size() * lettersPos[0].size()]};
		const auto& pos{ disk.pos };
		const auto& color{ disk.color };
		const float radius{ disk.radius };

		m_vertices[6 * i + 0].position = pos + sf::Vector2(-radius, -radius);
		m_vertices[6 * i + 1].position = pos + sf::Vector2(radius, -radius);
		m_vertices[6 * i + 2].position = pos + sf::Vector2(radius, radius);
		m_vertices[6 * i + 3].position = pos + sf::Vector2(-radius, -radius);
		m_vertices[6 * i + 4].position = pos + sf::Vector2(-radius, radius);
		m_vertices[6 * i + 5].position = pos + sf::Vector2(radius, radius);

		m_vertices[6 * i + 0].color = color;
		m_vertices[6 * i + 1].color = color;
		m_vertices[6 * i + 2].color = color;
		m_vertices[6 * i + 3].color = color;
		m_vertices[6 * i + 4].color = color;
		m_vertices[6 * i + 5].color = color;

		m_vertices[6 * i + 0].texCoords = { 0.f, 0.f };
		m_vertices[6 * i + 1].texCoords = { textureSize.x / 2.f, 0.f };
		m_vertices[6 * i + 2].texCoords = { textureSize.x / 2.f, textureSize.y };
		m_vertices[6 * i + 3].texCoords = { 0.f, 0.f };
		m_vertices[6 * i + 4].texCoords = { 0.f, textureSize.y };
		m_vertices[6 * i + 5].texCoords = { textureSize.x / 2.f, textureSize.y };

	}
}