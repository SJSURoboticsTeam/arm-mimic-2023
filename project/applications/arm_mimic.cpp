#include <libhal-util/steady_clock.hpp>
#include <string>

#include "../hardware_map.hpp"
#include "implementations/adc_mux_cd74hc4067.hpp"
#include "implementations/common.hpp"



hal::status application(arm_mimic::hardware_map& p_map) {
    using namespace std::chrono_literals;
    using namespace hal::literals;
    const uint8_t N = 6;

    while (true) {

        auto digital_multiplexer = AdcMuxCd74hc4067(p_map.adc_pin, p_map.signal_0, p_map.signal_1, p_map.signal_2, p_map.signal_3, p_map.steady_clock);
        
        std::array<uint8_t, N> channels = {0, 1, 2, 3, 4, 5};
        std::array<float, N> degree_conversion = {360, 90, 90, 90, 360, 360}; 
        auto output_voltages = HAL_CHECK(digital_multiplexer.read_all<N>(channels));
        std::array<float, N> results = {};
        for (auto i = 0; i < N; i++) {
            float true_degree = arm_mimic::common::voltage_to_degree(output_voltages[i], 3.3, 360);
            results[i] = HAL_CHECK(arm_mimic::common::degree_phase_shift(true_degree, degree_conversion[i]));
        }
        //HAL_CHECK(arm_mimic::common::send_data_mc<N>(results));
        
       HAL_CHECK((arm_mimic::common::print_array<float, N>(results, *p_map.terminal)));
    }

    return hal::success();
}