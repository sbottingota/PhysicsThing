#ifndef PHYSICS_OBJECTS_HPP
#define PHYSICS_OBJECTS_HPP

#include <SFML/Graphics.hpp>
#include <memory>

#include "vec2.hpp"

#define ELASTICITY 1.0

class PhysicsObject : public sf::Drawable {
    protected:
    Pos2 position;
    Vec2 velocity;

    double mass;

    bool faces(std::shared_ptr<PhysicsObject> other) const {
        Vec2 displacement = other->position - position;
        return displacement.dot(velocity) > 0 && displacement.dot(other->velocity) < 0;
    }

    public:
    PhysicsObject(Pos2 position, Vec2 velocity, double mass)
        : position(position), velocity(velocity), mass(mass) {}

    void update(double dt) { // dt is the time (in seconds) since the last update
        position += dt * velocity;
    }

    virtual bool collides(std::shared_ptr<PhysicsObject> other) const = 0;
    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const = 0;

    Pos2 getPosition() const {
        return position;
    }

    Vec2 getVelocity() const {
        return velocity;
    }

    double getMass() const {
        return mass;
    }

    void setVelocity(Vec2 new_velocity) {
        velocity = new_velocity;
    }
};

class Group : public sf::Drawable {
    std::vector<std::shared_ptr<PhysicsObject>> objects;

    public:
    Group() {}

    void add_object(std::shared_ptr<PhysicsObject> object);
    void update(double dt) const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class Circle : public PhysicsObject {
    double radius;

    public:
    Circle(Pos2 position, Vec2 velocity, double mass, double radius)
        : PhysicsObject(position, velocity, mass), radius(radius) {}

    virtual bool collides(std::shared_ptr<PhysicsObject> other) const override;
    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif
