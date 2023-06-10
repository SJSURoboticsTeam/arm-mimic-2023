#include <libhal-armcortex/dwt_counter.hpp>
#include <libhal-armcortex/startup.hpp>
#include <libhal-armcortex/system_control.hpp>
#include <libhal-lpc40xx/output_pin.hpp>
#include <libhal-lpc40xx/system_controller.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal-util/serial.hpp>
#include <libhal/steady_clock.hpp>
#include <libhal-util/i2c.hpp>


#include "implementations/adc_mux_cd74hc4067.hpp"
#include "implementations/degree_manipulation.hpp"
#include "implementations/mission_control_handler.hpp"
#include "implementations/utility.hpp"

#include "hardware_map.hpp"

hal::status application(arm_mimic::hardware_map& p_map) {
    auto& i2c2 = *p_map.i2c;
  auto& clock = *p_map.steady_clock;
  auto& terminal = *p_map.terminal;

  auto tla2528 =
    HAL_CHECK(arm_mimic::TLA2528::create(i2c2, clock));

    while(true){
        HAL_CHECK(hal::delay(clock, 500ms));
        auto data = HAL_CHECK(tla2528.read_all());
        hal::print<64>(terminal, "DATA: %d\n", data);
        HAL_CHECK(hal::delay(clock, 1000ms));
    }
}