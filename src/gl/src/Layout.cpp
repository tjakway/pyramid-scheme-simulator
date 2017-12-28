#include "GraphLayout.hpp"

#include <utility>

namespace pyramid_scheme_simulator {

void GraphLayout::Layout::mutatePoints(
        std::function<Point(Point)> f)
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

void GraphLayout::Layout::mutatePointPairs(std::function<
        std::pair<Point, Point>(Point, Point)> f)
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
                    double, double)> f)
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
    [_repulsion](Point firstPoint, Point secondPoint)
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
    [](Node& firstPoint, Node& secondPoint, 
        double length, double springConstant)
    {
        
    });
}

}
