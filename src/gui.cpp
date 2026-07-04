#include "gui.hpp"

#include <iostream>

void GUIComponent::set_bounds(Bounds bounds) {
    this->bounds = bounds;
}

GUIRow::GUIRow(size_t n_components) {
    components = std::vector<std::shared_ptr<GUIComponent>>(n_components, nullptr);
}

void GUIRow::add_component(std::shared_ptr<GUIComponent> component, size_t idx) {
    if (idx >= n_components) {
        throw std::invalid_argument("Invalid index for adding component to GUIRow: expected 0 <= idx < " + std::to_string(n_components) + "but got index of: '" + std::to_string(idx) + "'.'");
    }

    components[idx] = component;

    update_component_bounds();
}

void GUIRow::set_bounds(Bounds bounds) {
    bounds = bounds;
    update_component_bounds();
}

void GUIRow::update_component_bounds() const {
    float padding = padding_proportion * bounds.width;
    
    float component_width = (bounds.width - padding * (n_components+1)) / n_components; // padding at start, and after each component, hence n_components+1
    float component_height = bounds.height - padding*2;

    for (size_t i = 0; i < n_components; ++i) {
        if (components[i] == nullptr) continue;

        float component_x = bounds.x + component_width * i + padding * (i+1);
        float component_y = bounds.y + padding;

        Bounds component_bounds(component_x, component_y, component_width, component_height);
        components[i]->set_bounds(component_bounds);
    }
}

void GUIRow::update(float dt) {
    for (auto component : components) {
        component->update(dt);
    }
}

void GUIRow::handle_event(const sf::Event &event) {
    for (auto component : components) {
        component->handle_event(event);
    }
}

void GUIRow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto component : components) {
        target.draw(*component);
    }
}

GUI::GUI(unsigned int width, unsigned int height, std::string window_name) {
    gui_window = sf::RenderWindow(sf::VideoMode({width, height}), window_name,
        sf::Style::Titlebar | sf::Style::Close);

    if (cursor.has_value()) {
        gui_window.setMouseCursor(cursor.value());
    }
}

void GUI::add_row(GUIRow row) {
    rows.push_back(row);
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
                /*
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
                */
            }

            for (GUIRow row : rows) {
                row.handle_event(*event);
            }
        }
    }

    for (GUIRow row : rows) {
        row.update(dt);
        gui_window.draw(row);
    }

    gui_window.display();
}

sf::RenderWindow &GUI::get_render_window() {
    return gui_window;
}

