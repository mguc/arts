#include "arts/Path.hpp"

namespace arts
{
    Path::Path(Receiver &rcv, Source &src) : mReceiver(rcv), mSource(src)
    {
        mLength = 0;
        // mNodes.clear();
        // mNodeObjects.clear();
        // mVectors.clear();
        mLength = sqrt((rcv.GetPosition()-src.GetPosition()).squared_length());
    }

    Path::Path(Receiver &rcv, Source &src, std::vector<Point> &nodes, std::vector<Object *> &objects) : mReceiver(rcv), mSource(src), mNodes(nodes), mNodeObjects(objects)
    {
        mVectors.clear();
        mLength = 0;
        Point tx = mSource.GetPosition();
        Vector v;
        for (auto it = mNodes.begin(); it != mNodes.end(); ++it)
        {
            v = Vector(tx, *it);
            mLength += sqrt(v.squared_length());
            mVectors.push_back(v);
            tx = *it;
        }
        v = Vector(tx, mReceiver.GetPosition());
        mLength += sqrt(v.squared_length());
        mVectors.push_back(v);
    }

    void Path::Draw(CGAL::Basic_viewer_qt &basicViewer)
    {
        CGAL::Color c;
        switch (mNodes.size())
        {
        case 1:
            c = CGAL::yellow();
            break;
        case 2:
            c = CGAL::orange();
            break;
        case 3:
            c = CGAL::red();
            break;
        default:
            c = CGAL::black();
            break;
        }
        Point tx = mSource.GetPosition();
        for (auto it = mNodes.begin(); it != mNodes.end(); ++it)
        {
            basicViewer.add_segment(tx, *it, c);
            tx = *it;
        }
        basicViewer.add_segment(tx, mReceiver.GetPosition(), c);
    }

    void Path::Print()
    {
        std::cout << "(" << mSource.GetPosition() << ")->";
        for (auto it = mNodes.begin(); it != mNodes.end(); ++it)
        {
            std::cout << "(" << *it << ")->";
        }
        std::cout << "(" << mReceiver.GetPosition() << "): " << mLength << std::endl;
    }

    uint32_t Path::NumberOfNodes()
    {
        return 2 + mNodes.size();
    }

    double Path::Length()
    {
        return mLength;
    }

    Receiver &Path::GetReceiver()
    {
        return mReceiver;
    }
    Source &Path::GetSource()
    {
        return mSource;
    }
    std::vector<Vector> &Path::GetVectors()
    {
        return mVectors;
    }

} // namespace arts
