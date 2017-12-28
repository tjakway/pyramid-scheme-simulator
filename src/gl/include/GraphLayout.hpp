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

namespace pyramid_scheme_simulator {

class GraphLayout 
{
    class Vector
    {
    public:
        const double x, y;

        Vector(double, double);
        Vector(const Vector&);
        Vector(std::pair<double, double>&);


        //scalar functions
        Vector add(double)      const;
        Vector subtract(double) const;
        Vector multiply(double) const;
        Vector divide(double)   const;

        double magnitude() const;
        Vector normal()    const;
        Vector normalise() const;

        static Vector random();
    };

    class Point
    {
    public:
        const double position, mass;
        const Vector velocity, acceleration;

        Point applyForce(const Vector& force);
    };

    class Spring
    {
    public:
        const Point pointA, pointB;
        const double length, springConstant;

        Spring(const Point&, const Point&, double, double);
        Spring(const Spring&);
    };
};

}
