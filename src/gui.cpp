#include "gui.hpp"

#include <iostream>

void GUIComponent::set_bounds(Bounds bounds) {
    this->bounds = bounds;
}

GUIRow::GUIRow(size_t n_components) {
    components = std::vector<std::shared_ptr<GUIComponent>>(n_components, nullptr);
}

void GUIRow::add_component(std::shared_ptr<GUIComponent> component, size_t idx) {
    if (idx >= components.size()) {
        throw std::invalid_argument("Invalid index for adding component to GUIRow: expected 0 <= idx < " + std::to_string(components.size()) + "but got index of: '" + std::to_string(idx) + "'.'");
    }

    components[idx] = component;

    update_component_bounds();
}

void GUIRow::set_bounds(Bounds bounds) {
    this->bounds = bounds;
    update_component_bounds();
}

void GUIRow::update_component_bounds() const {
    float padding = padding_proportion * bounds.width;
    
    float component_width = (bounds.width - padding * (components.size()+1)) / components.size(); // padding at start, and after each component, hence components.size()+1
    float component_height = bounds.height - padding;

    for (size_t i = 0; i < components.size(); ++i) {
        if (components[i] == nullptr) continue;

        float component_x = bounds.x + component_width * i + padding * (i+1);
        float component_y = bounds.y + padding / 2;

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

std::optional<sf::Cursor::Type> GUIRow::mouse_type(float mouse_x, float mouse_y) const {
    for (auto component : components) {
        if (component->x() < mouse_x && mouse_x < component->x() + component->width()
            && component->y() < mouse_y && mouse_y < component->y() + component->height()) {
            return component->mouse_type();
        }
    }

    return {};
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

    auto [gui_width, gui_height] = gui_window.getSize();
    float row_height = gui_height / rows.size();

    for (size_t i = 0; i < rows.size(); ++i) {
        rows[i].set_bounds(Bounds(0, i * row_height, gui_width, row_height));
    }
}

void GUI::update(float dt) {
    gui_window.clear(bg_color);

    while (const std::optional event = gui_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            gui_window.close();

        } else {
            if (const auto mouse_move = event->getIf<sf::Event::MouseMoved>()) {
                int mouse_x = mouse_move->position.x;
                int mouse_y = mouse_move->position.y;

                // have the rows check whether the mouse has moved to within the bounds of a GUI element
                // and change the look of the cursor correspondingly
                sf::Cursor::Type cursor_type = default_cursor_type;
                for (GUIRow row : rows) {
                    if (auto type = row.mouse_type(mouse_x, mouse_y)) {
                        cursor_type = *type;
                        break;
                    }
                }

                cursor = sf::Cursor::createFromSystem(cursor_type);
                if (cursor.has_value()) {
                    gui_window.setMouseCursor(cursor.value());
                }
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

