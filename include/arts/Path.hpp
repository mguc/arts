#ifndef __ARTS_PATH_HPP__
#define __ARTS_PATH_HPP__

#include "arts/Object.hpp"
#include "arts/Receiver.hpp"
#include "arts/Source.hpp"

#include <CGAL/Qt/Basic_viewer_qt.h>

namespace arts
{
    class Path
    {
    private:
        Receiver &mReceiver;
        Source &mSource;
        std::vector<Point> mNodes;
        std::vector<Object *> mNodeObjects;
        std::vector<Vector> mVectors;
        double mLength;

    public:
        Path(Receiver &rcv, Source &src);
        Path(Receiver &rcv, Source &src, std::vector<Point> &nodes, std::vector<Object *> &objects);
        void Draw(CGAL::Basic_viewer_qt &basicViewer);
        void Print();
        uint32_t NumberOfNodes();
        double Length();
        Receiver &GetReceiver();
        Source &GetSource();
        std::vector<Vector> &GetVectors();
    };

} // namespace arts

#endif /* __ARTS_PATH_HPP__ */
