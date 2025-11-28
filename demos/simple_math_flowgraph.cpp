#include <fmt/core.h>

#include <gnuradio-4.0/Graph.hpp>
#include <gnuradio-4.0/Scheduler.hpp>

// Blocks
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
        {"n_samples_max", 64000}
    });

    auto& sink = fg.emplaceBlock<gr::testing::NullSink<float>>();

    const char* connection_error = "connection_error";
    if (fg.connect<"out">(source).to<"in">(add) != gr::ConnectionResult::SUCCESS) {
        throw gr::exception(connection_error);
    }
    if (fg.connect<"out">(add).to<"in">(head) != gr::ConnectionResult::SUCCESS) {
        throw gr::exception(connection_error);
    }
    if (fg.connect<"out">(head).to<"in">(sink) != gr::ConnectionResult::SUCCESS) {
        throw gr::exception(connection_error);
    }

    fmt::print("Flowgraph created with {} blocks\n", fg.blocks().size());
    fmt::print("Running scheduler...\n");

    gr::scheduler::Simple<gr::scheduler::ExecutionPolicy::singleThreaded> sched;
    if (auto ret = sched.exchange(std::move(fg)); !ret) {
        throw std::runtime_error(std::format("failed to initialize scheduler: {}", ret.error()));
    }

    const auto ret = sched.runAndWait();
    if (!ret.has_value()) {
        std::print("scheduler error: {}", ret.error());
        std::exit(1);
    }

    fmt::print("Scheduler finished\n");

    return 0;
}