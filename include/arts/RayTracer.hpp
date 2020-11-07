#ifndef __ARTS_RAY_TRACER_HPP__
#define __ARTS_RAY_TRACER_HPP__

#include "arts/Environment.hpp"
#include "arts/Path.hpp"
#include "arts/Receiver.hpp"
#include "arts/Source.hpp"

#include <vector>

namespace arts
{
    class RayTracer
    {
    public:
        RayTracer(Environment &env, uint32_t order);
        void Run(Receiver &r, Source &s, std::vector<Path> &paths);

    private:
        void CreatePath(Source &src, Receiver &rcv, std::vector<Object*> &mirrors, std::vector<Path> &paths);
        void CreatePath(Source &src, Receiver &rcv, std::vector<Path> &paths);

        Environment &mEnvironment;
        uint32_t mOrder;
    };
} // namespace arts

#endif /* __ARTS_RAY_TRACER_HPP__ */
