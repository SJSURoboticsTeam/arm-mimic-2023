#pragma once
#include <libhal/error.hpp>
#include <assert.h> // TODO: remove after implemented
#include <libhal-util/map.hpp>
#include <libhal-util/serial.hpp>
#include <libhal/serial.hpp>
#include <libhal-util/serial.hpp>

namespace arm_mimic::common {

    template<uint8_t N>
    hal::status send_data_mc(hal::serial& uart, std::array<float, N> raw_data, bool is_network = false) {
        // Structure of json file
        /* 
        { 
            HB: 0,
            IO: 1,
            M: 'M',
            Angles: [$ROTUNDA, $SHOULDER, $ELBOW, $WRIST_PITCH, $WRIST_ROLL, $END_EFFECTOR]
        }
        */
    char* json_str = "{\"HB\":\"0\",\"IO\":\"1\",\"M\":\"M\",\"CMD\":[";
    for (auto i = 0; i < N; i++) {
            if (i != N - 1)
                sprintf(json_str, "%f,", raw_data[i]);
            else
                sprintf(json_str, "%f", raw_data[i]);
    }


    sprintf(json_str, "%s", "]}");
    if (is_network) {
        // TODO: implement networking
        assert("POST to esp/output serial not implemented");
    }
    else
        hal::print<128>(uart, json_str);
        
    return hal::success();
    }

    hal::result<float> degree_phase_shift(float input_degree, float new_origin) {
        if (new_origin < 0 || new_origin > 360)
            return hal::new_error("This function only considers the unit circle. Please set new_zero to the position on the unit circle it will be.");
        
        while (input_degree > 360 || input_degree < -360) {
            // take modulus of a float without losing data, thanks Corey!
            input_degree = input_degree / 360;
            input_degree = (input_degree - static_cast<int>(input_degree)) * 360;
        }

        return input_degree - new_origin;
    }

    float voltage_to_degree(float value, float max_voltage, float max_degree) {
        return hal::map(value, 
        std::pair<float, float>(0, max_voltage), 
        std::pair<float, float>(0, 360));
    }

    template<typename T, uint8_t N>
    hal::status print_array(std::array<T, N>& array, hal::serial& uart) {

        const char* index = "Index: ";
        const char* value = " Value: ";
        for (int i = 0; i < N; i++) {
            hal::print<128>(uart, "%s %i %s %f\n", index, i, value, array[i]);
        }

        return hal::success();
    }

} // arm_mimic::common
