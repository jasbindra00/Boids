#include <SFML/Graphics.hpp>
#include "Flock.h"
#include <string>
float screen_width{ 1700 };
float screen_height{ 800 };
namespace sf
{
	using Vector2f = sf::Vector2<float>;
}

int main()
{
	std::vector<Slider<float>> sliders;
	sf::RenderWindow window(sf::VideoMode{ static_cast<unsigned int>(screen_width),static_cast<unsigned int>(screen_height) }, "Boids", sf::Style::Default);
	Flock flock(window);
	bool gui_hidden{ false };
	sf::Font font;
	font.loadFromFile("arial.ttf");
	{
		sf::Vector2f starting_position{ 200,50 };
		std::array<float, 6> optimal_values
		{
			0.03,
			0.025750,
			0.048,
			42.75,
			75.375,
			37.125
		};
		std::array<std::string, 3> force_names
		{
			"Alignment",
			"Cohesion",
			"Seperation"
		};
		//OPTIMISE THIS
		for (int i = 0; i < 3; ++i)
		{
			sliders.emplace_back(window, force_names.at(i) + " Force", font, 0, 0.05, optimal_values.at(i), starting_position + sf::Vector2f{ 0,static_cast<float>(50 * i) }, [&flock, i](const float& f) {flock.SetMaxBehaviouralForce(static_cast<Behaviour_Type>(i), f); });
		}
		for (int i = 3; i < 6; ++i)
		{
			sliders.emplace_back(window, force_names.at(i - 3) + " Radius", font, 0, 150, optimal_values.at(i), starting_position + sf::Vector2f{ 0,static_cast<float>(50 * i) }, [&flock, i](const float& f) {flock.SetMaxBehaviouralRadius(static_cast<Behaviour_Type>(i - 3), f); });
		}
	}
	while (window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt)) 
		{

			switch (evnt.type)
			{
				case sf::Event::EventType::Closed:
				{
					window.close();
					break;
				}
				case sf::Event::EventType::MouseButtonPressed:
				{
					
					auto mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
					for (auto& slider : sliders)
					{
						slider.HandleClick(mouse_pos); // Scrap handle event.
					}
					switch (evnt.mouseButton.button)
					{
					case sf::Mouse::Button::Left:
					{
						flock.CreateBoid(mouse_pos);
						break;
					}
					}
					break;
				}
				case sf::Event::EventType::KeyPressed:
				{
					switch (evnt.key.code)
					{
					case sf::Keyboard::Key::Space:
					{
						(gui_hidden) ? gui_hidden = false : gui_hidden = true;
						break;
					}
					case sf::Keyboard::Key::R:
					{
						for (auto& slider : sliders)
						{
							slider.ResetToDefaultVal();
						}
						break;
					}
					case sf::Keyboard::Key::H:
					{
						for (auto& slider : sliders)
						{
							slider.ToggleHidden();
						}
						break;
					}
					}
					break;
				}
			}
		}

		flock.Update();
		window.clear();
		flock.Render();
		if (!gui_hidden)
		{
			for (auto& slider : sliders)
			{
				slider.Update();
			}
			for (auto& slider : sliders)
			{
				slider.Render();
			}
		}
		window.display();
	}
}
