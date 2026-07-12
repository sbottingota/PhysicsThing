#include "physics_objects.hpp"

#include <numeric>

#define _USE_MATH_DEFINES
#include <cmath>

Pos2 calculate_center(std::vector<Pos2> vertices) {
    // TODO: replace with a more correct way to find center (currently only accurate for regular polygons)
    return std::accumulate(vertices.begin(), vertices.end(), Pos2(0, 0)) / vertices.size();
}

Polygon::Polygon(std::vector<Pos2> vertices, Vec2 velocity, float mass)
    : PhysicsObject(calculate_center(vertices), velocity, mass), vertices(vertices) {}

Polygon::Polygon(int n_vertices, int side_length, Pos2 center, Vec2 velocity, float mass)
    : PhysicsObject(center, velocity, mass) {
    vertices.reserve(n_vertices);

    float circumradius = side_length / (2.0 * std::sin(M_PI / n_vertices));
    float vertex_spacing = 2.0 * M_PI / n_vertices;

    for (int i = 0; i < n_vertices; ++i) {
        float vertex_angle = vertex_spacing * i;
        vertices.push_back({center.get_x() + circumradius * std::cos(vertex_angle),
            center.get_y() + circumradius * std::sin(vertex_angle)});
    }
}

void Polygon::update(float dt) {
    PhysicsObject::update(dt);

    for (Pos2 &vertex : vertices) {
        vertex = vertex.rotated(angular_velocity * dt, position);
        vertex += velocity * dt;
    }
}

// TODO: implement
Vec2 Polygon::handle_collision(std::shared_ptr<PhysicsObject> other) const {
    return {0, 0};
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::ConvexShape shape;
    shape.setPointCount(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i) {
        shape.setPoint(i, {vertices[i].get_x(), vertices[i].get_y()});
    }

    target.draw(shape);
}

// returns axes which are normal to its edges
std::vector<Axis> Polygon::get_axes() const {
    std::vector<Axis> axes;
    axes.reserve(vertices.size());
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        // add axes normal to all adjacent pairs of axes
        axes.push_back(Axis::normal_to(vertices[i], vertices[(i+1) % vertices.size()]));
    }

    return axes;
}

Pos2 Polygon::get_vertex(int idx) const {
    return vertices[idx];
}

int Polygon::get_vertex_count() const {
    return vertices.size();
}

// using SAT collision checking
std::optional<Pos2> Polygon::get_collision_point(const Polygon &other) const {
    std::vector<Axis> axes = get_axes();
    std::vector<Axis> other_axes = other.get_axes();

    axes.reserve(axes.size() + other_axes.size());
    axes.insert(axes.end(), other_axes.begin(), other_axes.end());

    for (Axis &axis : axes) {
        Projection projection1 = axis.project(*this);
        Projection projection2 = axis.project(other);

        if (!projection1.overlaps(projection2)) {
            return {}; // no collision if projections do not overlap
        }
    }

    return Pos2(0, 0); // TODO: add logic for finding collision point, rather than just whether collision occurs
}


