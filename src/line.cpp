#include "physics_objects.hpp"

#include <cfloat>
#include <iostream>

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

std::vector<Axis> Line::get_axes(const PhysicsObject &other) const {
    return {Axis::normal_to(position, position + direction)};
}

Projection Line::project(Axis axis) const {
    Vec2 axis_vector = (axis.get_end() - axis.get_start()).normalized();
    float a = position.dot(axis_vector);
    float b = (position + direction).dot(axis_vector);
    return Projection(std::min(a, b), std::max(a, b));
}

