#pragma once

#include <libhal/functional.hpp>
#include <libhal/input_pin.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/serial.hpp>
#include <libhal/steady_clock.hpp>
#include <libhal/adc.hpp>
#include <libhal/i2c.hpp>
#include "implementations/i2c_tla2528.hpp"


namespace arm_mimic {
struct hardware_map
{
  hal::serial* terminal; // UART serial abstraction
  // digital signal pins to control mux
  hal::output_pin* signal_0;
  hal::output_pin* signal_1;
  hal::output_pin* signal_2;
  hal::output_pin* signal_3;
  hal::i2c* i2c;

  hal::steady_clock* steady_clock; // clock of mcu
  hal::adc* adc_pin; // adc input pin to read from mux
  hal::callback<void()> reset; // reset function

};
}  // namespace arm_mimic

// Application function must be implemented by one of the compilation units
// (.cpp) files.
hal::result<arm_mimic::hardware_map> initialize_target();
hal::status application(arm_mimic::hardware_map& p_map);