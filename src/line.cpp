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

