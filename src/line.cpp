#include "physics_objects.hpp"

#include <cfloat>
#include <iostream>

// set velocity and mass to placeholder values, since these don't apply to static lines
Line::Line(Pos2 point1, Pos2 point2) : PhysicsObject(point1, {0, 0}, DBL_MAX), direction(point2 - point1) {
    init_common_collision_checkers();
}

// lines are fixed in space, so a no-op
Vec2 Line::handle_collision(std::shared_ptr<PhysicsObject> other) const {
    return {0, 0};
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

