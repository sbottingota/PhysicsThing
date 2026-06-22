#include "physics_objects.hpp"

#include <iostream>
#include <cstdlib>
#include <typeinfo>


Circle::Circle(Pos2 position, Vec2 velocity, double mass, double radius)
    : PhysicsObject(position, velocity, mass), radius(radius) {
    init_common_collision_checkers();
}

bool Circle::is_faced_by(const PhysicsObject &other) const {
    Pos2 closest_point = position.closest_point_on_line(other.get_position(), other.get_velocity());
    return (closest_point - position).length() <= radius;
}

Vec2 Circle::handle_collision(std::shared_ptr<PhysicsObject> other) const {
    if (typeid(*other) == typeid(Circle)) {
        Circle &circle = static_cast<Circle&>(*other);

        // taken from here: https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
        double a = ((1 + ELASTICITY) * circle.mass) / (mass + circle.mass);
        double b = (velocity - circle.velocity).dot(position - circle.position) / (position - circle.position).length_squared();

        return velocity - a * b * (position - circle.position);
    } else if (typeid(*other) == typeid(Line)) {
        Line &line = static_cast<Line&>(*other);

        Pos2 contact = line.closest_point_to(position);
        return ELASTICITY * velocity.reflected_over(position - contact);
    }

    std::clog << "no collision-handling logic defined for '" << typeid(*this).name() << "' and '" << typeid(*other).name() << "'\n";
    std::exit(1);
}

void Circle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::CircleShape screen_object(radius);
    screen_object.setPosition({position.getX() - radius, position.getY() - radius});

    target.draw(screen_object, states);
}

double Circle::get_radius() const {
    return radius;
}

