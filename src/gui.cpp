#include "gui.hpp"

#include <iostream>

GUI::GUI(int width, int height, std::string window_name) {
    gui_window = sf::RenderWindow(sf::VideoMode({width, height}), window_name);
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

    return gui;
}

