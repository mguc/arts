#ifndef __ARTS_LOCATION_ESTIMATOR__
#define __ARTS_LOCATION_ESTIMATOR__

#include "arts/Receiver.hpp"
#include "arts/Source.hpp"

#include "audiofile/AudioFile.h"
#include <eigen3/Eigen/Dense>

#include <map>
#include <vector>

namespace arts
{
    class LocationEstimator
    {
    public:
        enum RetCode
        {
            OK,
            LOCATING_FAILED
        };
        LocationEstimator(std::vector<Receiver> &receivers);
        virtual RetCode Locate(std::map<uint32_t, AudioFile<double>> &audioFiles, std::vector<Point> &locations) = 0;

    protected:
        std::vector<Receiver> &mReceivers;
    };

    class LOSEstimator : public LocationEstimator
    {
    public:
        LOSEstimator(std::vector<Receiver> &receivers);
        RetCode Locate(std::map<uint32_t, AudioFile<double>> &audioFiles, std::vector<Point> &locations);

    private:
        Eigen::MatrixX3d mA;
    };

} // namespace arts

#endif /* __ARTS_LOCATION_ESTIMATOR__ */
