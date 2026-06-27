#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "physics_objects.hpp"
#include "vec2.hpp"
#include "gui.hpp"

#define WIDTH 1280
#define HEIGHT 720

#define GUI_WIDTH 300
#define GUI_HEIGHT 720

#define PADDING (GUI_WIDTH / 15)
#define INPUT_WIDTH (GUI_WIDTH/2 - 2*PADDING)

GUI create_simulation_gui(Group &simulation_group) {
    GUI gui(GUI_WIDTH, GUI_HEIGHT, "Physics Thing GUI");

    auto add_circle_label = std::make_shared<Label>("Add Circle:", PADDING, PADDING);
    gui.add_component(add_circle_label);

    auto x_input = std::make_shared<NumberInput>(PADDING, PADDING * 4, INPUT_WIDTH, 40);
    gui.add_component(x_input);

    auto y_input = std::make_shared<NumberInput>(2*PADDING + INPUT_WIDTH, PADDING * 4, INPUT_WIDTH, 40);
    gui.add_component(y_input);

    auto button = std::make_shared<Button>("Add Circle", PADDING, PADDING * 8, GUI_WIDTH - 2 * PADDING, 40);
    button->set_action([=]() mutable {
        std::optional<float> x = x_input->get_number();
        std::optional<float> y = y_input->get_number();

        if (x.has_value() && y.has_value()) {
            auto circle = std::make_shared<Circle>(Pos2(x.value(), y.value()), Vec2(0, 0), 10, 50);
            simulation_group.add_object(circle);
        }
    });
    gui.add_component(button);

    return gui;
}


int main() {
	sf::RenderWindow simulation(sf::VideoMode({WIDTH, HEIGHT}), "Physics Thing");

    Group group;

    GUI gui = create_simulation_gui(group);
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

