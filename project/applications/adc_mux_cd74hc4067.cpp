#include <algorithm>

#include <libhal-util/steady_clock.hpp>

#include "adc_mux_cd74hc4067.hpp"


// FIXME: unsure if HAL_CHECK is needed here or if its taken care of by the HAL_CHECK'd wrapped functions
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
                                clock_{clk} {
    
    
    signal_0_->level(false); // might need to HAL_CHECK
    signal_1_->level(false); // might need to HAL_CHECK
    signal_2_->level(false); // might need to HAL_CHECK
    signal_3_->level(false); // might need to HAL_CHECK

};

hal::result<void> AdcMuxCd74hc4067::swapChannels(uint8_t number) {
    // Read each individual bit in the given number and set digital pin state, number should be 4 bits long only
    using namespace std::chrono_literals;
    using namespace hal::literals;

    signal_0_->level(bool(number & (1 << 0))); // might need to HAL_CHECK
    signal_1_->level(bool(number & (1 << 1))); // might need to HAL_CHECK
    signal_2_->level(bool(number & (1 << 2))); // might need to HAL_CHECK
    signal_3_->level(bool(number & (1 << 3))); // might need to HAL_CHECK
    hal::delay(clock_, 1ms);

    return hal::success();
}

hal::result<float> AdcMuxCd74hc4067::readOne(uint8_t channel) {
    if (channel > 16)
        return hal::new_error();
    
    float average = 0;
    HAL_CHECK(swapChannels(channel)); // checking early to ensure continuation

    // take ten readings then average them
    for (int i = 0; i < 10; i++) {
        float digital_value = HAL_CHECK(adc_output_->read());
        float voltage = std::clamp(digital_value, 0.0f, 3.3f);
        average += voltage;
    }

    return average / 10;
}

template<uint8_t N>
hal::result<std::array<float, N>> readAll(std::array<uint8_t, N> channel_list) {
    if (N > 16)
        return hal::new_error();
    
    std::array<float, N> results;
    results.fill(0);

    for (uint8_t i = 0; i < N; i++) 
        results[i] = readOne(channel_list[i]);
    
    return results;

}

