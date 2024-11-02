#include "logger.hpp"

#include "utils.hpp"

int main(int argc, char** argv)
{
    LOG_INFO("Starting application...");
    sleep(10);
    LOG_TRACE("Test 10ms log");
    sleep(100);
    LOG_TRACE("Test 100ms log");

    return 0;
}
