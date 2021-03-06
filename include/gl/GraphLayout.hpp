#pragma once

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

#include <utility>
#include <functional>
#include <ostream>

#include <boost/graph/adjacency_list.hpp>

#include "PopulationGraph.hpp"
#include "Util/Unique.hpp"
#include "Util/NewExceptionType.hpp"

namespace pyramid_scheme_simulator {

class GraphLayout 
{
public:
    NEW_EXCEPTION_TYPE(GraphLayoutException);

    class Vector
    {
    public:
        const double x, y;

        Vector(double, double);
        Vector(const Vector&);
        Vector(std::pair<double, double>&);


        //scalar functions
        Vector add(const Vector&) const;
        Vector subtract(const Vector&) const;
        Vector multiply(double) const;
        Vector divide(double) const;

        double magnitude() const;
        Vector normal() const;
        Vector normalise() const;

        bool operator==(const Vector&) const;
        bool operator!=(const Vector&) const;

        static Vector random();
    };

    using Position = Vector;
    class Point
    {
    public:
        const Position position;
        const double mass;
        const Vector velocity, acceleration;

        Point(Position, double, Vector, Vector);
        Point(const Point&);

        Point applyForce(const Vector& force) const;

        bool operator==(const Point&) const;
        bool operator!=(const Point&) const;

        static Point random();
    };

    class Node;

    struct SpringProperties
    {
        double length, springConstant;
    };

    using Graph = boost::adjacency_list<
            boost::vecS,
            boost::vecS, 
            boost::undirectedS,
            Node,
            SpringProperties>;

    class Node
    {
        std::unique_ptr<Point> pointPtr;
        std::unique_ptr<Unique> id;

        void setUnique(const Unique&);
        class NodeCopier;

    public:

        Node();
        Node(const Unique&);
        Node(const Unique&, const Point&);
        Node(const Node&);

        void  setPoint(const Point&);
        Point getPoint() const;

        Unique getUnique() const { return *id; }

        static NodeCopier getNodeCopier(const Graph&, Graph&);

        std::string print() const;
    };


    //bottom left and top right
    using BoundingBox = std::pair<Position, Position>;

    class Layout
    {
        const double stiffness, 
              repulsion, 
              damping;

        const double minEnergyThreshold;
        const double maxSpeed;

        std::unique_ptr<Graph> graph;

        //call the function with every point or pair of points 
        //on the graph and replace them with the return value
        void mutatePoints(std::function<Point(const Point&)>);
        void mutatePointPairs(std::function<
                std::pair<Point, Point>(
                    const Point&, const Point&)>);

        void forEachSpring(
                std::function<void(Node&, Node&, 
                    double, double)>);

        void forEachPoint(std::function<void(const Point&)>) const;

        double totalEnergy() const;

        void applyCoulombsLaw();
        void applyHookesLaw();
        void attractToCenter();
        void updateVelocity(GraphLayoutTick);
        void updatePosition(GraphLayoutTick);

        void tick(GraphLayoutTick);

        std::unique_ptr<Graph> runSimulation(GraphLayoutTick*);

        /**
         * return a spring with default values
         */
        SpringProperties newSpring() const;

        void makeGraph(Graph&, const PopulationGraph&);

    public:
        Layout(const double, 
                const double,
                const double,
                const double,
                const double,
                const PopulationGraph&);
        Layout(const Layout&);

        BoundingBox getBoundingBox();

        std::unique_ptr<Graph> copyGraph() const;
        static std::unique_ptr<Graph> copyGraph(const Graph&);

        std::unique_ptr<Graph> runSimulation();
        std::unique_ptr<Graph> runSimulation(GraphLayoutTick maxTicks);
    };

protected:
    const std::unique_ptr<GraphLayoutTick> maxTicksPtr;
    Layout layout;

public:
    GraphLayout(
        const Config::BackendOptions::GLBackendOptions::GraphLayoutOptions&, 
        const PopulationGraph&);

    virtual ~GraphLayout() {}

    std::pair<std::unique_ptr<Graph>, BoundingBox> calculateLayout();
    //synonym for calculateLayout
    std::pair<std::unique_ptr<Graph>, BoundingBox> operator()();

    static void printGraphLayout(std::ostream&, Graph&);
};

class GraphLayout::Node::NodeCopier
{
    const Graph& from;
    Graph& to;

public:
    NodeCopier(const Graph& _from, Graph& _to)
        : from(_from), to(_to)
    {}

    void operator()(Graph::vertex_descriptor fromVd, Graph::vertex_descriptor toVd)
    {
        to[toVd].setUnique(from[fromVd].getUnique());
        to[toVd].setPoint(from[fromVd].getPoint());
    }
};

}
