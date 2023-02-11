#include <libhal-armcortex/dwt_counter.hpp>
#include <libhal-armcortex/startup.hpp>
#include <libhal-armcortex/system_control.hpp>

#include <libhal-lpc40xx/constants.hpp>
#include <libhal-lpc40xx/input_pin.hpp>
#include <libhal-lpc40xx/output_pin.hpp>
#include <libhal-lpc40xx/adc.hpp>
#include <libhal-lpc40xx/system_controller.hpp>
#include <libhal-lpc40xx/uart.hpp>

#include "hardware_map.hpp"

// hardware locations for pins and channels on the lpc4078
constexpr int SIGNAL_0_PORT = 2;
constexpr int SIGNAL_1_PORT = 2;
constexpr int SIGNAL_2_PORT = 2;
constexpr int SIGNAL_3_PORT = 2;


constexpr int SIGNAL_0_PIN = 1;
constexpr int SIGNAL_1_PIN = 4;
constexpr int SIGNAL_2_PIN = 6;
constexpr int SIGNAL_3_PIN = 8;

constexpr int ADC_CHANNEL = 4;

hal::result<arm_mimic::hardware_map> initialize_target()
{
  using namespace hal::literals;

  hal::cortex_m::initialize_data_section();
  hal::cortex_m::system_control::initialize_floating_point_unit();

  // Set the MCU to the maximum clock speed
  // Requires oscilation crystal to set a clock maximum
  // HAL_CHECK(hal::lpc40xx::clock::maximum(10.0_MHz));


  // Create a hardware counter
  auto& clock = hal::lpc40xx::clock::get();
  auto cpu_frequency = clock.get_frequency(hal::lpc40xx::peripheral::cpu);
  static hal::cortex_m::dwt_counter counter(cpu_frequency);

  // Get and initialize UART0 for UART based terminal logging
  auto& uart0 = HAL_CHECK((hal::lpc40xx::uart::get<0, 64>(hal::serial::settings{
    .baud_rate = 38400,
  })));

  auto& signal_0_raw = HAL_CHECK((hal::lpc40xx::output_pin::get<SIGNAL_0_PORT, SIGNAL_0_PIN>()));
  auto& signal_1_raw = HAL_CHECK((hal::lpc40xx::output_pin::get<SIGNAL_1_PORT, SIGNAL_1_PIN>()));
  auto& signal_2_raw = HAL_CHECK((hal::lpc40xx::output_pin::get<SIGNAL_2_PORT, SIGNAL_2_PIN>()));
  auto& signal_3_raw = HAL_CHECK((hal::lpc40xx::output_pin::get<SIGNAL_3_PORT, SIGNAL_3_PIN>()));

  auto& adc_inp_raw = HAL_CHECK((hal::lpc40xx::adc::get<ADC_CHANNEL>()));


  return arm_mimic::hardware_map {
    .terminal = &uart0,
    .signal_0 = &signal_0_raw,
    .signal_1 = &signal_1_raw,
    .signal_2 = &signal_2_raw,
    .signal_3 = &signal_3_raw,
    .steady_clock = &counter,
    .adc_pin = &adc_inp_raw,
    .reset = []() { hal::cortex_m::system_control::reset(); },
  };
}
