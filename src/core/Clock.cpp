#include "Clock.hpp"

namespace dryout {

void Clock::update() {
    auto now = std::chrono::steady_clock::now();
    delta = std::chrono::duration<float>(now - last).count();
    last = now;
}

float Clock::getDeltaSeconds() {
    return delta;
}

} // namespace dryout
