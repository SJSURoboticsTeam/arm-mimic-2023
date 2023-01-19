#pragma once
#include <string>
#include <libhal/error.hpp>
#include <assert.h> // TODO: remove after implemented
#include <libhal-util/map.hpp>
#include <libhal-util/serial.hpp>
#include <libhal/serial.hpp>

namespace arm_mimic::common {
    template<uint8_t N>
    hal::status send_data_mc(std::array<float, N> raw_data) {
        // Structure of json file
        /* 
        { 
            HB: 0,
            IO: 1,
            M: 'M',
            Angles: [$ROTUNDA, $SHOULDER, $ELBOW, $WRIST_PITCH, $WRIST_ROLL, $END_EFFECTOR]
        }
        */
    std::string json_str = "{\"HB\":\"0\",\"IO\":\"1\",\"M\":\"M\",\"CMD\":[";
    for (auto i = 0; i < N; i++) {
            if (i != N - 1)
                json_str.append(std::to_string(raw_data[i]) + ",");
            else
                json_str.append(std::to_string(raw_data[i]));
    }

    json_str.append("]}");
    // TODO POST to esp/output serial.
    assert("POST to esp/output serial not implemented");
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

        std::string output_string = "";
        std::string index = "Index: ";
        std::string value = " Value: ";
        for (int i = 0; i < N; i++) {
            output_string += index.append(std::to_string(i));
            output_string += value.append(std::to_string(array[i]));
            output_string += "\n";
        }

        HAL_CHECK(hal::write(uart, output_string));

        return hal::success();
    }

} // arm_mimic::common
