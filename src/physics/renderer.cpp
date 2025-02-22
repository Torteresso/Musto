#include "physics/renderer.h"

void Renderer::render(sf::RenderTarget& target)
{
	updateVertices();
	target.draw(m_vertices, &m_texture);
}

void Renderer::updateVertices()
{
	const auto& disks{ m_solver.m_disks };

	const sf::Vector2f& textureSize{ static_cast<sf::Vector2f>(m_texture.getSize()) };

	m_vertices.resize(disks.size() * 6);
	for (int i{}; i < disks.size(); i++)
	{
		const auto& disk{ *disks[i] };
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
		m_vertices[6 * i + 1].texCoords = { textureSize.x, 0.f };
		m_vertices[6 * i + 2].texCoords = textureSize;
		m_vertices[6 * i + 3].texCoords = { 0.f, 0.f };
		m_vertices[6 * i + 4].texCoords = { 0.f, textureSize.y };
		m_vertices[6 * i + 5].texCoords = textureSize;
	}
}