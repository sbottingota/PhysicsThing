#ifndef VEC2_HPP
#define VEC2_HPP

// using different names for clarity in different uses
class Vec2;
typedef Vec2 Pos2; 

class Vec2 {
    double x, y;

    public:
    Vec2(double x, double y) : x(x), y(y) {}

    double getX() const;
    double getY() const;

    Vec2 operator+(Vec2 other) const;
    Vec2 operator-(Vec2 other) const;

    Vec2 operator*(double n) const;
    Vec2 operator/(double n) const;

    void operator+=(Vec2 other);
    void operator-=(Vec2 other);

    void operator*=(double n);
    void operator/=(double n);

    double dot(Vec2 other) const;

    double length() const;
    double length_squared() const;

    double angle() const; // in radians

    Vec2 reflected_over(Vec2 normal) const;
    
    // find the closest point to this point, on a line defined by its start point and its direction
    Pos2 closest_point_on_line(Pos2 start, Vec2 direction) const;

    // same as above, but the line is said to end at start+direction
    Pos2 closest_point_on_line_clamped(Pos2 start, Vec2 direction) const;
};

Vec2 operator*(double n, Vec2 vec);

#endif
