#pragma once

#include <libhal/functional.hpp>
#include <libhal/input_pin.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/serial.hpp>
#include <libhal/steady_clock.hpp>
#include <libhal/adc.hpp>

namespace arm_mimic {
struct hardware_map
{
  // UART serial abstraction
  hal::serial* terminal; 
  // digital signal pins to control mux
  hal::output_pin* signal_0;
  hal::output_pin* signal_1;
  hal::output_pin* signal_2;
  hal::output_pin* signal_3;

  hal::steady_clock* steady_clock; // clock of mcu
  hal::callback<void()> reset; // reset function
  hal::adc* adc_pin; // adc input pin to read from mux

};
}  // namespace arm_mimic

namespace hello_world {
  struct hardware_map {
    hal::serial* terminal;
    hal::steady_clock* steady_clock; // clock of mcu
    hal::callback<void()> reset; // reset function
  };
}

// Application function must be implemented by one of the compilation units
// (.cpp) files.
hal::status application(arm_mimic::hardware_map& p_map);
hal::result<arm_mimic::hardware_map> initialize_arm_mimic();

hal::status application(hello_world::hardware_map& p_map);
hal::result<hello_world::hardware_map> initialize_hw();
