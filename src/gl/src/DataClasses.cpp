#include "GraphLayout.hpp"

#include "Util/Util.hpp"

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


//trivial comparison operator implementations
bool GraphLayout::Vector::operator==(const Vector& other) const
{
    return x == other.x && y == other.y;
}

bool GraphLayout::Vector::operator!=(const Vector& other) const
{
    return !this->operator==(other);
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


void GraphLayout::Spring::setPointA(const Point& to)
{
    aPtr = make_unique<Point>(to);
}

void GraphLayout::Spring::setPointB(const Point& to)
{
    bPtr = make_unique<Point>(to);
}


//trivial comparison operator implementations
bool GraphLayout::Point::operator==(const Point& other) const
{
    return position == other.position 
        && mass == other.mass
        && velocity == other.velocity
        && acceleration == other.acceleration;
}

bool GraphLayout::Point::operator!=(const Point& other) const
{
    return !this->operator==(other);
}

GraphLayout::Spring::Spring(
        const Point& a, 
        const Point& b, 
        double l, 
        double k)
    : aPtr(make_unique<Point>(a)),
    bPtr(make_unique<Point>(b)),
    length(l),
    springConstant(k)
{}

GraphLayout::Spring::Spring(const Spring& other)
    : aPtr(make_unique<Point>(other.getPointA())),
    bPtr(make_unique<Point>(other.getPointB())),
    length(other.length),
    springConstant(other.springConstant)
{}

GraphLayout::Spring::Spring(
        const Node& left, 
        const Node& right)
    : aPtr(make_unique<Point>(left.getPoint())),
    bPtr(make_unique<Point>(right.getPoint())),
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
    : pointPtr(make_unique<Point>(p)),
    id(u)
{}

GraphLayout::Node::Node(const Node& other)
    : pointPtr(make_unique<Point>(other.getPoint())),
    id(other.id)
{}

void GraphLayout::Node::setPoint(const Point& x)
{
    pointPtr = make_unique<Point>(x);
}

GraphLayout::Point GraphLayout::Node::getPoint() const
{
    return *pointPtr;
}

}
