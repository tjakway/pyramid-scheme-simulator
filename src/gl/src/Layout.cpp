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


void GraphLayout::Layout::applyCoulombsLaw()
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(graph);

    for(; i != iEnd; ++i)
    {
        Graph::vertex_iterator j, jEnd;
        std::tie(j, jEnd) = boost::vertices(graph);

        for(; j != jEnd; ++j)
        {
            const Point firstPoint = graph[*i].getPoint(),
                        secondPoint = graph[*j].getPoint();
            
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
                        direction.multiply(repulsion);
                const double distanceSqr = distance * distance;

                //apply forces to each point and replace the
                //points in the graph with the new positions
                graph[*i].setPoint(
                 firstPoint
                    .applyForce(
                        repulse
                         .divide(distanceSqr * 0.5)));

                graph[*j].setPoint(
                 secondPoint
                    .applyForce(
                        repulse
                         .divide(distanceSqr * (-0.5))));
            }
        }
    }
}


void GraphLayout::Layout::applyHookesLaw()
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(graph);

    for(; i != iEnd; ++i)
    {
        Graph::vertex_iterator j, jEnd;
        std::tie(j, jEnd) = boost::vertices(graph);

        for(; j != jEnd; ++j)
        {

        }
    }
}

}
