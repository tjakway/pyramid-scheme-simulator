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

#include <boost/graph/adjacency_list.hpp>

#include "Util/Unique.hpp"

namespace pyramid_scheme_simulator {

class GraphLayout 
{
public:
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
    };

    class Node;

    class Spring
    {
    public:
        const Point pointA, pointB;
        const double length, springConstant;

        Spring(const Point&, const Point&, double, double);
        Spring(const Spring&);
        Spring(const Node&, const Node&);
    };

    class Node
    {
        std::unique_ptr<Point> pointPtr;
    public:
        const Unique id;

        Node();
        Node(const Unique&, const Point&);
        Node(const Node&);

        void  setPoint(const Point&);
        Point getPoint() const;
    };

    using Graph = boost::adjacency_list<
            boost::vecS,
            boost::vecS, 
            boost::undirectedS,
            Node,
            Spring>;


    class Layout
    {
        const double stiffness, 
              repulsion, 
              damping;

        const double minEnergyThreshold;
        const double maxSpeed;

        Graph graph;

        //call the function with every point or pair of points 
        //on the graph and replace them with the return value
        void mutatePoints(std::function<Point(Point)>);
        void mutatePointPairs(std::function<
                std::pair<Point, Point>(Point, Point)>);

    public:
        Layout(const double, 
                const double,
                const double,
                const double,
                const double);
        Layout(const Layout&);

        void applyCoulombsLaw();
        void applyHookesLaw();
    };

};

}
