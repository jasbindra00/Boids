#ifndef FLOCK_H
#define FLOCK_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <array>
#include <functional>
#include "Boid.h"
#include "Behaviour.h"
#include "VectorManip.h"
#include "Slider.h"

extern float screen_width;
extern float screen_height;

//forward declarations
enum class Behaviour_Type : uint8_t
{
	Alignment, Cohesion, Seperation
};
class Flock
{
protected:
private:

	sf::RenderTarget& window;
	std::size_t total_boids{ 400 };

	float maximum_velocity{ 2 };

	std::vector<Boid> boids;
	sf::Texture boid_texture;
	sf::IntRect boid_texture_rect;
	std::array<Behaviour, 3> behaviours;


	void ApplyForces()
	{
		for (std::size_t current_boid = 0; current_boid < boids.size(); ++current_boid)
		{
			unsigned int n_neighbors{ 0 };
			Boid& b1 = boids.at(current_boid);
			for (int i = 0; i < 3; ++i)
			{
				n_neighbors = 0;
				auto& behaviour = behaviours.at(i);
				sf::Vector2f force{ 0,0 };
				/*
				
				*/
				for (std::size_t next_boid = 0; next_boid < boids.size(); ++next_boid)
				{
					if (current_boid == next_boid) continue;
					Boid& b2 = boids.at(next_boid);
					//Boid& b2 = boids.at((next_boid + current_boid) % boids.size());
					force += behaviour.CalculateBehaviouralForce(b1, b2, n_neighbors);
				}
				if (n_neighbors == 0) continue;
				force /= static_cast<float>(n_neighbors);
				if (i == 1) force -= b1.GetPosition();
				VectorManip::SetMagnitude(force, maximum_velocity);
				force -= b1.velocity;
				VectorManip::LimitMagnitude(force, behaviour.GetMaxBehaviouralForce());
				boids.at(current_boid).ApplyForce(std::move(force), maximum_velocity);
			}
		}
	}
	inline Behaviour& GetBehaviour(const Behaviour_Type& t) { return behaviours.at(static_cast<uint8_t>(t)); }
public:
	Flock(sf::RenderTarget& win)
		:window(win),
		behaviours
		(
			//"Alignment",0.018750,57,
			//"Cohesion",0.011500,100,
			//"Seperation",0.014500,49,5,

			{
				//Alignment functor
				Behaviour([](const Boid& boid_one, const Boid& boid_two)->sf::Vector2f {return boid_two.GetVelocity(); }),
				//Cohesion functor
				Behaviour([](const Boid& boid_one, const Boid& boid_two)->sf::Vector2f {return boid_two.GetPosition(); }),
				//Seperation functor
				Behaviour([](const Boid& boid_one, const Boid& boid_two)->sf::Vector2f
				{
					sf::Vector2f intra_distance = boid_one.GetPosition() - boid_two.GetPosition(); //The direction of the force must be opposite.
					return intra_distance / (VectorManip::SqMagnitude(intra_distance)); //Normalise
				}) 
			}
		)
	{
		//Load the texture
		boid_texture.loadFromFile("boid_arrow.png");
		//Init total_boids
		for (int i = 0; i < total_boids; ++i)
		{
			CreateBoid(sf::Vector2f{ VectorManip::RandomNumber<float>(0, screen_width), VectorManip::RandomNumber<float>(0,screen_height) });
		}
	}
	void Render() const
	{
		for (auto& boid : boids)
		{
			window.draw(boid);
		}
	}
	inline void CreateBoid(const sf::Vector2f& position)
	{
		boids.emplace_back(boid_texture,position, sf::Vector2f{ VectorManip::RandomNumber<float>(-maximum_velocity, maximum_velocity), VectorManip::RandomNumber<float>(-maximum_velocity, maximum_velocity) });
	}
	void Update()
	{
		for (auto& boid : boids)
		{
			boid.Move();
		}
		ApplyForces();
	}

	inline void SetMaxBehaviouralRadius(const Behaviour_Type& type, const float& rad)
	{
		GetBehaviour(type).SetBehaviouralRadius(rad);
	}
	inline void SetMaxBehaviouralForce(const Behaviour_Type& type, const float& max_force)
	{
		GetBehaviour(type).SetBehaviouralForce(max_force);
	}
	inline void SetMaxVelocity(const float& max_v) { maximum_velocity = max_v; }
	void ResetToOptimal(std::vector<Slider<float>>& sliders)
	{
// 		for (int i = 0; i < sliders.size(); ++i)
// 		{
// 			sliders.at(i).SetSliderVal(optimal_values.at(i));
// 		}
	}
};
#endif