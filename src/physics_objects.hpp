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

    bool faces(std::shared_ptr<PhysicsObject> other) const;

    public:
    PhysicsObject(Pos2 position, Vec2 velocity, double mass)
        : position(position), velocity(velocity), mass(mass) {}


    void update(double dt); // dt is the time (in seconds) since the last update

    void setVelocity(Vec2 new_velocity);

    virtual bool collides(std::shared_ptr<PhysicsObject> other) const = 0;
    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const = 0;
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
