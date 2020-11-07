#ifndef __ARTS_PATH_SAMPLER__
#define __ARTS_PATH_SAMPLER__

#include "arts/Path.hpp"

#include "audiofile/AudioFile.h"

namespace arts
{
    void SamplePaths(std::vector<Path> &paths, double time, AudioFile<double> &samples);
} // namespace arts

#endif /* __ARTS_PATH_SAMPLER__ */
