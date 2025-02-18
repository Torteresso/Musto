#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <numeric>
#include <SFML/System/Vector2.hpp>

namespace Utility
{
	inline float average(std::vector<float> const& v){

		if(v.empty()){
			return 0;
		}

		auto const count = static_cast<float>(v.size());
		return std::reduce(v.begin(), v.end()) / count;
	}
	
	inline float distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		return (p1 - p2).lengthSquared();
	}
	inline float distance(const sf::Vector2i& p1, const sf::Vector2f& p2)
	{
		return (static_cast<sf::Vector2f>(p1) - p2).lengthSquared();
	}
	inline float distance(const sf::Vector2f& p1, const sf::Vector2i& p2)
	{
		return (p1 - static_cast<sf::Vector2f>(p2)).lengthSquared();
	}
	inline float distance(const sf::Vector2i& p1, const sf::Vector2i& p2)
	{
		return (p1 - p2).lengthSquared();
	}

	template <typename T> 
	int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}
}

#endif