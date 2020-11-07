#ifndef __ARTS_SOURCE_HPP__
#define __ARTS_SOURCE_HPP__

#include "arts/CGALTypes.hpp"

#include <memory>
#include <string>
#include <vector>

namespace arts
{
    class SourceSignal
    {
    public:
        virtual double GetSample(double time) = 0;
        virtual ~SourceSignal(){};
    };

    class SinglePulseSignal : public SourceSignal
    {
    public:
        static double DEFAULT_AMPLITUDE;
        static double DEFAULT_WIDTH;
        SinglePulseSignal();
        SinglePulseSignal(double amplitude, double width);
        double GetSample(double time);

    private:
        double mAmplitude;
        double mWidth;
    };

    class DiracDeltaSignal : public SourceSignal
    {
    public:
        DiracDeltaSignal();
        DiracDeltaSignal(double a);
        double GetSample(double time);

    private:
        double mA;
    };

    class SineSignal : public SourceSignal
    {
    public:
        static const uint32_t DEFAULT_AMPLITUDE = 1;
        static const uint32_t DEFAULT_FREQUENCY = 20000;
        SineSignal();
        SineSignal(double amplitude, uint32_t frequency);
        double GetSample(double time);

    private:
        double mAmplitude;
        uint32_t mFrequency;
    };

    class CustomSignal : public SourceSignal
    {
        enum RetCode
        {
            OK,
            FILE_DOES_NOT_EXIST,
            FILE_PARSING_FAILED
        };

    public:
        CustomSignal();
        RetCode Parse(std::string filePath);
    };

    class Source
    {
    public:
        enum RetCode {
            OK,
            NOK
        };

        Source();
        Source(Point position);
        Source(Point position, std::shared_ptr<SourceSignal> sourceSignal);
        Point &GetPosition();
        void SetPosition(Point &position);
        void MovePosition(Vector &vector);
        SourceSignal &GetSourceSignal();
        static void Generate(Range &r, std::vector<Source> &sources, uint32_t amount);
        uint32_t GetId();
        static RetCode Parse(std::string path, std::vector<Source> &sources);

    private:
        Point mPosition;
        uint32_t mId;
        std::shared_ptr<SourceSignal> mSourceSignal;

        static uint32_t mNextId;
    };

} // namespace arts

#endif /* __ARTS_SOURCE_HPP__ */
