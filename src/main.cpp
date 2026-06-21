#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "physics_objects.hpp"
#include "vec2.hpp"

#define WIDTH 1280
#define HEIGHT 720

int main() {
	sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Physics Thing");

    Group group;
    group.add_object(std::make_shared<Circle>(Pos2(100, 100), Vec2(150, 150), 10, 20));
    group.add_object(std::make_shared<Circle>(Pos2(WIDTH - 55, HEIGHT - 55), Vec2(-300, -75), 10, 50));

    // border
    group.add_object(std::make_shared<Line>(Pos2(0, 0), Pos2(0, HEIGHT)));
    group.add_object(std::make_shared<Line>(Pos2(0, 0), Pos2(WIDTH, 0)));
    group.add_object(std::make_shared<Line>(Pos2(0, HEIGHT), Pos2(WIDTH, HEIGHT)));
    group.add_object(std::make_shared<Line>(Pos2(WIDTH, 0), Pos2(WIDTH, HEIGHT)));

    group.add_object(std::make_shared<Line>(Pos2(WIDTH/2, 0), Pos2(WIDTH, HEIGHT/3)));

    sf::Clock clock;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
            }
		}

        sf::Time elapsed = clock.restart();
        double dt = elapsed.asSeconds();
        group.update(dt);

		window.clear();
		window.draw(group);
		window.display();
	}
}

