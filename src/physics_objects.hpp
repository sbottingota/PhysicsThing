#ifndef PHYSICS_OBJECTS_HPP
#define PHYSICS_OBJECTS_HPP

#include <SFML/Graphics.hpp>

#include <memory>

#include "vec2.hpp"

class PhysicsObject : public sf::Drawable {
    protected:
    Pos2 position;
    Vec2 velocity;

    double mass;

    public:
    PhysicsObject(Pos2 position, Vec2 velocity, double mass)
        : position(position), velocity(velocity), mass(mass) {}

    void update(double dt) { // dt is the time (in seconds) since the last update
        position += dt * velocity;
    }

    bool faces(std::shared_ptr<PhysicsObject> other) const {
        Vec2 displacement = other->position - position;
        return displacement.dot(velocity) > 0 && displacement.dot(other->velocity) < 0;
    }

    virtual bool collides(std::shared_ptr<PhysicsObject> other) const = 0;
    virtual void handle_collision(std::shared_ptr<PhysicsObject> other) = 0;
};

class Circle : public PhysicsObject {
    double radius;

    public:
    Circle(Pos2 position, Vec2 velocity, double mass, double radius)
        : PhysicsObject(position, velocity, mass), radius(radius) {}

    virtual bool collides(std::shared_ptr<PhysicsObject> other) const override;
    virtual void handle_collision(std::shared_ptr<PhysicsObject> other) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif
