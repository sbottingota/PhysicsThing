#include "physics_objects.hpp"

#include <iostream>
#include <cstdlib>
#include <typeinfo>

bool Circle::collides(std::shared_ptr<PhysicsObject> other) const {
    if (!faces(other)) {
        return false;
    }

    if (typeid(other) == typeid(Circle)) {
        Circle &sphere = static_cast<Circle&>(*other);

        return (sphere.position - position).length() <= radius + sphere.radius;
    }

    std::clog << "No collision logic defined for '" << typeid(*this).name() << "' and '" << typeid(other).name() << "'\n";
    std::exit(1);
}

void Circle::handle_collision(std::shared_ptr<PhysicsObject> other) {
    // TODO: implement
}


void Circle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::CircleShape screen_object(100);
    screen_object.setPosition({position.getX(), position.getY()});

    target.draw(screen_object, states);
}

