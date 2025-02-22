#ifndef RIGID_DISK_H
#define RIGID_DISK_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include "configuration.h"
#include "common/random.h"
#include <limits>

struct RigidDisk
{
private:
	static inline int s_internalIdGenerator{ 0 };

public:
	float radius{ 20 };
	sf::Vector2f pos{ 0.f, 0.f };
	sf::Vector2f oldPos{ 0.f, 0.f };
	
	sf::Color color;

	bool fixed{ false };
	bool isVirtual{ false };

	bool isDead{ false };

	int internalId{ s_internalIdGenerator++ };

	size_t next = std::numeric_limits<size_t>::max();
};

class Link
{
public:

	Link(RigidDisk& disk1, RigidDisk& disk2, const float targetDist) : m_disk1{ &disk1 }, m_disk2{ &disk2 }, m_targetDist{ targetDist }
	{

	}

	Link& operator = (const Link& link)
	{
		if (this == &link) return *this;

		m_disk1 = link.m_disk1;
		m_disk2 = link.m_disk2;
		m_targetDist = link.m_targetDist;
		m_isDead = link.m_isDead;

		if (&m_disk1 == &m_disk2) m_isDead = true;

		return *this;
	}
	
	void clean()
	{
		if (m_disk1->isDead || m_disk2->isDead)
		{
			m_isDead = true;
		}
	}

	void apply()
	{
		if (m_isDead) return;

		sf::Vector2f n{ m_disk2->pos - m_disk1->pos };
		
		float delta{ n.length() - m_targetDist };
	
		if (n.length() < 1e-6) return;

		float disk1Displacement{ 0.5f};

		if (m_disk1->fixed)
		{
			disk1Displacement = 0.f;
		}
		else if (m_disk2->fixed)
		{
			disk1Displacement = 1.f;
		}

		const float disk2Displacement{ 1.f - disk1Displacement };

		m_disk1->pos += disk1Displacement * delta * n.normalized();
		m_disk2->pos -= disk2Displacement * delta * n.normalized();


	}

	const bool isDead() const { return m_isDead; }

private:

	RigidDisk* m_disk1;
	RigidDisk* m_disk2;
	float m_targetDist{};
	
	bool m_isDead{ false };

};

class DiskObject
{
private:
	static inline int s_internalIdGenerator{ 0 };

public:
	int m_internalId;

	DiskObject(std::vector<RigidDisk*>& components, const bool fixed = true) :
		m_components{ components }, m_internalId{ s_internalIdGenerator++ }, m_fixed{ fixed }
	{
		fixed ? fix() : unfix();
	}

	void unfix()
	{
		makeLinks();
		for (auto& disk : m_components)
		{
			disk->fixed = false;
		}

	}

	void clean()
	{
		for (auto& link : m_links)
		{
			link.clean();
		}

		m_links.erase(std::remove_if(m_links.begin(), m_links.end(),
			[](const Link& link) {return link.isDead();}), m_links.end());

		m_components.erase(std::remove_if(m_components.begin(), m_components.end(),
			[](const RigidDisk* disk) {return disk->isDead;}), m_components.end());
	}

	void fix()
	{
		m_links.clear();
		for (auto& disk : m_components)
		{
			disk->fixed = true;
		}
	}

	void applyLinks()
	{
		for (auto& link : m_links)
		{
			link.apply();
		}
	}

	const bool contains(const RigidDisk& disk) const
	{
		return std::find(m_components.begin(), m_components.end(), &disk) != m_components.end();
	}

	void explode()
	{
		m_links.clear();
		for (auto& disk : m_components)
		{
			sf::Vector2f randomDir{ static_cast<float>(Random::get(-2000, 2000)), static_cast<float>(Random::get(-2000, 2000))};
			disk->oldPos += randomDir.normalized() * 5.f;
		}
	}

private:

	void makeLinks()
	{
		if (m_components.size() <= 1) return;

		const float dist{ (m_components[0]->pos - m_components[1]->pos).length() };
		m_links.push_back({ *m_components[0], *m_components[1], dist });

		if (m_components.size() == 2) return;

		for (int i{ 1 }; i < m_components.size(); i++)
		{
			auto& disk{ *m_components[i] };
			std::sort(m_components.begin(), m_components.end(),
				[disk](RigidDisk* disk1, RigidDisk* disk2)
				{
					return (disk1->pos - disk.pos).length() < (disk2->pos - disk.pos).length();
				});

			auto& closestDisk{ *m_components[1] };  //index 0 is disk
			const float distClose{ (disk.pos - closestDisk.pos).length() };

			m_links.push_back({ closestDisk, disk, distClose });

			if (m_components.size() <= 2) continue;

			auto& furthestDisk{ *m_components.back() };
			const float distFar{ (disk.pos - furthestDisk.pos).length() };

			m_links.push_back({ furthestDisk, disk, distFar });
			if (m_components.size() <= 3) continue;

			auto& closestDisk2{ *m_components[2] };  //index 0 is disk
			const float distClose2{ (disk.pos - closestDisk2.pos).length() };

			m_links.push_back({ closestDisk2, disk, distClose2 });

			if (m_components.size() <= 4) continue;

			auto& closestDisk3{ *m_components[3] };  //index 0 is disk
			const float distClose3{ (disk.pos - closestDisk3.pos).length() };

			m_links.push_back({ closestDisk3, disk, distClose3 });

			if (m_components.size() <= 6) continue;

			auto& closestDisk4{ *m_components[4] };  //index 0 is disk
			auto& furthestDisk2{ *m_components[m_components.size() - 2] };
			const float distClose4{ (disk.pos - closestDisk4.pos).length() };
			const float distFar2{ (disk.pos - furthestDisk2.pos).length() };

			m_links.push_back({ closestDisk4, disk, distClose4 });
			m_links.push_back({ furthestDisk2, disk, distFar2 });

			if (m_components.size() <= 7) continue;

			auto& middleDisk{ *m_components[static_cast<int>(m_components.size() / 2)] };
			const float distMiddle{ (disk.pos - middleDisk.pos).length() };

			m_links.push_back({ middleDisk, disk, distMiddle });
		}

	}

	bool m_fixed;
	std::vector<RigidDisk*> m_components;
	std::vector<Link> m_links;
};

#endif