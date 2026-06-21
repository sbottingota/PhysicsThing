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

    Vec2 offset = Vec2(direction.getY(), -direction.getX()) / direction.length();
    offset *= thickness / 2;

    line.setPosition({position.getX() + offset.getX(), position.getY() + offset.getY()});
    line.rotate(sf::radians(direction.angle()));

    target.draw(line);
}

Pos2 Line::closest_point_to(Pos2 point) const {
    double t = (point - position).dot(direction) / direction.length_squared();

    // clamp point between the bounds of the line
    if (t < 0) { t = 0; }
    if (t > 1) { t = 1; }

    return position + direction * t;
}

bool Line::is_faced_by(const PhysicsObject &other) const {
    Vec2 displacement = other.get_position() - closest_point_to(other.get_position());
    return displacement.dot(other.get_velocity()) < 0;
}

