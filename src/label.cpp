#include "gui.hpp"

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Text text_object(font);

    text_object.setPosition({bounds.x, bounds.y});
    text_object.setCharacterSize(bounds.height);
    text_object.setFillColor(text_color);
    text_object.setString(text);

    target.draw(text_object);
}

