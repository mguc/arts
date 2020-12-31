#include "arts/estimator/LocationEstimator.hpp"
#include "arts/Path.hpp"
#include "arts/Simulation.hpp"

#include "catch2/catch.hpp"

#include <math.h>

using namespace arts;
using namespace std;

#define REFERENCE_ROOM    "rsc/environment/4x5x3room.obj"
#define SIMULATION_TIME_S 0.1
#define DISTANCE_M        0.1

double EstimateAngle(double t1, double t2)
{
    double c = 340;
    return acos(c*(t1-t2)/DISTANCE_M) * 180.0 / M_PI;
}

double EstimateDistance(double t1, double t2)
{
    double c = 340;
    double t = (t1+t2)/2;
    return c*t/2;
}

TEST_CASE("TestWallEstimator", "[estimator]")
{
    Environment env;
    env.Parse(REFERENCE_ROOM);
    uint32_t order = 1;

    double d = DISTANCE_M;

    vector<Source> sources;
    sources.push_back(Source(Point(d/2,0,0)));

    vector<Receiver> receivers;
    receivers.push_back(Receiver(Point(0,0,0)));
    receivers.push_back(Receiver(Point(d,0,0)));

    Simulation sim(env, receivers, sources);
    vector<vector<vector<Path>>> paths(receivers.size(), vector<vector<Path>>(sources.size()));
    vector<vector<AudioFile<double>>> audioFiles(receivers.size(), vector<AudioFile<double>>(sources.size()));

    vector<vector<double>> rx_times(receivers.size());

    sim.Run(SIMULATION_TIME_S, order, paths, audioFiles);

    for (uint32_t rx = 0; rx < receivers.size(); ++rx)
    {
        for(uint32_t sample = 0; sample < audioFiles[rx][0].getNumSamplesPerChannel(); ++sample)
        {
            if(audioFiles[rx][0].samples[0][sample] > 0)
            {
                cout << "rx" << rx << ": " << sample << endl;
                rx_times[rx].push_back((double)sample/receivers[rx].GetSamplingFrequency());
            }
        }
    }

    for(uint32_t i = 0; i < rx_times[0].size(); ++i) {
        cout << "Angle: " << EstimateAngle(rx_times[0][i], rx_times[1][i]) << "°, Distance: " << EstimateDistance(rx_times[0][i], rx_times[1][i]) << endl;
    }

}
