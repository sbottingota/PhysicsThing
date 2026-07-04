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
            if (bounds.x < click_x && click_x < bounds.x + bounds.width
                && bounds.y < click_y && click_y < bounds.y + bounds.height) {
                action();

                pressed_time = animation_time;
            }
        }
    } }

void Button::update(float dt) {
    if (pressed_time > 0) {
        pressed_time -= dt;
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape button({bounds.width, bounds.height});
    button.setPosition({bounds.x, bounds.y});

    if (pressed_time <= 0) {
        button.setFillColor(fg_color);
    } else {
        button.setFillColor(interacted_color);
    }

    target.draw(button);

    sf::Text text_object(font, text);
    text_object.setPosition({bounds.x, bounds.y});
    text_object.setFillColor(text_color);
    text_object.setCharacterSize(font_size_multiplier * bounds.height);
    target.draw(text_object);
}

