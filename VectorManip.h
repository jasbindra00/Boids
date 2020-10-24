#ifndef VECTORMANIP_H
#define VECTORMANIP_H
#include <SFML/Graphics/Transformable.hpp>
#include <random>
namespace VectorManip
{
	static std::mt19937 random_generator;
	inline float static SqMagnitude(const sf::Vector2f& v) {
		return (v.x * v.x) + (v.y * v.y);
	}
	void static SetMagnitude(sf::Vector2f& vec, const float& newmagnitude) {
		float currentMag = sqrt(SqMagnitude(vec));
		vec /= currentMag;
		vec *= newmagnitude;
	}
	void static LimitMagnitude(sf::Vector2f& vec, const float& limit) {
		float magnitudesq = SqMagnitude(vec);
		if (magnitudesq > limit * limit) SetMagnitude(vec, limit);
	}
	template<typename T>
	T RandomNumber(const T& lower_bound, const T& upper_bound)
	{
		std::uniform_real_distribution<T> dist(lower_bound, upper_bound);
		return dist(random_generator);
	}

}




#endif