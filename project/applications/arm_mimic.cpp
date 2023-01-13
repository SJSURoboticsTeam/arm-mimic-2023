#include <libhal-util/steady_clock.hpp>

#include "../hardware_map.hpp"
#include "headers/adc_mux_cd74hc4067.hpp"

hal::status application(arm_mimic::hardware_map& p_map) {
    using namespace std::chrono_literals;
    using namespace hal::literals;

    while (true) {
        float adc_value = HAL_CHECK(p_map.adc_pin->read());
        HAL_CHECK(hal::delay(*p_map.steady_clock, 1000ms));
        auto mux = AdcMuxCd74hc4067(p_map.adc_pin, p_map.signal_0, p_map.signal_1, p_map.signal_2, p_map.signal_3, p_map.steady_clock);
    }

    return hal::success();
}