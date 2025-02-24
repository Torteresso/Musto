#include "physics/solver.h"
#include <iostream>
#include <cmath>

Solver::Solver()
{
	m_diskPool = ObjectPool<RigidDisk>(2 * m_grid.size());
	m_diskObjectPool = ObjectPool<DiskObject>(m_grid.size()/100);
	addInitialConfig();
}

void Solver::addInitialConfig()
{
	
}

void Solver::update(const float dt)
{
	const int nbSubStep{ 8 };
	const float subDt{ dt / static_cast<float>(nbSubStep) };
	for (int i{}; i < nbSubStep; i++)
	{
		updateGrid();
		solveLinks();
		solveCollision();
		updatePos(subDt);
		m_grid.clear();
		clean();
	}
}

void Solver::clean()
{
	for (int i{}; i < m_disks.size(); i++)
	{
		if (!m_disks[i]->isDead) continue;

		for (auto& diskObj : m_diskObjects)
		{
			if (!diskObj->contains(*m_disks[i])) continue;


			diskObj->clean();

			break;
		}

		m_diskPool.Free(m_disks[i]);

		auto it = m_disks.begin();
        std::advance(it, i);
        m_disks.erase(it);
	}

	for (int j{}; j < m_diskObjects.size(); j++)
	{
		if (!m_diskObjects[j]->isDead()) continue;

		m_diskObjectPool.Free(m_diskObjects[j]);
		
		auto it = m_diskObjects.begin();
        std::advance(it, j);
        m_diskObjects.erase(it);

	}
}

void Solver::solveLinks()
{
	for (auto& diskObj : m_diskObjects)
	{
		diskObj->applyLinks();
	}
}

void Solver::updateGrid()
{
	for (int i {}; i < m_disks.size(); i++)
	{
		m_grid.addDisk(*m_disks[i], i);
	}
}

void Solver::solveCollision()
{
	const int nbCol{ m_grid.getNbCol()};
	for (int k{}; k < m_grid.size(); k++)
	{
		const auto& currentCell{ m_grid[k] };
		for (int j{}; j < currentCell.diskCount; j++)
		{
			const int i{ currentCell.disks[j] };
			if (m_grid.idexIsValid(k + nbCol)) solveDiskCellCollision(i, m_grid[k + nbCol]);
			if (m_grid.idexIsValid(k + nbCol - 1)) solveDiskCellCollision(i, m_grid[k + nbCol - 1]);
			if (m_grid.idexIsValid(k + nbCol + 1)) solveDiskCellCollision(i, m_grid[k + nbCol + 1]);
			if (m_grid.idexIsValid(k)) solveDiskCellCollision(i, m_grid[k]);
			if (m_grid.idexIsValid(k - 1)) solveDiskCellCollision(i, m_grid[k - 1]);
			if (m_grid.idexIsValid(k + 1)) solveDiskCellCollision(i, m_grid[k + 1]);
			if (m_grid.idexIsValid(k - nbCol)) solveDiskCellCollision(i, m_grid[k - nbCol]);
			if (m_grid.idexIsValid(k - nbCol - 1)) solveDiskCellCollision(i, m_grid[k - nbCol - 1]);
			if (m_grid.idexIsValid(k - nbCol + 1)) solveDiskCellCollision(i, m_grid[k - nbCol + 1]);
		}
	}
}

void Solver::solveDiskCollision(const int id1, const int id2)
{
	auto& disk1{ *m_disks[id1] };
	auto& disk2{ *m_disks[id2] };

	if (disk1.isVirtual || disk2.isVirtual) return;

	sf::Vector2f n{ disk2.pos - disk1.pos };

	const float delta = n.length() - disk1.radius - disk2.radius;

	if (delta > 0) return;

	//if (n.length() < 1e-6) n = sf::Vector2f{ 1.f, 1.f };
	if (n.length() < 1e-6) return;

	float disk1Displacement{ 0.5f};

	if (disk1.fixed)
	{
		disk1Displacement = 0.f;
	}
	else if (disk2.fixed)
	{
		disk1Displacement = 1.f;
	}

	const float disk2Displacement{ 1.f - disk1Displacement };

	disk1.pos += disk1Displacement * delta * n.normalized();
	disk2.pos -= disk2Displacement * delta * n.normalized();
}

