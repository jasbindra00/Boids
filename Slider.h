#ifndef SLIDER_H
#define SLIDER_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Flock.h"
#include <functional>

template<typename T>
class Slider
{
private:
	using Applier = std::function<void(float)>;
	static sf::Font font;
	static const int slider_height{ 13 };
	static const int slider_width{ 200 };
	const sf::Color slider_color{ sf::Color::Red };

	sf::RenderWindow& target;

	sf::RectangleShape background;
	sf::RectangleShape slider;

	std::string slider_name;
	sf::Text text;
	bool clicked{ false };
	mutable bool hidden{ false };

	T default_val;
	T current_val;
	T min_val;
	T max_val;

	Applier applier;
	void ApplySliderValue()
	{
		text.setString(slider_name + " : " + std::to_string(current_val));
		text.setOrigin(static_cast<float>(text.getLocalBounds().width / 2), static_cast<float>(text.getLocalBounds().height));
		text.setPosition(background.getPosition() + sf::Vector2f{ background.getSize().x / 2, -1 });
		applier(current_val);
	}
protected:
public:
	Slider()
	{
		slider.setSize(sf::Vector2f{ 7,18 });
		slider.setFillColor(sf::Color::White);

		text.setFillColor(sf::Color::White);
		text.setCharacterSize(slider_height + 3);
	}

	Slider(sf::RenderWindow& window,const std::string& name, const sf::Font& font, const T& min, const T& max, const T& default_v,const sf::Vector2f& position, Applier a)
		:target(window),
		slider_name(name),
		min_val(min),
		max_val(max),
		default_val(default_v),
		applier(a)

	{
		background.setFillColor(sf::Color::Red);
		background.setPosition(position);
		background.setSize(sf::Vector2f{ slider_width, static_cast<float>(slider_height) });
		
		slider.setSize(sf::Vector2f{ 7,18 });
		slider.setFillColor(sf::Color::White);
		slider.setPosition(position);

		text.setFont(font);
		text.setString(slider_name);
		text.setFillColor(sf::Color::White);
		text.setCharacterSize(slider_height + 3);
		ResetToDefaultVal();
	}
	void HandleClick(const sf::Vector2f& mouse_pos)
	{
		if (hidden) return;
		if (clicked) clicked = false;
		else if (background.getGlobalBounds().contains(mouse_pos) || slider.getGlobalBounds().contains(mouse_pos))
		{
			clicked = true;
		}
	}
	void Update()
	{
		if (clicked)
		{
			auto mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(target));
			sf::Vector2f background_position(background.getPosition());
			if (mouse_position.x < background_position.x) return;
			if (mouse_position.x > background_position.x + background.getSize().x) return;
			slider.setPosition(mouse_position.x, slider.getPosition().y);
			current_val = ((max_val - min_val) / background.getSize().x) * (slider.getPosition().x - background.getPosition().x) + min_val;
			ApplySliderValue();
		}
	}
	void Render()
	{
		if (hidden) return;
		target.draw(background);
		target.draw(text);
		target.draw(slider);
	}
	T GetSliderVal() const
	{
		return current_val;
		
	}
	void SetSliderVal(const T& val)
	{
		if (val < min_val || val > max_val) return;
		current_val = val;
		float posx = ((val - min_val) / ((max_val - min_val) / background.getSize().x)) + background.getPosition().x;
		slider.setPosition(sf::Vector2f{ std::move(posx), slider.getPosition().y });
		ApplySliderValue();
	}
	void ResetToDefaultVal()
	{
		SetSliderVal(default_val);
	}
	inline void ToggleHidden() const noexcept { (hidden) ? hidden = false : hidden = true; }
};

#endif