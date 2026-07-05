#include "gui.hpp"

#include <iostream>

// for the sake of convenience, since I'll be parsing a lot of key presses
using Scancode = sf::Keyboard::Scancode;

std::optional<char> scancode_to_digit(Scancode scancode) {
    if (scancode == Scancode::Num0 || scancode == Scancode::Numpad0) return '0';
    if (scancode == Scancode::Num1 || scancode == Scancode::Numpad1) return '1';
    if (scancode == Scancode::Num2 || scancode == Scancode::Numpad2) return '2';
    if (scancode == Scancode::Num3 || scancode == Scancode::Numpad3) return '3';
    if (scancode == Scancode::Num4 || scancode == Scancode::Numpad4) return '4';
    if (scancode == Scancode::Num5 || scancode == Scancode::Numpad5) return '5';
    if (scancode == Scancode::Num6 || scancode == Scancode::Numpad6) return '6';
    if (scancode == Scancode::Num7 || scancode == Scancode::Numpad7) return '7';
    if (scancode == Scancode::Num8 || scancode == Scancode::Numpad8) return '8';
    if (scancode == Scancode::Num9 || scancode == Scancode::Numpad9) return '9';

    return {};
}

std::optional<float> NumberInput::get_number() const {
    if (text.empty()) {
        return {};
    } else {
        return std::stof(text);
    }
}

void NumberInput::handle_event(const sf::Event &event) {
    if (const auto mouse_press = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse_press->button == sf::Mouse::Button::Left) {
            int click_x = mouse_press->position.x;
            int click_y = mouse_press->position.y;

            // set in_focus to whether the mouse click is inside the bounding box
            in_focus = (bounds.x < click_x && click_x < bounds.x + bounds.width
                && bounds.y < click_y && click_y < bounds.y + bounds.height);
        }
    } else if (const auto *keypress = event.getIf<sf::Event::KeyPressed>()) {
        if (keypress && in_focus) {
            // if backspace is pressed, delete the last charcter in the string
            if (keypress->scancode == Scancode::Backspace && !text.empty()) {
                text.pop_back();

            // if period is pressed, add it only if the string doesn't already contain a decimal point
            } else if (keypress->scancode == Scancode::Period && text.find('.') == std::string::npos) {
                text.push_back('.');

            // otherwise add the key only if it is a digit
            } else if (std::optional<char> digit = scancode_to_digit(keypress->scancode)) {
                text.push_back(*digit);
            }
        }
    }
}

void NumberInput::update(float dt) {
    box.setPosition({bounds.x, bounds.y});
    box.setSize({bounds.width, bounds.height});

    box.setFillColor(bg_color);
    box.setOutlineThickness(border_thickness);
    box.setOutlineColor(border_color);

    text_object.setPosition({bounds.x, bounds.y});
    text_object.setFillColor(text_color);
    text_object.setCharacterSize(font_size_multiplier * bounds.height);

    text_object.setString(text);

    // make sure that the resulting text can actually fit within the box
    while (text_object.getLocalBounds().size.x >= box.getLocalBounds().size.x) {
        text.pop_back();
        text_object.setString(text);
    }

}

void NumberInput::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(box);

    if (!text.empty()) {
        target.draw(text_object);
    }
}

