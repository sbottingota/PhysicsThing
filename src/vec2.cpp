#include "vec2.hpp"

#include <cmath>

double Vec2::getX() const {
    return x;
}

double Vec2::getY() const {
    return y;
}

Vec2 Vec2::operator+(Vec2 other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(Vec2 other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(double n) const {
    return Vec2(n * x, n * y);
}

Vec2 Vec2::operator/(double n) const {
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

void Vec2::operator*=(double n) {
    x *= n;
    y *= n;
}

void Vec2::operator/=(double n) {
    x /= n;
    y /= n;
}

double Vec2::dot(Vec2 other) const {
    return x * other.x + y * other.y;
}

double Vec2::length() const {
    return std::sqrt(length_squared());
}

double Vec2::length_squared() const {
    return x*x + y*y;
}

double Vec2::angle() const {
    return std::atan2(y, x);
}

Vec2 operator*(double n, Vec2 vec) {
    return vec * n;
}

