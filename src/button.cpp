#include "gui.hpp"

#include <iostream>

void Button::set_action(std::function<void()> action) {
    this->action = action;
}

void Button::handle_event(const sf::Event &event) {
    if (const auto mouse_press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse_press->button == sf::Mouse::Button::Left && pressed_time <= 0) {
            int click_x = mouse_press->position.x;
            int click_y = mouse_press->position.y;

            // if the click event is within the bounds of the button, run the corresponding action
            if (x < click_x && click_x < x + width
                && y < click_y && click_y < y + height) {
                action();

                pressed_time = animation_time;
            }
        }
    } else if (const auto mouse_move = event.getIf<sf::Event::MouseMoved>()) {
        int move_x = mouse_move->position.x;
        int move_y = mouse_move->position.y;

        // check whether the mouse has moved to within the bounds of the button
        // and change the look of the cursor correspondingly
        if (x < move_x && move_x < x + width
            && y < move_y && move_y < y + height) {
            cursor_type = sf::Cursor::Type::Hand;
        } else {
            cursor_type = sf::Cursor::Type::Arrow;
        }
    }
}

void Button::update(float dt, GUI &gui) {
    if (pressed_time > 0) {
        pressed_time -= dt;
    }

    if (cursor_type.has_value()) {
        if (const auto cursor = sf::Cursor::createFromSystem(*cursor_type)) {
            gui.get_render_window().setMouseCursor(*cursor);
        }

        cursor_type = {};
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape button({width, height});
    button.setPosition({x, y});

    if (pressed_time <= 0) {
        button.setFillColor(fg_color);
    } else {
        button.setFillColor(interacted_color);
    }

    target.draw(button);

    sf::Text text_object(font, text);
    text_object.setPosition({x, y});
    text_object.setFillColor(text_color);
    target.draw(text_object);
}

