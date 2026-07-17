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

struct SupportPoint {
    Pos2 point_a, point_b;
    Pos2 minkowski;
};

SupportPoint minkowski_diff(const PhysicsObject &shape1, const PhysicsObject &shape2, Vec2 direction) {
    Pos2 a = shape1.support(direction);
    Pos2 b = shape2.support(-direction);

    return {a, b, a - b};
}

// modifies 'simplex' and 'direction', and returns whether the simplex contains the origin
bool handle_simplex(std::vector<SupportPoint> &simplex, Vec2 &direction) {
    if (simplex.size() == 1) {
        direction = -simplex[0].minkowski;
        return false;

    } else if (simplex.size() == 2) {
        Pos2 point_a = simplex[1].minkowski;
        Pos2 point_b = simplex[0].minkowski;

        Vec2 ab = point_b - point_a;
        Vec2 ao = -point_a;

        if (ab.dot(ao) <= 0) {
            simplex = {simplex[1]};
            direction = ao;
        } else {
            direction = triple_product(ab, ao, ab);
        }

        return false;

    } else if (simplex.size() == 3) {
        Pos2 point_a = simplex[2].minkowski;
        Pos2 point_b = simplex[1].minkowski;
        Pos2 point_c = simplex[0].minkowski;

        Vec2 ab = point_b - point_a;
        Vec2 ac = point_c - point_a;
        Vec2 ao = -point_a;

        // test region outside AB
        Vec2 normal_ab = ab.perp();
        if (normal_ab.dot(ac) > 0) {
            normal_ab = -normal_ab;
        }

        if (normal_ab.dot(ao) > 0) {
            simplex = {simplex[2], simplex[1]};
            direction = triple_product(ab, ao, ab);
            return false;
        }

        // test region outside AC
        Vec2 normal_ac = ac.perp();
        if (normal_ac.dot(ab) > 0) {
            normal_ac = -normal_ac;
        }

        if (normal_ac.dot(ao) > 0) {
            simplex = {simplex[2], simplex[0]};
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

struct Edge {
    int index;
    Vec2 normal;
    float distance;

    Edge() : index(0), normal(Vec2(0, 0)), distance(std::numeric_limits<float>::max()) {} // init dummy values

    static Edge find_closest(const std::vector<SupportPoint> &polytope) {
        Edge closest;

        for (size_t i = 0; i < polytope.size(); ++i) {
            Vec2 a = polytope[i].minkowski;
            Vec2 b = polytope[(i+1) % polytope.size()].minkowski;

            Vec2 normal = (b - a).perp().normalized();
            float distance = normal.dot(a);
            if (distance < 0) {
                normal = -normal;
                distance = -distance;
            }

            if (distance < closest.distance) {
                closest.index = i;
                closest.normal = normal;
                closest.distance = distance;
            }
        }

        return closest;
    }
};

// EPA to determine collision normal and penetration
CollisionResult get_collision_result(const PhysicsObject &shape1, const PhysicsObject &shape2, std::vector<SupportPoint> polytope) {
    const float tolerance = 1e-3;

    while (true) {
        Edge closest = Edge::find_closest(polytope);
        SupportPoint point = minkowski_diff(shape1, shape2, closest.normal);

        float distance = point.minkowski.dot(closest.normal);
        if (distance - closest.distance < tolerance) {
            return {closest.normal, distance};
        }

        polytope.insert(polytope.begin() + closest.index + 1, point);
    }
}

// using GJK collision checking
std::optional<CollisionResult> PhysicsObject::collides(const PhysicsObject &other) const {
    std::vector<SupportPoint> simplex;
    Vec2 direction = other.position - position;

    while (true) {
        SupportPoint support_point = minkowski_diff(*this, other, direction);

        if (support_point.minkowski.dot(direction) <= 0) {
            return {};
        }

        simplex.push_back(support_point);

        if (handle_simplex(simplex, direction)) {
            return get_collision_result(*this, other, simplex);
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

