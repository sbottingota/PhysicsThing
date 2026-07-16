#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <memory>

class Projection {
    const float min, max;

    public:
    Projection(float min, float max) : min(min), max(max) {}

    bool overlaps(Projection &other) const;
    float overlap_size(Projection &other) const;
};

class Axis {
    Pos2 start, end; // note that these points merely lie on the axis; they do not constrain it

    public:
    Axis(Pos2 start, Pos2 end) : start(start), end(end) {}
    static Axis normal_to(Pos2 point1, Pos2 point2); // axis normal to edge provided, passing through midpoint of edge

    Pos2 get_start() const {
        return start;
    }

    Pos2 get_end() const {
        return end;
    }
};

#endif
