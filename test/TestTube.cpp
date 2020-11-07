#include "arts/Path.hpp"
#include "arts/Simulation.hpp"

#include "catch2/catch.hpp"

#include <chrono>
#include <math.h>

using namespace arts;
using namespace std;

#define ROOM_FILE "rsc/environment/tube.obj"

#define SIMULATION_TIME_MIN     0.1
#define SIMULATION_TIME_STEP    0.2
#define SIMULATION_COUNT_MAX    3

#define NUMBER_OF_CUBES_MAX     10

uint32_t ExpectedNumberOfPaths(uint32_t M, uint32_t N, uint32_t K, uint32_t order) {
    uint32_t sum = 1;
    for(uint32_t i = 0; i < order; ++i) {
        sum += K*pow(K-1, i);
    }
    return M*N*sum;
}

TEST_CASE("TestTube", "[tube]") {
    Environment env;
    env.Parse(ROOM_FILE);
    Range range = env.GetRange();
    uint32_t order = 3;
    vector<Receiver> receivers;
    receivers.push_back(Receiver());

    uint32_t M = 0;
    double simulation_time_s = 0;
    vector<Source> sources;

    fstream results;
    results.open("TestTubeResults.txt", fstream::out);

    for(uint32_t X = 1; X <= NUMBER_OF_CUBES_MAX; ++X) {
        Source::Generate(range, sources, X);
        Simulation sim(env, receivers, sources);

        M = sources.size();
        simulation_time_s = SIMULATION_TIME_MIN;
        results << M << " & " << ExpectedNumberOfPaths(M, 1, 2, 3) << " & ";
    
        for(uint32_t i = 0; i < SIMULATION_COUNT_MAX; ++i) {
            vector<vector<vector<Path>>> paths(receivers.size(), vector<vector<Path>>(sources.size()));
            vector<vector<AudioFile<double>>> audioFiles(receivers.size(), vector<AudioFile<double>>(sources.size()));

            auto start = chrono::steady_clock::now();
            sim.Run(simulation_time_s, order, paths, audioFiles);
            auto end = chrono::steady_clock::now();
            chrono::duration<double> elapsed_seconds = end-start;
            results << elapsed_seconds.count();
            simulation_time_s += SIMULATION_TIME_STEP;

            if(i < (SIMULATION_COUNT_MAX - 1)) {
                results << " & ";
            }
            paths.clear();
            audioFiles.clear();
        }
        results << " \\\\" << endl;
        sources.clear();
    }

    results.close();
}
