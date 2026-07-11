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
    group.add_object(std::make_shared<Circle>(Pos2(WIDTH / 2, HEIGHT / 3), Vec2(-50, -75), 5, 25));
    group.add_object(std::make_shared<Circle>(Pos2(WIDTH / 2, 75), Vec2(10, 90), 15, 75));

    // polygons
    group.add_object(std::make_shared<Polygon>(3, 100, Pos2(WIDTH * 3.0/4.0, HEIGHT / 3), Vec2(20, 50), 5));
    group.add_object(std::make_shared<Polygon>(6, 50, Pos2(WIDTH * 3.0/4.0, HEIGHT * 2.0/3.0), Vec2(-10, -30), 10));
    group.add_object(std::make_shared<Polygon>(std::vector({Pos2(10, 10), Pos2(10, 50), Pos2(50, 50), Pos2(50, 10)}), Vec2(20, 20), 10));

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

