#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
#include "Boid.h"
#include "Slider.h"
#include <functional>
class Behaviour
{
	/*
	-The movement of each boid is governed by three behaviours : 
		-Alignment
			-Every boid wants to go at the same average velocity of its immediate neighbors.
		-Cohesion
			-Every boid wants to go to the centre of its nearest flock mates.
		-Seperation
			-Every boid wants to be distanced by the surrounding boids (so as to not collide with each other)
	*/
private:

	using Behavioural_Applier = std::function<sf::Vector2f(const Boid&, const Boid&)>;
	Behavioural_Applier applier; //A custom functor which is called when a neighboring boid is in range for the behaviour to take effect.
	const std::string behaviour_name;
	float maximum_force{ 0 };
	float maximum_radius{ 0 };
public:
	Behaviour(Behavioural_Applier pred)
		: applier(pred)
	{

// 		radius_slider.InitSlider(behaviour_name + " Radius", 0, 150, optimal_radius);
// 		force_slider.InitSlider(behaviour_name + " Force", 0, 0.05, optimal_force);
	}

	inline const float& GetBehaviouralRadius() const noexcept { return maximum_radius; }
	inline const float& GetMaxBehaviouralForce() const noexcept { return maximum_force; }
	inline void SetBehaviouralForce(const float& val) { maximum_force = val; }
	inline void SetBehaviouralRadius(const float& rad) { maximum_radius = rad; }
	inline std::string GetBehaviourName() const noexcept { return behaviour_name; }
	sf::Vector2f CalculateBehaviouralForce(const Boid& boid_one, const Boid& boid_two, unsigned int& n_neighbors)
	{
		//Two boids must be within the behavioural_radius for them to feel the effects.
		if (VectorManip::SqMagnitude(boid_one.GetPosition() - boid_two.GetPosition()) > pow(maximum_radius, 2)) return sf::Vector2f{ 0,0 }; //Not within range.
		++n_neighbors;
		return applier(boid_one, boid_two);
	}

};
#endif