void Solver::solveDiskCellCollision(const int id, const Cell& cell)
{
	for (int i{}; i < cell.diskCount; i++)
	{
		const int idBis{ cell.disks[i]};
		if (idBis != id) solveDiskCollision(id, idBis);
	}
}

void Solver::updatePos(const float dt)
{
	for (auto& diskPtr : m_disks)
	{
		auto& disk{ *diskPtr };

		if (disk.fixed) continue;

		sf::Vector2f newOldPos{ disk.pos };

		const float VELOCITY_DAMPING = 40.0f;

		disk.pos += disk.pos - disk.oldPos + (Config::gravity - (disk.pos - disk.oldPos) * VELOCITY_DAMPING) * dt * dt;
		disk.oldPos = newOldPos;

		applyConstraints(disk);
	}
}

void Solver::applyConstraints(RigidDisk& disk)
{
	const float safety{ 2.f };

	if (disk.pos.y - 4 * disk.radius + safety >= (m_grid.getNbRow() - 2 * m_grid.getPadding()) * m_grid.getCellSize())
	{
		disk.isDead = true;
	}
	else if (disk.pos.y - disk.radius - safety <= 0)
	{
		disk.pos.y = disk.radius + safety;
	}

	if (disk.pos.x + disk.radius + safety >= (m_grid.getNbCol() - 2 * m_grid.getPadding()) * m_grid.getCellSize())
	{
		disk.pos.x = (m_grid.getNbCol() - 2 * m_grid.getPadding()) * m_grid.getCellSize() - disk.radius - safety;
	}

	else if (disk.pos.x - disk.radius - safety <= 0)
	{
		disk.pos.x = disk.radius + safety;
	}
}


void Solver::addDisk(float radius, const sf::Vector2f& pos, const sf::Vector2f& oldPos, 
					 const sf::Color& color, const bool fixed, const bool isVirtual)
{
	sf::Vector2f newPos{ pos };
	sf::Vector2f newOldPos{ oldPos };

	if (fixed)
	{
		const int cellId{ m_grid.posToIndex(pos) };
		if (m_occupied.count(cellId))
		{
			return;
		}

		m_occupied.insert(cellId);

		newPos = m_grid.getCellMiddlePos(cellId);
		newOldPos = newPos;
	}

	const int diskId{ static_cast<int>(m_disks.size()) };

		
	RigidDisk* disk = m_diskPool.Alloc();

	disk->radius = radius;
	disk->pos = newPos;
	disk->oldPos = newOldPos;
	disk->color = color;
	disk->fixed = fixed;
	disk->isVirtual = isVirtual;

	m_disks.emplace_back(disk);

	if (diskId % 1000 == 0) std::cout << "Number of disks : " << m_disks.size() << "\n";
}

void Solver::addDiskForObject(float radius, const sf::Vector2f& pos, const sf::Vector2f& oldPos, const sf::Color& color)
{
	const int diskId{ static_cast<int>(m_disks.size()) };

	addDisk(radius, pos, oldPos, color, true, true);

	if (m_disks.size() == diskId) return;  //No new DISK has been added

	m_objDiskComponents.push_back(m_disks.back());
}

DiskObject* Solver::addObject()
{
	for (auto& disk : m_objDiskComponents)
	{
		const int cellId{ m_grid.posToIndex(disk->pos)};
		m_occupied.erase(cellId);

		disk->isVirtual = false;
		disk->fixed = false;
	}

	DiskObject* diskObj = m_diskObjectPool.Alloc();

	diskObj->initialize(m_objDiskComponents);

	m_diskObjects.emplace_back(diskObj);

	m_objDiskComponents.clear();

	return m_diskObjects.back();
}

void Solver::clear()
{
	m_grid.clear();
	m_disks.clear();
	m_diskPool.clear();
	m_diskObjectPool.clear();
	m_objDiskComponents.clear();
	m_diskObjects.clear();
	m_occupied.clear();
	addInitialConfig();
}