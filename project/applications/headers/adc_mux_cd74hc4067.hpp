#pragma once
#include <libhal/adc.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/steady_clock.hpp>
#include <array>
#include <libhal/error.hpp>

class AdcMuxCd74hc4067
{
private:
    hal::output_pin* signal_0_;
    hal::output_pin* signal_1_;
    hal::output_pin* signal_2_;
    hal::output_pin* signal_3_;
    hal::steady_clock* clock_;
    hal::adc* adc_output_;

public:
    AdcMuxCd74hc4067(
        hal::adc* adc_pin,
        hal::output_pin* s0, 
        hal::output_pin* s1, 
        hal::output_pin* s2, 
        hal::output_pin* s3,
        hal::steady_clock* clk
    );

    hal::result<void> swapChannels(uint8_t number);
    hal::result<float> readOne(uint8_t channel);

    template<uint8_t N>
    hal::result<std::array<float, N>> readAll(std::array<uint8_t, N> channel_list);

};

