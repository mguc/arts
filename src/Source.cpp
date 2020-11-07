#include "arts/Source.hpp"
#include "fast-cpp-csv-parser/csv.h"

#include <math.h>

namespace arts
{
    double SinglePulseSignal::DEFAULT_AMPLITUDE = 1.0;
    double SinglePulseSignal::DEFAULT_WIDTH = 0.000001;
    SinglePulseSignal::SinglePulseSignal() : SinglePulseSignal(DEFAULT_AMPLITUDE, DEFAULT_WIDTH){};
    SinglePulseSignal::SinglePulseSignal(double amplitude, double width) : mAmplitude(amplitude), mWidth(width){};
    double SinglePulseSignal::GetSample(double time)
    {
        return (time >= 0 && time < mWidth) ? mAmplitude : 0;
    }

    DiracDeltaSignal::DiracDeltaSignal() : DiracDeltaSignal(1.0 / sqrt(M_PI)) {}
    DiracDeltaSignal::DiracDeltaSignal(double a) : mA(a) {}
    double DiracDeltaSignal::GetSample(double time)
    {
        return (exp(-(time * time) / (mA * mA)) / abs(mA) / sqrt(M_PI));
    }

    SineSignal::SineSignal() : SineSignal(DEFAULT_AMPLITUDE, DEFAULT_FREQUENCY) {}
    SineSignal::SineSignal(double amplitude, uint32_t frequency) : mAmplitude(amplitude), mFrequency(frequency) {}

    double SineSignal::GetSample(double time)
    {
        return (time >= 0 && time < 1.0/mFrequency) ? (mAmplitude * sin(2.0 * M_PI * mFrequency * time)) : 0;
    }

    uint32_t Source::mNextId = 0;

    Source::Source() : Source(Point(0, 0, 0)) {}
    Source::Source(Point position) : Source(position, std::shared_ptr<SourceSignal>(new SinglePulseSignal())) {}
    Source::Source(Point position, std::shared_ptr<SourceSignal> sourceSignal) : mPosition(position), mSourceSignal(sourceSignal), mId(mNextId++) {}

    Point &Source::GetPosition()
    {
        return mPosition;
    }
    SourceSignal &Source::GetSourceSignal()
    {
        return *mSourceSignal;
    }

    void Source::Generate(Range &r, std::vector<Source> &sources, uint32_t amount = 10)
    {
        amount = amount + 1;
        double delta_x = r.max.x() - r.min.x();
        double delta_y = r.max.y() - r.min.y();
        double delta_z = r.max.z() - r.min.z();

        double step_x = delta_x / amount;
        double step_y = delta_y / amount;
        double step_z = delta_z / amount;

        for (uint32_t i = 1; i < amount; ++i)
        {
            for (uint32_t j = 1; j < amount; ++j)
            {
                for (uint32_t k = 1; k < amount; ++k)
                {
                    sources.push_back(Source(Point(r.min.x() + i * step_x, r.min.y() + j * step_y, r.min.z() + k * step_z)));
                }
            }
        }
    }

    uint32_t Source::GetId()
    {
        return mId;
    }

    Source::RetCode Source::Parse(std::string path, std::vector<Source> &sources)
    {
        io::CSVReader<3> in(path);
        double x, y, z;
        while (in.read_row(x, y, z))
        {
            sources.push_back(Source(Point(x, y, z)));
        }
        return OK;
    }

} // namespace arts
