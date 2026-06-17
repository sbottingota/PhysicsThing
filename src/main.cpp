#include <SFML/Graphics.hpp>

#include "physics_objects.hpp"
#include "vec2.hpp"

#define WIDTH 1280
#define HEIGHT 720

int main() {
	sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Physics Thing");

    Circle sphere1(Pos2(50, 50), Vec2(50, 30), 10, 5);
    Circle sphere2(Pos2(WIDTH-50, HEIGHT-50), Vec2(-100, -60), 10, 5);

    sf::Clock clock;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
            }
		}

        sf::Time elapsed = clock.restart();
        double dt = elapsed.asSeconds();

        sphere1.update(dt);
        sphere2.update(dt);

		window.clear();
		window.draw(sphere1);
		window.draw(sphere2);
		window.display();
	}
}

