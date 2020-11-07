#include "arts/estimator/LocationEstimator.hpp"

using namespace Eigen;

namespace arts
{
    LocationEstimator::LocationEstimator(std::vector<Receiver> &receivers) : mReceivers(receivers) {}

    LOSEstimator::LOSEstimator(std::vector<Receiver> &receivers) : LocationEstimator(receivers)
    {
        mA.resize(mReceivers.size()-1, 3);
        Point &anchor = mReceivers[0].GetPosition();
        for (uint32_t i = 1; i < mReceivers.size(); ++i)
        {
            mA(i - 1, 0) = anchor.x() - mReceivers[i].GetPosition().x();
            mA(i - 1, 1) = anchor.y() - mReceivers[i].GetPosition().y();
            mA(i - 1, 2) = anchor.z() - mReceivers[i].GetPosition().z();
        }

        mA = 2 * mA;
    }

    LocationEstimator::RetCode LOSEstimator::Locate(std::map<uint32_t, AudioFile<double>> &audioFiles, std::vector<Point> &locations)
    {
        if (audioFiles.size() != mReceivers.size())
        {
            return RetCode::LOCATING_FAILED;
        }

        double c = 340;

        // Pre-Processing: Get timestamp of first sample > 0.5 for each receiver
        std::map<uint32_t, double> distances;
        for (auto rx = audioFiles.begin(); rx != audioFiles.end(); ++rx)
        {
            for (uint32_t s = 0; s < rx->second.getNumSamplesPerChannel(); ++s)
            {
                if (rx->second.samples[0][s] > 0)
                {
                    distances[rx->first] = c * s / mReceivers[rx->first].GetSamplingFrequency();
                    break;
                }
            }
        }

        Point &anchor = mReceivers[0].GetPosition();
        VectorXd b(mReceivers.size()-1);
        for (uint32_t i = 1; i < mReceivers.size(); ++i)
        {
            Point r = mReceivers[i].GetPosition();
            b(i - 1) = distances[i] * distances[i] - distances[0] * distances[0] -
                       r.x() * r.x() + anchor.x() * anchor.x() -
                       r.y() * r.y() + anchor.y() * anchor.y() -
                       r.z() * r.z() + anchor.z() * anchor.z();
        }

        VectorXd x = mA.colPivHouseholderQr().solve(b);
        locations.push_back(Point(x(0), x(1), x(2)));
        return RetCode::OK;
    }
} // namespace arts
