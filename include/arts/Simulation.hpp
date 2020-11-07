#ifndef __ARTS_SIMULATION_HPP__
#define __ARTS_SIMULATION_HPP__

#include "arts/Environment.hpp"
#include "arts/Receiver.hpp"
#include "arts/Source.hpp"

#include "audiofile/AudioFile.h"
namespace arts
{
    class Simulation
    {
    public:
        Simulation(Environment &environment, std::vector<Receiver> &receivers, std::vector<Source> &sources);
        void Run(double time_s, uint32_t order, std::vector<std::vector<std::vector<Path>>> &paths, std::vector<std::vector<AudioFile<double>>> &audioFiles);

    private:
        Environment &mEnvironment;
        std::vector<Receiver> &mReceivers;
        std::vector<Source> &mSources;
    };

} // namespace arts

#endif /* __ARTS_SIMULATOR_HPP__ */
