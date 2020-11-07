
#include "arts/Environment.hpp"

#include <CGAL/IO/OBJ_reader.h>

#include <fstream>

namespace arts
{
    Environment::Environment() {}

    Environment::RetCode Environment::Parse(std::string filePath)
    {
        RetCode ret = OK;
        mObjects.clear();
        std::ifstream envFile(filePath.c_str());
        std::vector<Point> points;
        std::vector<std::vector<std::size_t>> faces;
        if (CGAL::read_OBJ(envFile, points, faces))
        {
            for (auto face = faces.begin(); face != faces.end(); ++face)
            {
                std::vector<Point> objectPoints;
                for (auto objPointIndex = face->begin(); objPointIndex != face->end(); ++objPointIndex)
                {
                    objectPoints.push_back(points[*objPointIndex]);
                }
                mObjects.push_back(Object(objectPoints));
                objectPoints.clear();
            }
        }
        else
        {
            mObjects.clear();
            ret = FILE_PARSING_FAILED;
        }

        return ret;
    }

    std::vector<Object> &Environment::GetObjects()
    {
        return mObjects;
    }

    void Environment::Draw(CGAL::Basic_viewer_qt &basicViewer)
    {
        for(auto it = mObjects.begin(); it != mObjects.end(); ++it) {
            it->Draw(basicViewer);
        }
    }

    Range Environment::GetRange() {
        double x_min = 0, y_min = 0, z_min = 0;
        double x_max = 0, y_max = 0, z_max = 0;
        for(auto it = mObjects.begin(); it != mObjects.end(); ++it) {
            for(auto point = it->GetPoints().begin(); point != it->GetPoints().end(); ++point) {
                if(point->x() < x_min) {
                    x_min = point->x();
                }
                if(point->x() > x_max) {
                    x_max = point->x();
                }

                if(point->y() < y_min) {
                    y_min = point->y();
                }
                if(point->y() > y_max) {
                    y_max = point->y();
                }

                if(point->z() < z_min) {
                    z_min = point->z();
                }
                if(point->z() > z_max) {
                    z_max = point->z();
                }
            }
        }

        Range range = {
            Point(x_min, y_min, z_min),
            Point(x_max, y_max, z_max)
        };

        return range;
    }

    void Environment::AddObject(Object &obj) {
        mObjects.push_back(obj);
    }

} // namespace arts
