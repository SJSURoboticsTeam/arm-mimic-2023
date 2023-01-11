#include <libarmcortex/dwt_counter.hpp>
#include <libarmcortex/startup.hpp>
#include <libarmcortex/system_control.hpp>

#include <liblpc40xx/constants.hpp>
#include <liblpc40xx/input_pin.hpp>
#include <liblpc40xx/output_pin.hpp>
#include <liblpc40xx/adc.hpp>
#include <liblpc40xx/system_controller.hpp>
#include <liblpc40xx/uart.hpp>

#include "../../hardware_map.hpp"

hal::result<hello_world::hardware_map> initialize_hw() {
  using namespace hal::literals;

  hal::cortex_m::initialize_data_section();
  hal::cortex_m::system_control::initialize_floating_point_unit();

  // Set the MCU to the maximum clock speed
  HAL_CHECK(hal::lpc40xx::clock::maximum(10.0_MHz));

  // Create a hardware counter
  auto& clock = hal::lpc40xx::clock::get();
  auto cpu_frequency = clock.get_frequency(hal::lpc40xx::peripheral::cpu);
  static hal::cortex_m::dwt_counter steady_clock(cpu_frequency);

  // Get and initialize UART0 for UART based terminal logging
  auto& uart0 = HAL_CHECK((hal::lpc40xx::uart::get<0, 64>(hal::serial::settings{
    .baud_rate = 38400,
  })));
  return hello_world::hardware_map {
    .terminal = &uart0,
    .steady_clock = &steady_clock,
    .reset = []() { hal::cortex_m::system_control::reset(); }
  };
}