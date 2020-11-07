#ifndef __ARTS_OBJECT_HPP
#define __ARTS_OBJECT_HPP

#include "arts/CGALTypes.hpp"

#include <CGAL/Qt/Basic_viewer_qt.h>

#include <vector>

namespace arts
{
    class Object
    {
    public:
        Object(std::vector<Point> points);
        Point MirrorPoint(Point &point);
        bool Intersect(Segment &segment, Point &point);
        bool Collide(Segment &segment);
        void Draw(CGAL::Basic_viewer_qt &basicViewer);
        std::vector<Point> &GetPoints();
        bool operator==(Object& obj);
        void Print();

    private:
        std::vector<Point> mPoints;
        std::vector<Triangle> mTriangles;
        Plane mPlane;
        uint32_t mId;

        static uint32_t mNextId;
        const double TRIANGLE_TOLERANCE = 0.001;
    };
} // namespace arts

#endif /* ARTS_OBJECT_HPP */
