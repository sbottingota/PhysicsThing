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

    public:
    PhysicsObject(Pos2 position, Vec2 velocity, double mass)
        : position(position), velocity(velocity), mass(mass) {}


    virtual void update(double dt); // dt is the time (in seconds) since the last update

    Pos2 get_position() const;
    Vec2 get_velocity() const;

    void set_velocity(Vec2 new_velocity);

    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const = 0;

    bool faces(std::shared_ptr<PhysicsObject> other) const;
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
    Circle(Pos2 position, Vec2 velocity, double mass, double radius);

    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    double get_radius() const;
};

class Line : public PhysicsObject {
    constexpr static double thickness = 5;

    Vec2 direction;

    public:
    Line(Pos2 point1, Pos2 point2);

    virtual void update(double dt) override {} // as lines are stationary, override update() to be a no-op

    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


using CollisionChecker = bool (*)(const PhysicsObject&, const PhysicsObject&);

template<typename T1, typename T2>
void add_collision_checker(CollisionChecker checker);

bool collides(std::shared_ptr<PhysicsObject> object1, std::shared_ptr<PhysicsObject> object2);

// initializes collision checkers between shapes defined in src/physics_objects.hpp
void init_common_collision_checkers();

#endif
