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

GraphLayout::Point GraphLayout::Point::random()
{
    return Point(Position::random(), 
        1.0, //mass
        Vector(0.0, 0.0), 
        Vector(0.0, 0.0));
}

GraphLayout::Node::Node()
    : Node(Unique(),
    Point(Point::random()))
{}

GraphLayout::Node::Node(const Unique& u)
    : Node(u, Point::random())
{}

GraphLayout::Node::Node(
        const Unique& u,
        const Point& p)
    : pointPtr(make_unique<Point>(p)),
    id(make_unique<Unique>(u))
{}

GraphLayout::Node::Node(const Node& other)
    : pointPtr(make_unique<Point>(other.getPoint())),
    id(make_unique<Unique>(other.getUnique()))
{}

void GraphLayout::Node::setPoint(const Point& x)
{
    pointPtr = make_unique<Point>(x);
}

GraphLayout::Point GraphLayout::Node::getPoint() const
{
    return *pointPtr;
}

void GraphLayout::Node::setUnique(const Unique& newId)
{
    id = make_unique<Unique>(newId);
}


GraphLayout::Node::NodeCopier GraphLayout::Node::getNodeCopier(Graph& from, Graph& to)
{
    return NodeCopier(from, to);
}

//Layout CTORs
GraphLayout::Layout::Layout(
        const double _stiffness,
        const double _repulsion,
        const double _damping,
        const double _minEnergyThreshold,
        const double _maxSpeed,
        const PopulationGraph& populationGraph)
    : stiffness(_stiffness),
    repulsion(_repulsion),
    damping(_damping),
    minEnergyThreshold(_minEnergyThreshold),
    maxSpeed(_maxSpeed),
    //default-construct the graph
    graph(make_unique<Graph>())
{
    //then populate it
    makeGraph(*graph, populationGraph);
}

GraphLayout::Layout::Layout(const Layout& other)
    : stiffness(other.stiffness),
    repulsion(other.repulsion),
    damping(other.damping),
    minEnergyThreshold(other.minEnergyThreshold),
    maxSpeed(other.maxSpeed),
    graph(other.copyGraph())
{}


//GraphLayout CTOR
GraphLayout::GraphLayout(
    const Config::BackendOptions::GLBackendOptions::GraphLayoutOptions& options, 
    const PopulationGraph& populationGraph)
    : layout(
            options.stiffness,
            options.repulsion,
            options.damping,
            options.minEnergyThreshold,
            options.maxSpeed,
            populationGraph)
{}

}
