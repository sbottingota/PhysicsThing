#include "physics_objects.hpp"

#include <utility>
#include <typeindex>
#include <algorithm>
#include <iostream>
#include <map>

bool Projection::overlaps(Projection &other) const {
    return std::max(min, other.min) <= std::min(max, other.max);
}

float Projection::overlap_size(Projection &other) const {
    return std::min(max, other.max) - std::max(min, other.min);
}

Axis Axis::normal_to(Pos2 point1, Pos2 point2) {
    Pos2 start = (point1 + point2) / 2;
    Pos2 end = start + (point2 - point1).perp();

    return Axis(start, end);
}

