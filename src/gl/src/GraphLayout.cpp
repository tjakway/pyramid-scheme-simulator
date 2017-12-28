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


GraphLayout::Vector GraphLayout::Vector::add(double c) const
{
    return Vector(
            x + c,
            y + c);
}

GraphLayout::Vector GraphLayout::Vector::subtract(double c) const
{
    return Vector(
            x - c,
            y - c);
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

}
