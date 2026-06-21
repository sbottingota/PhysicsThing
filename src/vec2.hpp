#ifndef VEC2_HPP
#define VEC2_HPP

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

    Vec2 reflected_over(Vec2 axis) const;
};

Vec2 operator*(double n, Vec2 vec);

typedef Vec2 Pos2; // using different names for clarity

#endif
