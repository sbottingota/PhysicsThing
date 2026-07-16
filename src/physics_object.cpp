#include "physics_objects.hpp"

#include <iostream>

bool PhysicsObject::faces(const PhysicsObject &other) const {
    return is_faced_by(other) || other.is_faced_by(*this);
}

bool PhysicsObject::is_faced_by(const PhysicsObject &other) const {
    Pos2 relative_pos = position - other.get_position();
    Vec2 relative_vel = other.get_velocity() - velocity;

    return relative_pos.dot(relative_vel) > 0;
}

void PhysicsObject::update(float dt) { 
    position += dt * velocity;
}

Pos2 PhysicsObject::get_position() const {
    return position;
}

Vec2 PhysicsObject::get_velocity() const {
    return velocity;
}

void PhysicsObject::set_velocity(Vec2 new_velocity) {
    velocity = new_velocity;
}

// using SAT collision checking
bool PhysicsObject::collides(const PhysicsObject &other) const {
    std::vector<Axis> axes = get_axes(other);
    std::vector<Axis> other_axes = other.get_axes(*this);

    axes.reserve(axes.size() + other_axes.size());
    axes.insert(axes.end(), other_axes.begin(), other_axes.end());

    for (Axis &axis : axes) {
        Projection projection1 = project(axis);
        Projection projection2 = other.project(axis);

        if (!projection1.overlaps(projection2)) {
            return false; // no collision if projections do not overlap
        }
    }

    return true;
}

void PhysicsObject::draw_velocity(sf::RenderTarget &target, sf::RenderStates states) const {
    // if velocity is 0, don't draw an arrow at all
    if (velocity.length() <= 1e-5) return;

    sf::RectangleShape tail({arrow_width, velocity.length() * arrow_scale});

    Vec2 tail_offset = arrow_width/2 * Vec2(velocity.get_y(), -velocity.get_x()) / velocity.length();
    tail.setPosition({position.get_x() - tail_offset.get_x(), position.get_y() - tail_offset.get_y()});

    tail.setRotation(sf::radians(velocity.angle()) - sf::degrees(90)); // SFML angles have a different starting point, so subtract 90deg to make it line up
    tail.setFillColor(arrow_color);

    Pos2 base = position + velocity * arrow_scale;
    sf::ConvexShape head;

    head.setPosition({base.get_x(), base.get_y()});

    head.setPointCount(3);
    head.setPoint(0, {base.get_x() - arrow_width * 2, base.get_y()});
    head.setPoint(1, {base.get_x() + arrow_width * 2, base.get_y()});
    head.setPoint(2, {base.get_x(), base.get_y() + arrow_width * 2});

    head.setFillColor(arrow_color);

    head.setOrigin({base.get_x(), base.get_y()});
    head.setRotation(sf::radians(velocity.angle()) - sf::degrees(90));

    target.draw(tail);
    target.draw(head);
}

