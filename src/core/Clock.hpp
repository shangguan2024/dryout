#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <chrono>

namespace dryout {

class Clock {
  public:
    Clock() = default;
    ~Clock() = default;

    void update();

    float getDeltaSeconds();

  private:
    std::chrono::steady_clock::time_point last;
    float delta; // in seconds
};

} // namespace dryout

#endif // CLOCK_HPP
