#ifndef GRID_H
#define GRID_H

#include <vector>
#include "array"
#include <SFML/Graphics.hpp>
#include "physics/rigidDisk.h"
#include "configuration.h"
#include <iostream>

struct Cell
{
	static constexpr int capacity{ 4 };
	static constexpr int maxIndex{ 3 };

	int id{};

	int diskCount{ 0 };
	std::array<int, capacity> disks {};

	Cell() = default;

	void addDisk(const int index)
	{
		disks[diskCount] = index;
		if (diskCount > 3) std::cout << id << ", " << diskCount << "wow\n";
		diskCount += diskCount < maxIndex;
	}

	void clear()
	{
		diskCount = 0;
	}
};

class Grid
{
	using DiskGrid = std::vector<Cell>;

	static constexpr float m_cellSize{ 2 * Config::diskRadius };
	static constexpr int m_padding{ 1 };
	const int m_nbRow{static_cast<int>(Config::windowSize.y / (2*Config::diskRadius)) + 2 * m_padding};
	const int m_nbCol{static_cast<int>(Config::windowSize.x / (2*Config::diskRadius)) + 2 * m_padding};

public:
	Grid()
	{
		m_disks.resize(m_nbCol * m_nbRow);

		for (int i{}; i < m_nbCol * m_nbRow; i++)
		{
			m_disks[i].id = i;
		}
	}

	void addDisk(RigidDisk& disk, const int id)
	{
		if (disk.pos.y - Config::diskRadius >= Config::windowSize.y) return;
		m_disks[posToIndex(disk.pos)].addDisk(id);
	}

	void clear()
	{
		for (auto& elt : m_disks) elt.clear();
	}
	
	const int size() const
	{
		return m_disks.size();
	}

	const int sizeWithoutPadding() const
	{
		return (m_nbCol - 2 * m_padding) * (m_nbRow - 2 * m_padding);
	}

	Cell& operator[] (int index)
	{
		return m_disks[index];
	}

	const int getNbCol() const{ return m_nbCol; }
	const int getNbRow() const{ return m_nbRow; }
	const int getCellSize() const { return m_cellSize; }
	const int getPadding() const { return m_padding; }

	const sf::Vector2f getCellMiddlePos(const int index)
	{
		float x{ (index % m_nbCol - 0.5f) * m_cellSize };
		float y{ (index / m_nbCol - 0.5f) * m_cellSize };

		return { x, y };
	}

	const int coordToIndex(const int x, const int y)
	{
		return x + y * m_nbCol;
	}

	const bool idexIsValid(const int i)
	{
		return (i >= 0 && i < m_disks.size());
	}

	const int posToIndex(const sf::Vector2f& pos)
	{
		return (static_cast<int>(pos.y / m_cellSize) + m_padding) * m_nbCol + static_cast<int>(pos.x / m_cellSize) + m_padding;
	}

private:

	DiskGrid m_disks;
};

#endif
