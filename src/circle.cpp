#include "physics_objects.hpp"

#include <iostream>
#include <cstdlib>
#include <typeinfo>


Vec2 Circle::handle_collision(const PhysicsObject &other, CollisionResult collision_result) const {
    if (typeid(other) == typeid(Circle)) {
        const Circle &circle = static_cast<const Circle&>(other);

        // taken from here: https://en.wikipedia.org/wiki/Elastic_collision#Two-dimensional_collision_with_two_moving_objects
        float a = ((1 + ELASTICITY) * circle.mass) / (mass + circle.mass);
        float b = (velocity - circle.velocity).dot(position - circle.position) / (position - circle.position).length_squared();

        return velocity - a * b * (position - circle.position);
    } else if (typeid(other) == typeid(Line)) {
        const Line &line = static_cast<const Line&>(other);

        Pos2 contact = line.closest_point_to(position);
        return ELASTICITY * velocity.reflected_over(position - contact);
    }

    std::clog << "no collision-handling logic defined for '" << typeid(*this).name() << "' and '" << typeid(other).name() << "'\n";
    std::exit(1);
}

Pos2 Circle::support(Vec2 direction) const {
    return position + direction.normalized() * radius;
}

void Circle::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::CircleShape screen_object(radius);
    screen_object.setPosition({position.get_x() - radius, position.get_y() - radius});

    target.draw(screen_object, states);
}

float Circle::get_radius() const {
    return radius;
}

