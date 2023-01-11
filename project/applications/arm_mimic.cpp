#include <libhal-util/steady_clock.hpp>
#include "../hardware_map.hpp"

hal::status application(arm_mimic::hardware_map& p_map) {
    using namespace std::chrono_literals;
    using namespace hal::literals;

    while (true) {
        float adc_value = HAL_CHECK(p_map.adc_pin->read());
        HAL_CHECK(hal::delay(*p_map.steady_clock, 1000ms));
    }

    return hal::success();
}