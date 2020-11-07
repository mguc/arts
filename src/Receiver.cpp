#include "arts/Receiver.hpp"
#include "fast-cpp-csv-parser/csv.h"

namespace arts
{
    uint32_t Receiver::mNextId = 0;

    Receiver::Receiver() : Receiver(Point(0, 0, 0)) {}
    Receiver::Receiver(Point position) : Receiver(position, 0, 0) {}
    Receiver::Receiver(Point position, double phi, double theta) : Receiver(position, phi, theta, DEFAULT_SAMPLING_FREQUENCY_HZ) {}
    Receiver::Receiver(Point position, double phi, double theta, uint32_t samplingFrequency) : mPosition(position), mPhi(phi), mTheta(theta), mSamplingFrequency(samplingFrequency), mId(mNextId++)
    {
    }

    double Receiver::Fov(double phi, double theta)
    {
        return 1;
    }

    Point &Receiver::GetPosition()
    {
        return mPosition;
    }

    uint32_t &Receiver::GetSamplingFrequency()
    {
        return mSamplingFrequency;
    }

    Receiver::RetCode Receiver::Parse(std::string path, std::vector<Receiver> &receivers)
    {
        try
        {
            io::CSVReader<3> in(path);
            double x, y, z;
            while (in.read_row(x, y, z))
            {
                receivers.push_back(Receiver(Point(x, y, z)));
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return NOK;
        }

        return OK;
    }

    uint32_t Receiver::GetId()
    {
        return mId;
    }
} // namespace arts
