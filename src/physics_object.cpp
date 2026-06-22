#include "physics_objects.hpp"

bool PhysicsObject::faces(const PhysicsObject &other) const {
    return is_faced_by(other) || other.is_faced_by(*this);
}

void PhysicsObject::update(double dt) { 
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
