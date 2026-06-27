#include "gui.hpp"

Label::Label(std::string text, float x, float y) : GUIComponent(x, y, 0, 0) {
    text_object.setPosition({x, y});
    text_object.setFillColor(text_color);
    text_object.setString(text);
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(text_object);
}

