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

    const float mass;

    public:
    constexpr static sf::Color arrow_color = sf::Color::Red;
    constexpr static float arrow_width = 10.0;
    constexpr static float arrow_scale = 0.5;

    PhysicsObject(Pos2 position, Vec2 velocity, float mass)
        : position(position), velocity(velocity), mass(mass) {}


    virtual void update(float dt); // dt is the time (in seconds) since the last update

    Pos2 get_position() const;
    Vec2 get_velocity() const;

    void set_velocity(Vec2 new_velocity);

    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const = 0;

    // for shapes A and B, returns true iff A is faced by B, and B is faced by A
    bool faces(const PhysicsObject &other) const;

    virtual bool is_faced_by(const PhysicsObject &other) const = 0;

    virtual void draw_velocity(sf::RenderTarget &target, sf::RenderStates states) const;
};

class Group : public sf::Drawable {
    std::vector<std::shared_ptr<PhysicsObject>> objects;

    public:
    constexpr static bool draw_velocities = true;

    Group() {}

    void add_object(std::shared_ptr<PhysicsObject> object);
    void update(float dt) const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class Circle : public PhysicsObject {
    const float radius;

    public:
    Circle(Pos2 position, Vec2 velocity, float mass, float radius);

    virtual bool is_faced_by(const PhysicsObject &other) const override;
    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    float get_radius() const;
};

class Line : public PhysicsObject {
    constexpr static float thickness = 5;

    const Vec2 direction;

    public:
    Line(Pos2 point1, Pos2 point2);

    virtual void update(float dt) override {} // as lines are stationary, override update() to be a no-op

    virtual Vec2 handle_collision(std::shared_ptr<PhysicsObject> other) const override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    Pos2 closest_point_to(Pos2 point) const;

    virtual bool is_faced_by(const PhysicsObject &other) const override;
};


using CollisionChecker = bool (*)(const PhysicsObject&, const PhysicsObject&);

template<typename T1, typename T2>
void add_collision_checker(CollisionChecker checker);

bool collides(std::shared_ptr<PhysicsObject> object1, std::shared_ptr<PhysicsObject> object2);

// initializes collision checkers between shapes defined in src/physics_objects.hpp
void init_common_collision_checkers();

#endif
