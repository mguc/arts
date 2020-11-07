#ifndef __ARTS_RECEIVER_HPP__
#define __ARTS_RECEIVER_HPP__

#include "arts/CGALTypes.hpp"

#include <string>
#include <vector>

namespace arts
{
    class Receiver
    {
    public:
        enum RetCode {
            OK,
            NOK
        };

        static const uint32_t DEFAULT_SAMPLING_FREQUENCY_HZ = 1000000;
        Receiver();
        Receiver(Point position);
        Receiver(Point position, double phi, double theta);
        Receiver(Point position, double phi, double theta, uint32_t samplingFrequency);

        double Fov(double phi, double theta);
        Point& GetPosition();
        uint32_t& GetSamplingFrequency();
        static RetCode Parse(std::string path, std::vector<Receiver> &receivers);
        bool operator==(Receiver& rcv);
        uint32_t GetId();

    private:
        Point mPosition;
        double mPhi, mTheta; // Orientation: mPhi [0, pi/2], mTheta [0, pi)
        uint32_t mSamplingFrequency;
        uint32_t mId;

        static uint32_t mNextId;
    };
} // namespace arts

#endif /* __ARTS_RECEIVER_HPP__ */
