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
std::vector<Axis> Polygon::get_axes(const PhysicsObject &other) const {
    std::vector<Axis> axes;
    axes.reserve(vertices.size());
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        // add axes normal to all adjacent pairs of axes
        axes.push_back(Axis::normal_to(vertices[i], vertices[(i+1) % vertices.size()]));
    }

    return axes;
}

Projection Polygon::project(Axis axis) const {
    float min = (axis.get_end() - axis.get_start()).dot(vertices[0] - axis.get_start());
    float max = min;

    for (size_t i = 0; i < vertices.size(); ++i) {
        float projected_len = (axis.get_end() - axis.get_start()).dot(vertices[i] - axis.get_start());

        if (projected_len < min) {
            min = projected_len;
        } else if (projected_len > max) {
            max = projected_len;
        }
    }

    return Projection(min, max);
}

