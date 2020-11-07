#ifndef __ARTS_ENVIRONMENT_HPP__
#define __ARTS_ENVIRONMENT_HPP__

#include "arts/CGALTypes.hpp"
#include "arts/Object.hpp"

#include <CGAL/Qt/Basic_viewer_qt.h>

#include <string>
#include <vector>

namespace arts
{
    class Environment
    {
    public:
        enum RetCode {
            OK,
            FILE_DOES_NOT_EXIST,
            FILE_PARSING_FAILED
        };
        Environment();
        RetCode Parse(std::string filePath);
        std::vector<Object> &GetObjects();
        void Draw(CGAL::Basic_viewer_qt &basicViewer);
        Range GetRange();
        void AddObject(Object &obj);

    private:
        std::vector<Object> mObjects;
    };
} // namespace arts

#endif /* __ARTS_ENVIRONMENT_HPP__ */
