#include "physics_objects.hpp"

void Group::add_object(std::shared_ptr<PhysicsObject> object) {
    objects.push_back(object);
}

void Group::update(double dt) const {
    for (auto object : objects) {
        object->update(dt);
    }

    for (auto object1 : objects) {
        for (auto object2 : objects) {
            if (object1->collides(object2)) {
                Vec2 vel1 = object1->handle_collision(object2);
                Vec2 vel2 = object2->handle_collision(object1);

                object1->setVelocity(vel1);
                object2->setVelocity(vel2);
            }
        }
    }
}

void Group::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto object : objects) {
        target.draw(*object);
    }
}

