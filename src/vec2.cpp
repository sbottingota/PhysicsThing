#include "vec2.hpp"

#include <cmath>

float Vec2::get_x() const {
    return x;
}

float Vec2::get_y() const {
    return y;
}

Vec2 Vec2::operator+(Vec2 other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(Vec2 other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float n) const {
    return Vec2(n * x, n * y);
}

Vec2 Vec2::operator/(float n) const {
    return Vec2(x / n, y / n);
}

void Vec2::operator+=(Vec2 other) {
    x += other.x;
    y += other.y;
}

void Vec2::operator-=(Vec2 other) {
    x -= other.x;
    y -= other.y;
}

void Vec2::operator*=(float n) {
    x *= n;
    y *= n;
}

void Vec2::operator/=(float n) {
    x /= n;
    y /= n;
}

float Vec2::dot(Vec2 other) const {
    return x * other.x + y * other.y;
}

float Vec2::length() const {
    return std::sqrt(length_squared());
}

float Vec2::length_squared() const {
    return x*x + y*y;
}

float Vec2::angle() const {
    return std::atan2(y, x);
}

Vec2 Vec2::reflected_over(Vec2 normal) const {
    normal /= normal.length(); // normalise to a length of 1
    return *this - 2 * dot(normal) * normal;
}

Vec2 Vec2::perp() const {
    return Vec2({y, -x});
}

Pos2 Vec2::rotated(float angle, Pos2 center) const {
    float sin_angle = std::sin(angle);
    float cos_angle = std::cos(angle);

    float new_x = (x - center.x) * cos_angle + (y - center.y) * sin_angle + center.x;
    float new_y = -(x - center.x) * sin_angle + (y - center.y) * cos_angle + center.y;

    return Pos2(new_x, new_y);
}

Pos2 Vec2::closest_point_on_line(Pos2 start, Vec2 direction) const {
    float t = (*this - start).dot(direction) / direction.length_squared();

    if (t < 0) { t = 0; }

    return start + direction * t;
}

Pos2 Vec2::closest_point_on_line_clamped(Pos2 start, Vec2 direction) const {
    float t = (*this - start).dot(direction) / direction.length_squared();

    // clamp point between the bounds of the line
    if (t < 0) { t = 0; }
    if (t > 1) { t = 1; }

    return start + direction * t;
}

Vec2 operator*(float n, Vec2 vec) {
    return vec * n;
}

