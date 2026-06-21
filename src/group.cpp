#include "physics_objects.hpp"

void Group::add_object(std::shared_ptr<PhysicsObject> object) {
    objects.push_back(object);
}

void Group::update(double dt) const {
    for (auto object : objects) {
        object->update(dt);
    }

    for (int i = 0; i < objects.size(); ++i) {
        for (int j = 0; j < objects.size(); ++j) {
            if (i == j) continue;
            auto object1 = objects[i];
            auto object2 = objects[j];

            if (collides(object1, object2)) {
                Vec2 vel1 = object1->handle_collision(object2);
                Vec2 vel2 = object2->handle_collision(object1);

                object1->set_velocity(vel1);
                object2->set_velocity(vel2);
            }
        }
    }
}

void Group::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto object : objects) {
        target.draw(*object);
    }
}

