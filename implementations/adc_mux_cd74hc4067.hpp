#pragma once
#include <libhal/adc.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/steady_clock.hpp>
#include <array>
#include <libhal/error.hpp>
#include <libhal-util/map.hpp>
#include <libhal-util/math.hpp>

namespace arm_mimic{
class AdcMuxCd74hc4067
{
public:
    AdcMuxCd74hc4067(
        hal::adc* adc_pin,
        hal::output_pin* s0, 
        hal::output_pin* s1, 
        hal::output_pin* s2, 
        hal::output_pin* s3,
        hal::steady_clock* clk
    );

    hal::result<void> swap_channels(uint8_t number);
    hal::result<float> read_one(uint8_t channel);

    template<uint8_t N>
    hal::result<std::array<float, N>> read_all(std::array<uint8_t, N> channel_list);
    
private:
    hal::adc* adc_output_;
    hal::output_pin* signal_0_;
    hal::output_pin* signal_1_;
    hal::output_pin* signal_2_;
    hal::output_pin* signal_3_;
    hal::steady_clock* clock_;


};


AdcMuxCd74hc4067::AdcMuxCd74hc4067(
        hal::adc* adc_pin,
        hal::output_pin* s0, 
        hal::output_pin* s1, 
        hal::output_pin* s2, 
        hal::output_pin* s3,
        hal::steady_clock* clk): adc_output_{adc_pin}, 
                                signal_0_{s0}, 
                                signal_1_{s1}, 
                                signal_2_{s2}, 
                                signal_3_{s3}, 
                                clock_{clk} {};



hal::result<void> AdcMuxCd74hc4067::swap_channels(uint8_t number) {
    // Read each individual bit in the given number and set digital pin state, number should be 4 bits long only
    using namespace std::chrono_literals;
    using namespace hal::literals;
 
    HAL_CHECK(signal_0_->level(bool(number & (1 << 0))));
    HAL_CHECK(signal_1_->level(bool(number & (1 << 1)))); 
    HAL_CHECK(signal_2_->level(bool(number & (1 << 2)))); 
    HAL_CHECK(signal_3_->level(bool(number & (1 << 3)))); 
    HAL_CHECK(hal::delay(*clock_, 5ms));

    return hal::success();
}

hal::result<float> AdcMuxCd74hc4067::read_one(uint8_t channel) {
    using namespace std::chrono_literals;
    using namespace hal::literals;
    
    if (channel > 16)
        return hal::new_error("Channel is greater than 16", std::errc::invalid_argument);
    
    float average = 0;
    HAL_CHECK(swap_channels(channel)); // checking early to ensure continuation

    // take ten readings then average them
    const float average_count = 5;
    float digital_value;
    for (int i = 0; i < average_count; i++) {
        digital_value = HAL_CHECK(adc_output_->read());
        HAL_CHECK(hal::delay(*clock_, 1ms));
        digital_value = hal::absolute_value(digital_value); // we assume positive reference voltage will be used
        float voltage = hal::map(
            digital_value,
            std::pair<float, float>(0.0f, 100.0f), // FIXME: get real maximum
            std::pair<float, float>(0.0f, 3.3f)
        ) * 100;
        average += voltage;
    }

    return average / average_count;
}

template<uint8_t N>
hal::result<std::array<float, N>> AdcMuxCd74hc4067::read_all(std::array<uint8_t, N> channel_list) {
    if (N > 16)
        return hal::new_error("Only 16 channels on mux");
    
    std::array<float, N> results;
    results.fill(0);

    for (uint8_t i = 0; i < N; i++) {
        results[i] = HAL_CHECK(read_one(channel_list[i]));
    }

    return results;

}
}