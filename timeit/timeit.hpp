#pragma once

#include <chrono>
#include <sstream>

class timeit {
  public:
    // Portable c++ timer. Probably better to use QueryPerformanceCounter, but this should be good enough.
    // 
    // Ideally use high_resolution_clock, but this may not necessarily be
    // monotonic. See here:
    //   https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
    //   https://stackoverflow.com/questions/67233593/stdchronohigh-resolution-clock-based-frame-timer
    // steady_clock is monotonic and hopefully has enough resolution? 
    using clock = std::chrono::steady_clock;
    using resolution = std::chrono::duration<double, std::chrono::milliseconds::period>;

    timeit()
    {
        _last = clock::now();
        reset();
    }

    void reset(double ave=3)
    {
        _min = 1e99;
        _max = 0;
        _ave = ave;
    }

    void start()
    {
        _last = clock::now();
    }

    void lap()
    {
        lap(clock::now());
    }

    void lap(clock::time_point tp)
    {
        double delta = std::chrono::duration_cast<resolution>(tp - _last).count();
        _last = tp;
        if (delta < _min)
        {
            _min = delta;
        }
        _ave = _ave * 0.9 + delta * 0.1;
        if (delta > _max)
        {
            _max = delta;
        }
    }

    std::string record()
    {
        // this is the c++20 way - needs #include <format>
        //return std::format("min = {:.3f}, ave = {:.3f}, max = {:.3f} (ms)", title, _min, _ave, _max);
        std::ostringstream sstream;
        sstream << std::fixed << std::setprecision(2) << "min=" << _min << ", ave=" << _ave << ", max=" << _max;
        return sstream.str();
    }

  private:
    clock::time_point _last;
    double _min;
    double _ave;
    double _max;

};

