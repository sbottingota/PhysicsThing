#include "physics_objects.hpp"

#include <iostream>
#include <cstdlib>
#include <typeinfo>


Vec2 Circle::handle_collision(std::shared_ptr<PhysicsObject> other) const {
    if (typeid(*other) == typeid(Circle)) {
        Circle &circle = static_cast<Circle&>(*other);

        // taken from here: https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
        float a = ((1 + ELASTICITY) * circle.mass) / (mass + circle.mass);
        float b = (velocity - circle.velocity).dot(position - circle.position) / (position - circle.position).length_squared();

        return velocity - a * b * (position - circle.position);
    } else if (typeid(*other) == typeid(Line)) {
        Line &line = static_cast<Line&>(*other);

        Pos2 contact = line.closest_point_to(position);
        return ELASTICITY * velocity.reflected_over(position - contact);
    }

    std::clog << "no collision-handling logic defined for '" << typeid(*this).name() << "' and '" << typeid(*other).name() << "'\n";
    std::exit(1);
}

std::vector<Axis> Circle::get_axes(const PhysicsObject &other) const {
    return {Axis(position, other.get_position())};
}

Projection Circle::project(Axis axis) const {
    Vec2 axis_vector = (axis.get_end() - axis.get_start()).normalized();
    float center_projection = position.dot(axis_vector);

    float min = center_projection - radius;
    float max = center_projection + radius;

    return Projection(min, max);
}

void Circle::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::CircleShape screen_object(radius);
    screen_object.setPosition({position.get_x() - radius, position.get_y() - radius});

    target.draw(screen_object, states);
}

float Circle::get_radius() const {
    return radius;
}

