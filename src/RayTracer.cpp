#include "arts/Path.hpp"
#include "arts/RayTracer.hpp"

#include <map>

namespace arts
{
    RayTracer::RayTracer(Environment &env, uint32_t order) : mEnvironment(env), mOrder(order) {}

    void RayTracer::Run(Receiver &r, Source &s, std::vector<Path> &paths)
    {
        std::vector<Object> &objects = mEnvironment.GetObjects();

        // Direct paths
        CreatePath(s, r, paths);

        if (mOrder < 1)
        {
            return;
        }

        // First order paths
        for (uint32_t obj1 = 0; obj1 < objects.size(); ++obj1)
        {
            std::vector<Object *> mirror = {&objects[obj1]};
            CreatePath(s, r, mirror, paths);
        }

        if (mOrder < 2)
        {
            return;
        }

        // Second order paths
        for (uint32_t obj1 = 0; obj1 < objects.size(); ++obj1)
        {
            for (uint32_t obj2 = 0; obj2 < objects.size(); ++obj2)
            {
                if (obj1 == obj2)
                {
                    continue;
                }
                std::vector<Object *> mirrors = {&objects[obj1], &objects[obj2]};
                CreatePath(s, r, mirrors, paths);
            }
        }

        if (mOrder < 3)
        {
            return;
        }

        // Third order paths
        for (uint32_t obj1 = 0; obj1 < objects.size(); ++obj1)
        {
            for (uint32_t obj2 = 0; obj2 < objects.size(); ++obj2)
            {
                if (obj1 == obj2)
                {
                    continue;
                }
                for (uint32_t obj3 = 0; obj3 < objects.size(); ++obj3)
                {
                    if (obj2 == obj3)
                    {
                        continue;
                    }
                    std::vector<Object *> mirrors = {&objects[obj1], &objects[obj2], &objects[obj3]};
                    CreatePath(s, r, mirrors, paths);
                }
            }
        }
    }

    void RayTracer::CreatePath(Source &src, Receiver &rcv, std::vector<Object *> &mirrors, std::vector<Path> &paths)
    {
        static uint32_t counter = 0;
        uint32_t order = mirrors.size();
        counter++;

        if (order == 0)
        {
            return;
        }

        std::vector<Object> &objects = mEnvironment.GetObjects();
        Point mirroredPoints[order];
        std::vector<Point> intersectionPoints;

        // Mirror receiver point
        mirroredPoints[0] = mirrors[0]->MirrorPoint(rcv.GetPosition());
        for (auto i = 1; i < order; ++i)
        {
            mirroredPoints[i] = mirrors[i]->MirrorPoint(mirroredPoints[i - 1]);
        }

        Point tx = src.GetPosition();
        Segment s;
        Point p;
        for (auto i = order; i > 0; --i)
        {
            s = Segment(tx, mirroredPoints[i - 1]);
            // 1. Check: Does reflection exist on object?
            if (!mirrors[i - 1]->Intersect(s, p))
            {
                return;
            }

            // 2. Check: Does collision exist with other objects?
            s = Segment(tx, p);
            for (auto k = objects.begin(); k != objects.end(); ++k)
            {
                if (*mirrors[i - 1] == *k)
                {
                    continue;
                }
                if (i < order)
                {
                    if (*mirrors[i] == *k)
                    {
                        continue;
                    }
                }
                if (k->Collide(s))
                {
                    return;
                }
            }

            intersectionPoints.push_back(p);
            tx = p;
        }

        // 3. Check: Does collision exist with other objects on last segment?
        s = Segment(tx, rcv.GetPosition());
        for (auto k = objects.begin(); k != objects.end(); ++k)
        {
            if (*mirrors[0] == *k)
            {
                continue;
            }
            if (k->Collide(s))
            {
                return;
            }
        }

        paths.push_back(Path(rcv, src, intersectionPoints, mirrors));
    }

    void RayTracer::CreatePath(Source &src, Receiver &rcv, std::vector<Path> &paths)
    {
        if (src.GetPosition() != rcv.GetPosition())
        {
            Segment s(src.GetPosition(), rcv.GetPosition());
            std::vector<Object> &objects = mEnvironment.GetObjects();
            for (auto obj = objects.begin(); obj != objects.end(); ++obj)
            {
                if (obj->Collide(s))
                {
                    return;
                }
            }
        }

        paths.push_back(Path(rcv, src));
    }
} // namespace arts
