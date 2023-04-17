#pragma once
#include <libhal/adc.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/steady_clock.hpp>
#include <array>
#include <span>
#include <libhal/error.hpp>
#include <libhal-util/map.hpp>
#include <libhal-util/math.hpp>

using namespace hal::literals;
using namespace std::chrono_literals;

namespace hal {

// This function does not take into account setup and hold times.
template<size_t N_signal>
hal::result<void> mux_swap_channels(size_t position, std::span<hal::output_pin> signal_pins) {

    static_assert(signal_pins.size() == N_signal);
    for (int i = 0; i < N_signal; i++) {
        HAL_CHECK(signal_pins[i].level(bool(position & (1 << i))));
    }
    return hal::success();
}  

template<size_t N_signal>
class AdcMultiplexer {
public:
    class AdcMuxPin: public hal::adc {
    public:
        // Constructor should have number on mux
        AdcMuxPin(uint8_t mux_port): mux_port_{mux_port} {};
        // TODO: impl read function for use with a mux
        virtual hal::result<read_t> driver_read() override {
            mux_swap_channels<N_signal>(mux_port_, signal_pins_);
            return HAL_CHECK(source_pin_->read()); // expects some driver pin
        }

    private:
        const uint8_t mux_port_;
        
    };

    // Constructor: keep track of how many pins are used on mux, set a max
    AdcMultiplexer(uint8_t mux_max, std::span<hal::output_pin> signal_pins, hal::adc* source_pin): mux_max_{mux_max}, source_pin_{source_pin} {
        static_assert(signal_pins.size() == N_signal);
        signal_pins_ = signal_pins;
    };

    hal::result<AdcMuxPin&> pin_factory() {
        if (pins_used_ >= mux_max_) {
            return hal::new_error("Unable to add any more pins to this multiplexer.\n");
        }

        uint8_t new_mux_port = pins_used_++;

        auto& new_adc_pin = AdcMuxPin(new_mux_port);
        return new_adc_pin;
    }

private:
    u_int8_t pins_used_ = 0;
    u_int8_t mux_max_;
    std::span<hal::output_pin> signal_pins_;
    hal::adc* source_pin_;

};




} // namespace hal