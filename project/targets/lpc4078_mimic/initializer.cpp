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

// TODO: move these probably
#define SIGNAL_0 2, 1
#define SIGNAL_1 2, 4
#define SIGNAL_2 2, 6
#define SIGNAL_3 2, 8
#define ADC_CHANNEL 4

hal::result<arm_mimic::hardware_map> initialize_arm_mimic()
{
  using namespace hal::literals;

  hal::cortex_m::initialize_data_section();
  hal::cortex_m::system_control::initialize_floating_point_unit();

  // Set the MCU to the maximum clock speed
  HAL_CHECK(hal::lpc40xx::clock::maximum(10.0_MHz));

  // Create a hardware counter
  auto& clock = hal::lpc40xx::clock::get();
  auto cpu_frequency = clock.get_frequency(hal::lpc40xx::peripheral::cpu);
  static hal::cortex_m::dwt_counter counter(cpu_frequency);

  // Get and initialize UART0 for UART based terminal logging
  auto& uart0 = HAL_CHECK((hal::lpc40xx::uart::get<0, 64>(hal::serial::settings{
    .baud_rate = 38400,
  })));

  auto& signal_0_raw = HAL_CHECK((hal::lpc40xx::output_pin::get<SIGNAL_0>()));
  auto& signal_1_raw = HAL_CHECK((hal::lpc40xx::output_pin::get<SIGNAL_1>()));
  auto& signal_2_raw = HAL_CHECK((hal::lpc40xx::output_pin::get<SIGNAL_2>()));
  auto& signal_3_raw = HAL_CHECK((hal::lpc40xx::output_pin::get<SIGNAL_3>()));

  auto& adc_inp_raw = HAL_CHECK((hal::lpc40xx::adc::get<ADC_CHANNEL>()));


  return arm_mimic::hardware_map {
    .terminal = &uart0,
    .signal_0 = &signal_0_raw,
    .signal_1 = &signal_1_raw,
    .signal_2 = &signal_2_raw,
    .signal_3 = &signal_3_raw,
    .steady_clock = &counter,
    .reset = []() { hal::cortex_m::system_control::reset(); },
    .adc_pin = &adc_inp_raw,
  };
}