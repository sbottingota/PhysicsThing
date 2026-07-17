#include "physics_objects.hpp"

#include <cfloat>
#include <iostream>

// lines are fixed in space, so a no-op
Vec2 Line::handle_collision(std::shared_ptr<PhysicsObject> other) const {
    return {0, 0};
}

Pos2 Line::support(Vec2 direction) const {
    Pos2 a = position;
    Pos2 b = position + this->direction;

    if (a.dot(direction) > b.dot(direction)) {
        return a;
    } else {
        return b;
    }
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape line({direction.length(), thickness});

    Vec2 offset = Vec2(direction.get_y(), -direction.get_x()) / direction.length();
    offset *= thickness / 2;

    line.setPosition({position.get_x() + offset.get_x(), position.get_y() + offset.get_y()});
    line.rotate(sf::radians(direction.angle()));

    target.draw(line);
}

Pos2 Line::closest_point_to(Pos2 point) const {
    return point.closest_point_on_line_clamped(position, direction);
}

bool Line::is_faced_by(const PhysicsObject &other) const {
    Vec2 displacement = other.get_position() - closest_point_to(other.get_position());
    return displacement.dot(other.get_velocity()) < 0;
}

