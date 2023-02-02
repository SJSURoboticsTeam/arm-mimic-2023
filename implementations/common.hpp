#pragma once
#include <libhal/error.hpp>
#include <assert.h> // TODO: remove after implemented
#include <libhal-util/map.hpp>
#include <libhal-util/serial.hpp>
#include <libhal/serial.hpp>
#include <libhal-util/serial.hpp>
#include <cstdio>

namespace arm_mimic::common
{

    hal::status send_data_mc(hal::serial &uart, std::array<float, 6> raw_data, bool is_network = false)
    {
        // Structure of json file
        /*
        {
            heartbeat_count: 0,
            is_operational: 1,
            speed: 'M',
            angles: [$ROTUNDA, $SHOULDER, $ELBOW, $WRIST_PITCH, $WRIST_ROLL, $END_EFFECTOR]
        }
        */

        const char *json_format_str = "{\"heartbeat_count\":0,\"is_operational\":1,\"speed\":2,\"angles\":[%d,%d,%d,%d,%d,%d]}\n";
        char buffer[200];
        snprintf(buffer, 200, json_format_str,
                 static_cast<int>(raw_data[0]), static_cast<int>(raw_data[1]), static_cast<int>(raw_data[2]), static_cast<int>(raw_data[3]), static_cast<int>(raw_data[4]), static_cast<int>(raw_data[5]));
        if (!is_network)
            hal::print<200>(uart, buffer);
        else
            return hal::new_error("Not implemented");

        return hal::success();
    }

    hal::result<float> degree_phase_shift(float input_deg, float new_zero)
    {
        // if (new_zero < -360 || new_zero > 360)
        //     return hal::new_error("This function only considers the unit circle. Please set new_zero to the position on the unit circle it will be.");

        while (input_deg > 360 || input_deg < -360)
        {
            // take modulus of a float without losing data
            input_deg = input_deg / 360;
            input_deg = (input_deg - static_cast<int>(input_deg)) * 360;
        }

        return input_deg - new_zero;
    }

    float voltage_to_degree(float value, float max_voltage, float max_degree)
    {
        return hal::map(value,
                        std::pair<float, float>(0, max_voltage),
                        std::pair<float, float>(0, max_degree));
    }

    template <typename T, uint8_t N>
    hal::status print_array(std::array<T, N> &array, hal::serial &uart)
    {

        const char *index = "Index: ";
        const char *value = " Value: ";
        for (int i = 0; i < N; i++)
        {
            hal::print<128>(uart, "%s %i %s %f\n", index, i, value, array[i]);
        }

        return hal::success();
    }

} // arm_mimic::common
