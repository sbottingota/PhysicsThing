#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "physics_objects.hpp"
#include "vec2.hpp"
#include "simulation.hpp"

#define WIDTH 1280
#define HEIGHT 720

int main() {
	sf::RenderWindow simulation(sf::VideoMode({WIDTH, HEIGHT}), "Physics Thing");

    Group group;

    GUI gui = create_simulation_gui(600, group);
    sf::RenderWindow &gui_window = gui.get_render_window();

    // balls
    group.add_object(std::make_shared<Circle>(Pos2(100, 100), Vec2(150, 150), 10, 50));
    group.add_object(std::make_shared<Circle>(Pos2(WIDTH - 55, HEIGHT - 55), Vec2(-300, -75), 10, 50));
    group.add_object(std::make_shared<Circle>(Pos2(WIDTH / 2, HEIGHT / 3), Vec2(-50, -75), 5, 25));
    group.add_object(std::make_shared<Circle>(Pos2(WIDTH / 2, 75), Vec2(10, 90), 15, 75));

    // border
    group.add_object(std::make_shared<Line>(Pos2(0, 0), Pos2(0, HEIGHT)));
    group.add_object(std::make_shared<Line>(Pos2(0, 0), Pos2(WIDTH, 0)));
    group.add_object(std::make_shared<Line>(Pos2(0, HEIGHT), Pos2(WIDTH, HEIGHT)));
    group.add_object(std::make_shared<Line>(Pos2(WIDTH, 0), Pos2(WIDTH, HEIGHT)));

    group.add_object(std::make_shared<Line>(Pos2(WIDTH/2, 0), Pos2(WIDTH, HEIGHT/3)));

    sf::Clock clock;

	while (simulation.isOpen() && gui_window.isOpen()) {
		while (const std::optional event = simulation.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				simulation.close();
                gui_window.close();
            }
		}

        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        group.update(dt);

		simulation.clear();
		simulation.draw(group);
		simulation.display();

        gui.update(dt);
	}
}

