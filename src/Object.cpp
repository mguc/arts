#include "arts/Object.hpp"

namespace arts
{
    uint32_t Object::mNextId = 0;

    Object::Object(std::vector<Point> points) : mPoints(points), mId(mNextId++)
    {
        if (mPoints.size() < 3)
        {
            mPoints.clear();
            mTriangles.clear();
        }
        else
        {
            // Split the polygon into triangles (triangulation). The triangles are only used for hit detection.
            for (uint32_t i = 2; i < mPoints.size(); ++i)
            {
                if (i < (mPoints.size() - 1))
                {
                    /* Cases have been observed where an intersection point should exist on the polygon, but none of
                     * the constructed triangles contained it. For this reason the "inner" segments of the polygon
                     * have been stretched to prevent this issue.
                     * TODO: Try to solve the problem in a different manner.
                     */
                    Vector v1 = TRIANGLE_TOLERANCE * Vector(mPoints[i - 1], mPoints[0]);
                    Vector v2 = TRIANGLE_TOLERANCE * Vector(mPoints[i - 1], mPoints[i]);
                    mTriangles.push_back(Triangle(mPoints[0] + v1, mPoints[i - 1], mPoints[i] + v2));
                }
                else
                {
                    mTriangles.push_back(Triangle(mPoints[0], mPoints[i - 1], mPoints[i]));
                }
            }

            // Create a plane from the first three points of the polygon
            mPlane = Plane(mPoints[0], mPoints[1], mPoints[2]);
        }
    }

    Point Object::MirrorPoint(Point &point)
    {
        Point projection_point = mPlane.projection(point);
        return (projection_point + (projection_point - point));
    }

    bool Object::Intersect(Segment &segment, Point &point)
    {
        if (Collide(segment))
        {
            auto result = intersection(segment, mPlane);
            if (result)
            {
                if (Point *p = boost::get<Point>(&*result))
                {
                    point = *p;
                    return true;
                }
            }
        }
        return false;
    }

    bool Object::Collide(Segment &segment)
    {
        // This is the case when the segment consists of two equal points
        if (segment.is_degenerate())
        {
            return false;
        }
        for (auto it = mTriangles.begin(); it != mTriangles.end(); ++it)
        {
            if (it->is_degenerate())
            {
                continue;
            }
            if (CGAL::do_intersect(segment, *it))
            {
                return true;
            }
        }
        return false;
    }

    void Object::Draw(CGAL::Basic_viewer_qt &basicViewer)
    {
        auto it = mPoints.begin();
        for (; it != mPoints.end() - 1; ++it)
        {
            basicViewer.add_segment(*it, *(it + 1));
        }
        basicViewer.add_segment(*it, *mPoints.begin());
    }

    std::vector<Point> &Object::GetPoints()
    {
        return mPoints;
    }

    bool Object::operator==(Object &obj)
    {
        return (mId == obj.mId);
    }

    void Object::Print()
    {
        for (auto it = mPoints.begin(); it != mPoints.end(); ++it)
        {
            std::cout << "(" << *it << ")->";
        }
        std::cout << "(" << mPoints[0] << ")" << std::endl;
    }
} // namespace arts
