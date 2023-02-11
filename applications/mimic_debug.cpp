#include <libhal-armcortex/dwt_counter.hpp>
#include <libhal-armcortex/startup.hpp>
#include <libhal-armcortex/system_control.hpp>
#include <libhal-lpc40xx/output_pin.hpp>
#include <libhal-lpc40xx/system_controller.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal-util/serial.hpp>
#include <libhal/steady_clock.hpp>

#include "implementations/adc_mux_cd74hc4067.hpp"
#include "implementations/degree_manipulation.hpp"
#include "implementations/mission_control_handler.hpp"
#include "implementations/utility.hpp"

#include "hardware_map.hpp"

using namespace hal::literals;
using namespace std::chrono_literals;

hal::status application(arm_mimic::hardware_map& p_map) {

  const uint8_t N = 6;

  while (true) {
    auto digital_multiplexer = arm_mimic::AdcMuxCd74hc4067(p_map.adc_pin, p_map.signal_0, p_map.signal_1, p_map.signal_2, p_map.signal_3, p_map.steady_clock);
    
    std::array<uint8_t, N> channels = {0, 1, 2, 3, 4, 5};
    std::array<float, N> degree_conversion = {360, 180, 180, 180, 360, 180}; 
    std::array<float, N> voltage_maxes = {0.3333, 3.3, 3.3, 3.3, 0.3333, 3.3};
    auto output_voltages = HAL_CHECK(digital_multiplexer.read_all<N>(channels));
    std::array<float, N> true_degrees = {};
    std::array<float, N> results = {};
    for (auto i = 0; i < N; i++) {
        float true_degree = arm_mimic::voltage_to_degree(output_voltages[i], voltage_maxes[i], 360);
        true_degrees[i] = true_degree;
        if (degree_conversion[i] != 360) {
        results[i] = HAL_CHECK(arm_mimic::degree_phase_shift(true_degree, degree_conversion[i]));
        }
        else {
        results[i] = true_degree;
        }
    }
    
    (void)hal::write(*p_map.terminal, "Phase shift:\n");
    (void)arm_mimic::print_array<float, N>(results, *p_map.terminal);
    HAL_CHECK(hal::delay(*p_map.steady_clock, 1ms));
    (void)hal::write(*p_map.terminal, "Voltage:\n");
    (void)arm_mimic::print_array<float, N>(output_voltages, *p_map.terminal);
    HAL_CHECK(hal::delay(*p_map.steady_clock, 1ms)); 
    (void)hal::write(*p_map.terminal, "True degree:\n");
    arm_mimic::print_array<float, N>(true_degrees, *p_map.terminal);
    HAL_CHECK(hal::delay(*p_map.steady_clock, 1ms));
    (void)hal::write(*p_map.terminal, "\n");
    HAL_CHECK(hal::delay(*p_map.steady_clock, 250ms));
  }

  return hal::success();
}