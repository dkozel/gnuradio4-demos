#include <fmt/core.h>
#include <gnuradio-4.0/Graph.hpp>
#include <gnuradio-4.0/Scheduler.hpp>

#include <gnuradio-4.0/basic/SignalGenerator.hpp>
#include <gnuradio-4.0/math/Math.hpp>
#include <gnuradio-4.0/testing/NullSources.hpp>

int main()
{
    float sample_rate = 32000;

    gr::Graph fg;
    auto& source = fg.emplaceBlock<gr::blocks::basic::SignalGenerator<int>>({
        {"sample_rate", sample_rate},
        {"frequency", 0},
        {"amplitude", 2},
        {"offset", 0},
        {"phase", 0}
    });

    auto& add = fg.emplaceBlock<gr::blocks::math::AddConst<int>>({
        {"value", 1}
    });

    auto& head = fg.emplaceBlock<gr::blocks::testing::HeadBlock<int>>({
        {"max_samples", 1000}
    });

    auto& sink = fg.emplaceBlock<gr::blocks::testing::NullSink<int>>();

    fg.connect<"out">(source).to<"in">(add);
    fg.connect<"out">(add).to<"in">(head);
    fg.connect<"out">(head).to<"in">(sink);


    gr::scheduler::Simple sched{ std::move(fg) };
    sched.runAndWait();

    return 0;
}