#include "logger.hpp"

#include "utils.hpp"
#include "timeit.hpp"

#include <chrono>

using namespace std::chrono_literals;
constexpr auto interval = 100us;

void run_test()
{
    timeit::spin_delay(interval);
    timeit::spin_delay(interval);
    timeit::spin_delay(interval);
    timeit t1;
    for (uint32_t count = 0; count < 10000; count++)
    {
        timeit::spin_delay(interval);
        t1.lap();
    }
    LOG_INFO("preformance: " + t1.record());
}

int main(int argc, char** argv)
{
    LOG_INFO("Starting application...");

    go_realtime();
    run_test();
    run_test();

    return 0;
}
