#include "GraphLayout.hpp"

#include <cmath>

namespace pyramid_scheme_simulator {

GraphLayout::Vector::Vector(double _x, double _y)
    : x(_x), y(_y)
{}

GraphLayout::Vector::Vector(const Vector& v)
    : x(v.x), y(v.y)
{}

GraphLayout::Vector::Vector(std::pair<double, double>& p)
    : x(p.first), y(p.second)
{}


GraphLayout::Vector GraphLayout::Vector::add(const Vector& v)
    const
{
    return Vector(
            x + v.x,
            y + v.y);
}

GraphLayout::Vector GraphLayout::Vector::subtract(
        const Vector& v) const
{
    return Vector(
            x - v.x,
            y - v.y);
}

GraphLayout::Vector GraphLayout::Vector::multiply(double c) const
{
    return Vector(
            x * c,
            y * c);
}

GraphLayout::Vector GraphLayout::Vector::divide(double c) const
{
    //replicating the behavior of springly,
    //which returns a (0, 0) vector if the argument is 0
    if(c == 0)
    {
        return Vector(0, 0);
    }
    else
    {
        return Vector(
                x / c,
                y / c);
    }
}


double GraphLayout::Vector::magnitude() const
{
    return sqrt(x * x + y * y);
}

GraphLayout::Vector GraphLayout::Vector::normal() const
{
    return Vector(-y, x);
}

GraphLayout::Vector GraphLayout::Vector::normalise() const
{
    return divide(magnitude());
}

GraphLayout::Point::Point(
        Position p,
        double m,
        Vector v, 
        Vector a)
    : position(p),
    mass(m),
    velocity(v),
    acceleration(a)
{}

GraphLayout::Point::Point(const Point& p)
    : position(p.position),
    mass(p.mass),
    velocity(p.velocity),
    acceleration(p.acceleration)
{}


GraphLayout::Point GraphLayout::Point::applyForce(
        const Vector& force) const
{
    const Vector newAcceleration = 
        acceleration.add(force.divide(mass));

    return Point(position, mass,
            velocity, newAcceleration);
}


GraphLayout::Spring::Spring(
        const Point& a, 
        const Point& b, 
        double l, 
        double k)
    : pointA(a),
    pointB(b),
    length(l),
    springConstant(k)
{}

GraphLayout::Spring::Spring(const Spring& other)
    : pointA(other.pointA),
    pointB(other.pointB),
    length(other.length),
    springConstant(other.springConstant)
{}

GraphLayout::Spring::Spring(
        const Node& left, 
        const Node& right)
    : pointA(left.point),
    pointB(right.point),
    length(0.0),
    springConstant(0.0)
{}


GraphLayout::Node::Node()
    : Node(Unique(),
    Point(Position::random(), 
        1.0, //mass
        Vector(0.0, 0.0), 
        Vector(0.0, 0.0)))
{}

GraphLayout::Node::Node(
        const Unique& u,
        const Point& p)
    : id(u), point(p)
{}

GraphLayout::Node::Node(const Node& other)
    : id(other.id),
    point(other.point)
{}

}
