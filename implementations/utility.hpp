#pragma once
// #include <libhal/error.hpp>
// #include <assert.h> // TODO: remove after implemented
#include <libhal-util/map.hpp>
#include <libhal-util/serial.hpp>

namespace arm_mimic{

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
}