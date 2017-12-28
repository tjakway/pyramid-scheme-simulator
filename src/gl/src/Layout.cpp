#include "GraphLayout.hpp"

#include "Util/Util.hpp"

#include <utility>

namespace pyramid_scheme_simulator {

void GraphLayout::Layout::mutatePoints(
        std::function<Point(const Point&)> f)
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(graph);

    for(; i != iEnd; ++i)
    {
        //replace each point with the result of calling 
        //the function
        graph[*i].setPoint(f(graph[*i].getPoint()));
    }
}


//like the above but without the ability to change the graph
void GraphLayout::Layout::forEachPoint(
        std::function<void(const Point&)> f)
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(graph);

    for(; i != iEnd; ++i)
    {
        f(graph[*i].getPoint());
    }
}

void GraphLayout::Layout::mutatePointPairs(std::function<
        std::pair<Point, Point>(const Point&, const Point&)> f)
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(graph);

    for(; i != iEnd; ++i)
    {
        Graph::vertex_iterator j, jEnd;
        std::tie(j, jEnd) = boost::vertices(graph);

        for(; j != jEnd; ++j)
        {
            std::pair<Point, Point> res = 
                f(graph[*i].getPoint(), graph[*j].getPoint());

            graph[*i].setPoint(res.first);
            graph[*j].setPoint(res.second);
        }
    }
}

void GraphLayout::Layout::forEachSpring(
        std::function<void(Node&, Node&, 
                    double, double)> f) const
{
    Graph::edge_iterator ei, eiEnd;
    std::tie(ei, eiEnd) = boost::edges(graph);

    for(; ei != eiEnd; ++ei)
    {
        Graph::edge_descriptor ed = *ei;

        f(graph[boost::source(ed, graph)],
          graph[boost::target(ed, graph)],
          graph[ed].length,
          graph[ed].springConstant);
    }
}


void GraphLayout::Layout::applyCoulombsLaw()
{
    const double _repulsion = repulsion;
    mutatePointPairs(
    [_repulsion](const Point& firstPoint, 
        const Point& secondPoint)
    {
        if(firstPoint != secondPoint)
        {
            const Position d = 
                firstPoint.position.subtract(
                        secondPoint.position);

            //add 0.1 to "avoid massive forces at small distances (and divide by zero)"
            const double distance = 
                d.magnitude() + 0.1;

            const Vector direction = d.normalise();

            const Vector repulse = 
                    direction.multiply(_repulsion);
            const double distanceSqr = distance * distance;

            //apply forces to each point and replace the
            //points in the graph with the new positions
            return std::make_pair(
            firstPoint
                .applyForce(
                    repulse
                        .divide(distanceSqr * 0.5)),
            secondPoint
                .applyForce(
                    repulse
                        .divide(distanceSqr * (-0.5))));
        }
        //if they're the same return them unchanged
        else
        {
            return std::make_pair(firstPoint, secondPoint);
        }
    });
}


void GraphLayout::Layout::applyHookesLaw()
{
    forEachSpring(
    [](Node& firstNode, Node& secondNode, 
        double length, double springConstant) -> void
    {
        const Position d = 
            secondNode.getPoint().position.subtract(
                    firstNode.getPoint().position);

        const double displacement = length - d.magnitude();
        const Vector direction = d.normalise();

        const auto newFirstPoint = 
            firstNode.getPoint().applyForce(
                    direction.multiply(
                        springConstant * displacement * -0.5));

        const auto newSecondPoint =
            secondNode.getPoint().applyForce(
                    direction.multiply(
                        springConstant * displacement * 0.5));

        firstNode.setPoint(newFirstPoint);
        secondNode.setPoint(newSecondPoint);
    });
}

void GraphLayout::Layout::updateVelocity(GraphLayoutTick tick)
{
    const double _maxSpeed = maxSpeed,
          _damping = damping;

    mutatePoints(
    [tick, _damping, _maxSpeed](const Point& point)
    {
        std::unique_ptr<Vector> newVelocity = 
            make_unique<Vector>(point.velocity.add(
                point.acceleration.multiply(tick))
            .multiply(_damping));

        if(newVelocity->magnitude() > _maxSpeed)
        {
            newVelocity = make_unique<Vector>(
                    newVelocity->normalise().multiply(_maxSpeed));
        }

        const Vector newAcceleration = Vector(0, 0);

        return Point(
                point.position,
                point.mass,
                *newVelocity,
                newAcceleration);
    });
}

void GraphLayout::Layout::updatePosition(GraphLayoutTick tick)
{
    mutatePoints(
    [tick](const Point& point)
    {
        const Position newPosition = 
            point.position.add(point.velocity.multiply(tick));

        return Point(
                newPosition,
                point.mass,
                point.velocity,
                point.acceleration);
    });
}

std::pair<GraphLayout::Position, GraphLayout::Position> 
    GraphLayout::Layout::getBoundingBox()
{
    double bottomLeftX = -2, bottomLeftY = -2,
           topRightX = 2, topRightY = 2;

    forEachPoint(
    [&](const Point& point)
    {
        if(point.position.x < bottomLeftX)
        {
            bottomLeftX = point.position.x;
        }
        if(point.position.y < bottomLeftY)
        {
            bottomLeftY = point.position.y;
        }
        if(point.position.x > topRightX)
        {
            topRightX = point.position.x;
        }
        if(point.position.y > topRightY)
        {
            topRightY = point.position.y;
        }
    });

    return std::make_pair(Vector(bottomLeftX, bottomLeftY),
            Vector(topRightX, topRightY));
}

void GraphLayout::Layout::attractToCenter()
{
    const double _repulsion = repulsion;
    mutatePoints(
    [_repulsion](const Point& point)
    {
        const auto direction = point.position.multiply(-1.0);
        return 
            point.applyForce(direction.multiply(
                        _repulsion / 50.0));
    });
}

double GraphLayout::Layout::totalEnergy() const
{
    double energy = 0.0;

    forEachPoint(
    [&energy](const Point& point)
    {
        const double speed = point.velocity.magnitude();
        energy += 0.5 * point.mass * speed * speed;
    });

    return energy;
}

}
