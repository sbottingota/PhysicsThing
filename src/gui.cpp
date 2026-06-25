#include "gui.hpp"

#include <iostream>

GUI::GUI(unsigned int width, unsigned int height, std::string window_name) {
    gui_window = sf::RenderWindow(sf::VideoMode({width, height}), window_name,
        sf::Style::Titlebar | sf::Style::Close);

    if (cursor.has_value()) {
        gui_window.setMouseCursor(cursor.value());
    }
}

void GUI::add_component(std::shared_ptr<GUIComponent> component) {
    components.push_back(component);
}

void GUI::update(float dt) {
    gui_window.clear(bg_color);

    while (const std::optional event = gui_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            gui_window.close();
        } else {
            if (const auto mouse_move = event->getIf<sf::Event::MouseMoved>()) {
                int move_x = mouse_move->position.x;
                int move_y = mouse_move->position.y;

                // check whether the mouse has moved to within the bounds of a GUI element
                // and change the look of the cursor correspondingly
                sf::Cursor::Type cursor_type = default_cursor_type;
                for (auto component : components) {
                    if (component->x < move_x && move_x < component->x + component->width
                        && component->y < move_y && move_y < component->y + component->height) {
                        cursor_type = component->mouse_type();
                    }
                }

                cursor = sf::Cursor::createFromSystem(cursor_type);
                if (cursor.has_value()) {
                    gui_window.setMouseCursor(cursor.value());
                }
            }

            for (auto component : components) {
                component->handle_event(*event);
            }
        }
    }

    for (auto component : components) {
        component->update(dt);
        gui_window.draw(*component);
    }

    gui_window.display();
}

sf::RenderWindow &GUI::get_render_window() {
    return gui_window;
}

GUI create_simulation_gui() {
    GUI gui(GUI_WIDTH, GUI_HEIGHT, "Physics Thing GUI");

    auto button = std::make_shared<Button>("Add Shape", PADDING, PADDING, GUI_WIDTH - 2 * PADDING, 40);
    button->set_action([]() {std::cout << "Clicked!\n"; });
    gui.add_component(button);

    auto number_input = std::make_shared<NumberInput>(PADDING, PADDING * 4, GUI_WIDTH - 2 * PADDING, 40);
    gui.add_component(number_input);

    return gui;
}

