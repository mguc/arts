#include "arts/estimator/LocationEstimator.hpp"
#include "arts/Path.hpp"
#include "arts/Simulation.hpp"

#include "catch2/catch.hpp"

#include <math.h>

using namespace arts;
using namespace std;

#define REFERENCE_ROOM "rsc/environment/4x5x3room.obj"
#define RECEIVER_FILE "rsc/receiver/los.csv"
#define SIMULATION_TIME_S 0.1

void CalculateError(vector<Point> &estimated_locations, vector<Source> &sources)
{
    double sum = 0, err = 0, min = 0, max = 0, avg = 0;

    for (uint32_t tx = 0; tx < sources.size(); ++tx)
    {
        err = sqrt((estimated_locations[tx] - sources[tx].GetPosition()).squared_length());
        sum += err;
        if (err > max)
        {
            max = err;
        }
        if (err < min)
        {
            min = err;
        }
    }

    cout << "Error avg: " << sum / sources.size() << endl;
    cout << "Error min: " << min << endl;
    cout << "Error max: " << max << endl;
}

TEST_CASE("TestLOSEstimator", "[estimator]")
{
    Environment env;
    env.Parse(REFERENCE_ROOM);
    Range range = env.GetRange();
    uint32_t order = 0;

    vector<Source> sources;
    Source::Generate(range, sources, 10);
    cout << sources.size() << endl;

    vector<Receiver> receivers;
    Receiver::Parse(RECEIVER_FILE, receivers);
    cout << receivers.size() << endl;

    Simulation sim(env, receivers, sources);
    vector<vector<vector<Path>>> paths(receivers.size(), vector<vector<Path>>(sources.size()));
    vector<vector<AudioFile<double>>> audioFiles(receivers.size(), vector<AudioFile<double>>(sources.size()));

    sim.Run(SIMULATION_TIME_S, order, paths, audioFiles);

    LOSEstimator estimator(receivers);
    vector<Point> locations;
    map<uint32_t, AudioFile<double>> samples_sorted;

    // Sort Samples
    for (uint32_t tx = 0; tx < sources.size(); ++tx)
    {
        for (uint32_t rx = 0; rx < receivers.size(); ++rx)
        {
            samples_sorted[rx] = audioFiles[rx][tx];
        }
        estimator.Locate(samples_sorted, locations);
        samples_sorted.clear();
    }
    CalculateError(locations, sources);
}
