#include "arts/PathSampler.hpp"

#include <math.h>

namespace arts
{
    void SamplePaths(std::vector<Path> &paths, double time, AudioFile<double> &audioFile)
    {
        Receiver &r = paths[0].GetReceiver();
        Source &s = paths[0].GetSource();

        double sampling_time = 1.0 / r.GetSamplingFrequency();
        uint32_t last_sample = (time / sampling_time);
        double c = 340;
        double sensitivity = 0.01;
        double alpha = 0.5;

        // adjust audio file
        audioFile.setSampleRate(r.GetSamplingFrequency());
        audioFile.setAudioBufferSize(1, last_sample);

        // Fast forwarding
        uint32_t first_samples[paths.size()] = {0};
        uint32_t shortest_path_index = 0;
        first_samples[0] = (paths[0].Length() / c / sampling_time);
        for (uint32_t i = 1; i < paths.size(); ++i)
        {
            first_samples[i] = (paths[i].Length() / c / sampling_time);
            if (paths[i].Length() < paths[shortest_path_index].Length())
            {
                shortest_path_index = i;
            }
        }
        for (uint32_t i = 0; i < first_samples[shortest_path_index]; ++i)
        {
            audioFile.samples[0][i] = 0;
        }

        double sample = 0;
        for (uint32_t current_sample = first_samples[shortest_path_index]; current_sample <= last_sample; ++current_sample)
        {
            sample = 0;
            for (uint32_t i = 0; i < paths.size(); ++i)
            {
                if (current_sample < first_samples[i])
                {
                    continue;
                }
                sample += s.GetSourceSignal().GetSample((current_sample - first_samples[i]) * sampling_time) / (1 + paths[i].Length());
            }
            audioFile.samples[0][current_sample] = sample;
        }
    }
} // namespace arts
