#ifndef BOID_H
#define BOID_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <cstdint>
#include "VectorManip.h"
extern float screen_width;
extern float screen_height;
class Boid : public sf::Drawable
{
	friend class Flock;
private:
protected:
	sf::Sprite boid;
	sf::Vector2f velocity;
public:
	Boid(sf::Texture& texture, const sf::Vector2f& initpos, const sf::Vector2f& initvel) 
		:velocity(initvel)
	{	
		boid.setPosition(initpos);
		//boid.setFillColor(sf::Color::White);
		//Don't set the origin
		boid.setTexture(texture);
		sf::Vector2f desired_size{ 10,10 };
		boid.setScale(desired_size.x / texture.getSize().x, desired_size.y / texture.getSize().y);
		boid.setOrigin(boid.getLocalBounds().width / 2, boid.getLocalBounds().height / 2);

	}
	inline virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const override
	{
		target.draw(boid);
	}
	inline void ApplyForce(const sf::Vector2f& f, const float& max_v)
	{
		velocity += f;
	
	}
	void Move()
	{
		boid.move(velocity);
		sf::Vector2f boid_position(boid.getPosition());

		if (boid_position.x < 0) boid.setPosition(screen_width, std::move(boid_position.y));
		else if (boid_position.x > screen_width) boid.setPosition(0, std::move(boid_position.y));
		else if (boid_position.y < 0) boid.setPosition(std::move(boid_position.x), screen_height);
		else if (boid.getPosition().y > screen_height) boid.setPosition(std::move(boid_position.x), 0);

		float rotation = atan(velocity.y/velocity.x) * (180 / 3.14);
		//Y co-ordinate is flipped for canvas!
		if ((velocity.y < 0 && velocity.x > 0) || (velocity.y > 0 && velocity.x > 0))
		{
			rotation = 90 + rotation;
		}
		else if ((velocity.y > 0 && velocity.x < 0) || (velocity.y < 0 && velocity.x < 0)) // tan is negative 3rd quadrant
		{
			rotation = 270 + rotation;
		}
		boid.setRotation(rotation);
	}
	inline sf::Vector2f GetPosition() const noexcept { return boid.getPosition(); }
	inline sf::Vector2f GetVelocity() const noexcept { return velocity; }
};
#endif