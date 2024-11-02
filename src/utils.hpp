#pragma once

#include <thread>
#include <chrono>

void sleep(long ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void wait_keyboard();

bool is_own_console();
