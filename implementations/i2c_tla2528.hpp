#pragma once

#include <libhal-util/i2c.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal/units.hpp>
#include <span>

using namespace hal::literals;
using namespace std::chrono_literals;

namespace arm_mimic{
class TLA2528
{
private:
    TLA2528(hal::i2c& p_i2c, hal::steady_clock& c)
    : m_i2c(p_i2c)
    , steady_clock{ c }
    {

    }
public:
    // 6 channel addresses for analog inputs
    // Single register READ Command: 0001 0000b
    // Single register WRITE Command: 0000 1000b 

    enum addresses : hal::byte
  {
    i2c_selection = 0b001'0111, // 0 ohm setting (17h)
    single_read = 0b0001'0000,
    single_write = 0b0000'1000,
    cont_read = 0b0011'0000,
    cont_write = 0b0010'1000
  };

    static hal::result<TLA2528> create(hal::i2c& p_i2c,
                                        hal::steady_clock& clock)
  {
    TLA2528 adc_mux(p_i2c, clock);
    //HAL_CHECK(adc_mux.start());
    return adc_mux;
  }

  hal::result<float> read_one()
  {
    std::array<hal::byte, 9> buffer;
    std::array<hal::byte, 1> single_read_address{ { single_read } };
    HAL_CHECK(hal::write(m_i2c, addresses::i2c_selection, single_read_address, hal::never_timeout()));
    HAL_CHECK(hal::delay(steady_clock, 1ms));
    HAL_CHECK(
      hal::read(m_i2c, i2c_selection, buffer, hal::never_timeout()));
  }
  
  hal::result<std::array<float, 4>> read_all() 
  {
    std::array<hal::byte, 1> read_all_address{ { cont_read } };
    HAL_CHECK(hal::write(m_i2c, addresses::i2c_selection, read_all_address, hal::never_timeout()));
    HAL_CHECK(hal::delay(steady_clock, 1ms));
  }
//   hal::status start()
//   {
//     HAL_CHECK(hal::write(m_i2c))
//   }

  hal::i2c& m_i2c;
  hal::steady_clock& steady_clock;
};
}