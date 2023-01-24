#include <libhal-armcortex/dwt_counter.hpp>
#include <libhal-armcortex/startup.hpp>
#include <libhal-armcortex/system_control.hpp>
#include <libhal-lpc40xx/output_pin.hpp>
#include <libhal-lpc40xx/system_controller.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal-util/serial.hpp>

#include "implementations/adc_mux_cd74hc4067.hpp"
#include "implementations/common.hpp"
#include "libhal_configuration/hardware_map.hpp"

using namespace hal::literals;
using namespace std::chrono_literals;

hal::status app_main(arm_mimic::hardware_map& p_map) {

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
    
    HAL_CHECK(arm_mimic::common::send_data_mc<N>(*p_map.terminal, results));
    HAL_CHECK(hal::delay(*p_map.steady_clock, 500ms));
    
  }

  return hal::success();
}

int main()
{
 

  // Initializing the data section initializes global and static variables and
  // is required for the standard C library to run.
  hal::cortex_m::initialize_data_section();
  hal::cortex_m::system_control::initialize_floating_point_unit();

  auto init_result = initialize_target();
  if (!init_result)
    hal::halt();
  
  auto p_map = init_result.value();

  // while (true)
  // {
  //   hal::write(*p_map.terminal, "test");
  //   hal::delay(*p_map.steady_clock, 500ms);
  // }
  

  auto app_result = app_main(p_map);
  
  if (!app_main) {
      p_map.reset();
    } else {
      hal::halt();
    }
  

  return 0;
}

namespace boost {
void throw_exception([[maybe_unused]] std::exception const& p_error)
{
  std::abort();
}
} // namespace boost
