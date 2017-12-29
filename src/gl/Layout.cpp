#include "gl/GraphLayout.hpp"

#include "Util/Util.hpp"

#include <unordered_map>
#include <utility>

#include <boost/graph/copy.hpp>


/**
 * derived from Springy JS
 * 
 * copyright notice below
 */
/**
 * Springy v2.7.1
 *
 * Copyright (c) 2010-2013 Dennis Hotson
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


namespace pyramid_scheme_simulator {

void GraphLayout::Layout::mutatePoints(
        std::function<Point(const Point&)> f)
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(*graph);

    for(; i != iEnd; ++i)
    {
        //replace each point with the result of calling 
        //the function
        (*graph)[*i].setPoint(f((*graph)[*i].getPoint()));
    }
}


//like the above but without the ability to change the graph
void GraphLayout::Layout::forEachPoint(
        std::function<void(const Point&)> f)
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(*graph);

    for(; i != iEnd; ++i)
    {
        f((*graph)[*i].getPoint());
    }
}

void GraphLayout::Layout::mutatePointPairs(std::function<
        std::pair<Point, Point>(const Point&, const Point&)> f)
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(*graph);

    for(; i != iEnd; ++i)
    {
        Graph::vertex_iterator j, jEnd;
        std::tie(j, jEnd) = boost::vertices(*graph);

        for(; j != jEnd; ++j)
        {
            std::pair<Point, Point> res = 
                f((*graph)[*i].getPoint(), (*graph)[*j].getPoint());

            (*graph)[*i].setPoint(res.first);
            (*graph)[*j].setPoint(res.second);
        }
    }
}

void GraphLayout::Layout::forEachSpring(
        std::function<void(Node&, Node&, 
                    double, double)> f) const
{
    Graph::edge_iterator ei, eiEnd;
    std::tie(ei, eiEnd) = boost::edges(*graph);

    for(; ei != eiEnd; ++ei)
    {
        Graph::edge_descriptor ed = *ei;

        f((*graph)[boost::source(ed, *graph)],
          (*graph)[boost::target(ed, *graph)],
          (*graph)[ed].length,
          (*graph)[ed].springConstant);
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

void GraphLayout::Layout::tick(GraphLayoutTick tick)
{
    applyCoulombsLaw();
    applyHookesLaw();
    attractToCenter();
    updateVelocity(tick);
    updatePosition(tick);
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

GraphLayout::SpringProperties GraphLayout::Layout::newSpring() const
{
    SpringProperties p;
    p.length = 1.0;
    p.springConstant = stiffness;

    return p;
}

void GraphLayout::Layout::makeGraph(Graph& to, const PopulationGraph& populationGraph)
{
    std::unordered_map<Unique, Graph::vertex_descriptor> vdMap;

    //add the vertices from the population graph,
    //creating new points for each unique
    for(const auto& p : populationGraph.vertices())
    {
        const Node newNode = Node(p->id);
        const Graph::vertex_descriptor newVd = boost::add_vertex(newNode, to);

        vdMap.emplace(std::make_pair(newNode.getUnique(), newVd));
    }

    PopulationGraph::Pop left, right;
    for(const auto e : populationGraph.edges())
    {
        std::tie(left, right) = e;

        const Graph::vertex_descriptor leftVd = vdMap.at(left->id),
              rightVd = vdMap.at(right->id);

        boost::add_edge(leftVd, rightVd, newSpring(), to);
    }
}


std::unique_ptr<GraphLayout::Graph> GraphLayout::Layout::copyGraph() const
{
    std::unique_ptr<Graph> newGraph = make_unique<Graph>();

    boost::copy_graph(*graph, *newGraph, 
            boost::vertex_copy(Node::getNodeCopier(*graph, *newGraph)));

    //this is probably unnecessary since the internal unique_ptr<Point> in Node
    //should only be changed by setting it to a new Point (i.e. not modifying the data
    //its pointing to), but just to be really sure...
    Graph::vertex_iterator vi, viEnd;
    std::tie(vi, viEnd) = boost::vertices(*newGraph);
    for(; vi != viEnd; ++vi)
    {
        (*newGraph)[*vi].setPoint((*newGraph)[*vi].getPoint());
    }

    return newGraph;
}

std::unique_ptr<GraphLayout::Graph> GraphLayout::Layout::runSimulation(
        GraphLayoutTick* maxTicksPtr)
{
    std::function<bool()> condition;
    GraphLayoutTick currentTick = 0;

    const std::function<bool()> energyThresholdCondition = 
        [this]() { return this->totalEnergy() < this->minEnergyThreshold; };

    //no maximum if the passed pointer is null
    if(maxTicksPtr == nullptr)
    {
        condition = energyThresholdCondition;
    }
    else
    {
        //otherwise make sure we haven't exceeded the max number of ticks
        const GraphLayoutTick maxTicks = *maxTicksPtr;
        condition = [energyThresholdCondition, maxTicks, &currentTick]()
        {
            return energyThresholdCondition() && currentTick < maxTicks;
        };
    }

    while(condition())
    {
        tick(currentTick);
        currentTick++;
    }

    return copyGraph();
}

std::unique_ptr<GraphLayout::Graph> GraphLayout::Layout::runSimulation()
{
    return runSimulation(nullptr);
}

std::unique_ptr<GraphLayout::Graph> GraphLayout::Layout::runSimulation(GraphLayoutTick maxTicks)
{
    return runSimulation(&maxTicks);
}

}
