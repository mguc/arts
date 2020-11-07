#include "arts/PathSampler.hpp"
#include "arts/RayTracer.hpp"
#include "arts/Simulation.hpp"

namespace arts
{
    Simulation::Simulation(Environment &environment, std::vector<Receiver> &receivers, std::vector<Source> &sources) : mEnvironment(environment), mReceivers(receivers), mSources(sources) {}

    void Simulation::Run(double time_s, uint32_t order, std::vector<std::vector<std::vector<Path>>> &paths, std::vector<std::vector<AudioFile<double>>> &audioFiles)
    {
        uint32_t receiverCount = mReceivers.size();
        uint32_t sourceCount = mSources.size();
        Range range = mEnvironment.GetRange();
        RayTracer rayTracer(mEnvironment, order);
        uint32_t total_number_of_paths = 0;
        std::map<uint32_t, uint32_t> order_paths;
        
        std::cout << "Order: " << order << std::endl;
        std::cout << "Range (min): " << range.min << std::endl;
        std::cout << "Range (max): " << range.max << std::endl;
        std::cout << "# of Receivers: " << mReceivers.size() << std::endl;
        std::cout << "# of Sources: " << mSources.size() << std::endl;
        std::cout << "# of Objects in environment: " << mEnvironment.GetObjects().size() << std::endl;

        for (uint32_t i = 0; i <= order; ++i)
        {
            order_paths[i] = 0;
        }

        for (uint32_t r = 0; r < receiverCount; ++r)
        {
            for (uint32_t s = 0; s < sourceCount; ++s)
            {
                rayTracer.Run(mReceivers[r], mSources[s], paths[r][s]);
                for (auto p = paths[r][s].begin(); p != paths[r][s].end(); ++p)
                {
                    order_paths[p->NumberOfNodes() - 2] += 1;
                }
                total_number_of_paths += paths[r][s].size();
            }
        }
        std::cout << "# of Paths: " << total_number_of_paths << std::endl;
        for (uint32_t i = 0; i <= order; ++i)
        {
            std::cout << "  " << i << "-Paths: " << order_paths[i] << std::endl;
        }

        uint32_t total_number_of_samples = 0;
        for (uint32_t r = 0; r < receiverCount; ++r)
        {
            for (uint32_t s = 0; s < sourceCount; ++s)
            {
                if (paths[r][s].size())
                {
                    SamplePaths(paths[r][s], time_s, audioFiles[r][s]);
                }
                total_number_of_samples += audioFiles[r][s].getNumSamplesPerChannel();
            }
        }
        std::cout << "# of Samples: " << total_number_of_samples << std::endl;
    }
} // namespace arts
