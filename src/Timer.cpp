#include "Timer.h"

Timer::Timer(float* diff, float* FPS)
    : m_diff(diff), m_FPS(FPS)
{
	start = std::chrono::steady_clock::now();
}

Timer::~Timer()
{
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> result = end - start;
    *m_diff = result.count() * 1000.0;
    *m_FPS = 1 / result.count();
}
