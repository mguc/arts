
#include "arts/Environment.hpp"
#include "arts/PathSampler.hpp"
#include "arts/Receiver.hpp"
#include "arts/RayTracer.hpp"
#include "arts/Simulation.hpp"
#include "arts/Source.hpp"
// #include "arts/estimator/LocationEstimator.hpp"

#include "audiofile/AudioFile.h"
#include "cxxopts/cxxopts.hpp"

#include <boost/filesystem.hpp>
#include <CGAL/IO/OBJ_reader.h>
#include <CGAL/Qt/Basic_viewer_qt.h>

#include <iostream>
#include <string>
#include <vector>

#define DEFAULT_ORDER               3
#define OUTPUT_FOLDER               "out/"
#define DEFAULT_SIMULATION_TIME_S   0.1

using namespace std;
using namespace arts;

int main(int argc, char *argv[])
{
    cxxopts::Options options("arts", "Acoustic Ray Tracing Simulator CLI");
    options
        .allow_unrecognised_options()
        .add_options()
            ("e,environment", "Environment OBJ file", cxxopts::value<string>())
            ("h,help", "Print help")
            ("o,order", "Order of reflections", cxxopts::value<uint32_t>())
            ("r,receiver", "Receiver CSV file", cxxopts::value<string>())
            ("s,source", "Source CSV file", cxxopts::value<string>())
            ("t,time", "Simulation time", cxxopts::value<double>())
            ("v,view", "Open CGAL Basic Viewer");

    auto result = options.parse(argc, argv);

    Environment env;
    if (result.count("e"))
    {
        Environment::RetCode envRet = env.Parse(result["e"].as<string>());
        if (envRet != Environment::RetCode::OK)
        {
            cerr << "Environment::RetCode: " << envRet << endl;
            return -1;
        }
    }

    uint32_t order = DEFAULT_ORDER;
    if (result.count("o"))
    {
        order = result["o"].as<uint32_t>();
    }

    std::vector<Receiver> receivers;
    if (result.count("r"))
    {
        Receiver::RetCode rcvRet = Receiver::Parse(result["r"].as<string>(), receivers);
        if (rcvRet != Receiver::OK)
        {
            cerr << "Receiver::RetCode: " << rcvRet << endl;
            return -1;
        }
    }
    else
    {
        receivers.push_back(Receiver());
    }

    std::vector<Source> sources;
    if (result.count("s"))
    {
        Source::RetCode srcRet = Source::Parse(result["s"].as<string>(), sources);
        if (srcRet != Source::OK)
        {
            cerr << "Source::RetCode: " << srcRet << endl;
            return -1;
        }
    }
    else
    {
        sources.push_back(Source());
    }

    double simulation_time_s = DEFAULT_SIMULATION_TIME_S;
    if (result.count("t"))
    {
        simulation_time_s = result["t"].as<double>();
    }

    Simulation sim(env, receivers, sources);
    vector<vector<vector<Path>>> paths(receivers.size(), vector<vector<Path>>(sources.size()));
    vector<vector<AudioFile<double>>> audioFiles(receivers.size(), vector<AudioFile<double>>(sources.size()));

    sim.Run(simulation_time_s, order, paths, audioFiles);

    cout << "Exporting samples to " << OUTPUT_FOLDER << endl;
    boost::filesystem::create_directory(OUTPUT_FOLDER);
    string fileName;
    for(uint32_t r = 0; r < receivers.size(); ++r)
    {
        for (uint32_t s = 0; s < sources.size(); ++s) {
            fileName = OUTPUT_FOLDER + to_string(r+1) + to_string(s+1) + ".wav";
            audioFiles[r][s].save(fileName);
        }
    }

    if (result.count("v"))
    {
        QApplication app(argc, argv);
        CGAL::Basic_viewer_qt bv(app.activeWindow());

        env.Draw(bv);

        for (uint32_t r = 0; r < receivers.size(); ++r)
        {
            for (uint32_t s = 0; s < sources.size(); ++s)
            {
                for (auto it = paths[r][s].begin(); it != paths[r][s].end(); ++it)
                {
                    it->Draw(bv);
                }
            }
        }

        bv.show();
        return app.exec();
    }

    return 0;
}
