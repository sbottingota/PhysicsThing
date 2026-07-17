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

// modifies 'simplex' and 'direction', and returns whether the simplex contains the origin
bool handle_simplex(std::vector<Pos2> &simplex, Vec2 &direction) {
    if (simplex.size() == 1) {
        direction = -simplex[0];
        return false;

    } else if (simplex.size() == 2) {
        Pos2 point_a = simplex[1];
        Pos2 point_b = simplex[0];

        Vec2 ab = point_b - point_a;
        Vec2 ao = -point_a;

        if (ab.dot(ao) <= 0) {
            simplex = {point_a};
            direction = ao;
        } else {
            direction = triple_product(ab, ao, ab);
        }

        return false;

    } else if (simplex.size() == 3) {
        Pos2 point_a = simplex[2];
        Pos2 point_b = simplex[1];
        Pos2 point_c = simplex[0];

        Vec2 ab = point_b - point_a;
        Vec2 ac = point_c - point_a;
        Vec2 ao = -point_a;

        // test region outside AB
        Vec2 normal_ab = ab.perp();
        if (normal_ab.dot(ac) > 0) {
            normal_ab = -normal_ab;
        }

        if (normal_ab.dot(ao) > 0) {
            simplex = {point_a, point_b};
            direction = triple_product(ab, ao, ab);
            return false;
        }

        // test region outside AC
        Vec2 normal_ac = ac.perp();
        if (normal_ac.dot(ab) > 0) {
            normal_ac = -normal_ac;
        }

        if (normal_ac.dot(ao) > 0) {
            simplex = {point_a, point_c};
            direction = triple_product(ac, ao, ac);
            return false;
        }

        // origin lies inside triangle
        return true;

    } else {
        // for 2D collision checking, the simplex shouldn't have more than 3 vertices
        std::clog << "Simplex is expected to have between 1 and 3 (inclusive) vertices, but had " << simplex.size() << '\n';
        std::exit(1);
    }
}

// using GJK collision checking
bool PhysicsObject::collides(const PhysicsObject &other) const {
    std::vector<Pos2> simplex;
    Vec2 direction = other.position - position;

    while (true) {
        Pos2 minkowski_diff = support(direction) - other.support(-direction);

        if (minkowski_diff.dot(direction) <= 0) {
            return false;
        }

        simplex.push_back(minkowski_diff);

        if (handle_simplex(simplex, direction)) {
            return true;
        }
    }
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

