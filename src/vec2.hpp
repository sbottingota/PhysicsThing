#ifndef VEC2_HPP
#define VEC2_HPP

// using different names for clarity in different uses
class Vec2;
typedef Vec2 Pos2; 

class Vec2 {
    float x, y;

    public:
    Vec2(float x, float y) : x(x), y(y) {}

    float get_x() const;
    float get_y() const;

    Vec2 operator+(Vec2 other) const;
    Vec2 operator-(Vec2 other) const;

    Vec2 operator*(float n) const;
    Vec2 operator/(float n) const;

    void operator+=(Vec2 other);
    void operator-=(Vec2 other);

    void operator*=(float n);
    void operator/=(float n);

    float dot(Vec2 other) const;

    float length() const;
    float length_squared() const;

    float angle() const; // in radians

    Vec2 reflected_over(Vec2 normal) const;

    Vec2 perp() const;

    Vec2 normalized() const;

    // angle in radians
    Pos2 rotated(float angle, Pos2 center) const;

    // find the closest point to this point, on a line defined by its start point and its direction
    Pos2 closest_point_on_line(Pos2 start, Vec2 direction) const;

    // same as above, but the line is said to end at start+direction
    Pos2 closest_point_on_line_clamped(Pos2 start, Vec2 direction) const;
};

Vec2 operator*(float n, Vec2 vec);

#endif
