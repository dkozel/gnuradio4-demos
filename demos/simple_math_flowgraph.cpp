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
    auto& source = fg.emplaceBlock<gr::basic::SignalGenerator<float>>({
        {"sample_rate", sample_rate},
        {"frequency", 0.0f},
        {"amplitude", 2.0f},
        {"offset", 0.0f},
        {"phase", 0.0f}
    });

    auto& add = fg.emplaceBlock<gr::blocks::math::AddConst<float>>({
        {"value", 1.0f}
    });

    auto& head = fg.emplaceBlock<gr::testing::HeadBlock<float>>({
        {"n_samples_max", 1000}
    });

    auto& sink = fg.emplaceBlock<gr::testing::NullSink<float>>();

    [[maybe_unused]] auto conn1 = fg.connect<"out">(source).to<"in">(add);
    [[maybe_unused]] auto conn2 = fg.connect<"out">(add).to<"in">(head);
    [[maybe_unused]] auto conn3 = fg.connect<"out">(head).to<"in">(sink);

    fmt::print("Flowgraph created with {} blocks\n", fg.blocks().size());
    fmt::print("Running scheduler...\n");

    gr::scheduler::Simple sched;
    auto ret = sched.exchange(std::move(fg));
    [[maybe_unused]] auto result = sched.runAndWait();

    fmt::print("Scheduler finished\n");

    return 0;
}