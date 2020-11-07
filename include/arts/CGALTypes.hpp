#ifndef __ARTS_CGAL_TYPES_HPP__
#define __ARTS_CGAL_TYPES_HPP__

#include <CGAL/Cartesian.h>

namespace arts
{
    typedef CGAL::Cartesian<double> K;
    typedef K::Plane_3 Plane;
    typedef K::Point_3 Point;
    typedef K::Ray_3 Ray;
    typedef K::Segment_3 Segment;
    typedef K::Triangle_3 Triangle;
    typedef K::Vector_3 Vector;

    struct Range {
        Point min;
        Point max;
    };
} // namespace arts

#endif /* __ARTS_CGAL_TYPES_HPP__ */
