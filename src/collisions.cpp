#include "physics_objects.hpp"

#include <utility>
#include <typeindex>
#include <iostream>
#include <map>

std::map<
    std::pair<std::type_index, std::type_index>,
    CollisionChecker
> collision_table;

template<typename T1, typename T2>
void add_collision_checker(CollisionChecker checker) {
    std::pair<std::type_index, std::type_index> types(typeid(T1), typeid(T2));
    collision_table.emplace(types, checker);
}

bool collides(std::shared_ptr<PhysicsObject> object1, std::shared_ptr<PhysicsObject> object2) {
    // two objects cannot be colliding unless they are moving towards each other
    if (!object1->faces(object2)) {
        return false;
    }

    std::pair<std::type_index, std::type_index> types(typeid(*object1), typeid(*object2));
    std::pair<std::type_index, std::type_index> reversed_types(typeid(*object2), typeid(*object1));

    if (collision_table.find(types) != collision_table.end()) {
        return collision_table.at(types)(*object1, *object2);

    } else if (collision_table.find(reversed_types) != collision_table.end()) {
        return collision_table.at(reversed_types)(*object1, *object2);

    } else {
        std::clog << "no collision-checking logic defined for '" << typeid(*object1).name() << "' and '" << typeid(*object2).name() << "'\n";
        std::exit(1);
    }
}

bool circles_collide(const PhysicsObject &shape1, const PhysicsObject &shape2) {
    const Circle &circle1 = static_cast<const Circle&>(shape1);
    const Circle &circle2 = static_cast<const Circle&>(shape2);

    return (circle1.get_position() - circle2.get_position()).length() <= circle1.get_radius() + circle2.get_radius();
}

bool circle_collides_line(const PhysicsObject &shape1, const PhysicsObject &shape2) {
    const Circle &circle = static_cast<const Circle&>(shape1);
    const Line &line = static_cast<const Line&>(shape2);

    return false; // TODO: implement
}

bool lines_collide(const PhysicsObject &shape1, const PhysicsObject &shape2) {
    return false; // collision detection between lines is never needed
}

void init_common_collision_checkers() {
    static bool init_finished = false;

    if (!init_finished) {
        add_collision_checker<Circle, Circle>(circles_collide);
        add_collision_checker<Circle, Line>(circle_collides_line);

        init_finished = true;
    }
}

