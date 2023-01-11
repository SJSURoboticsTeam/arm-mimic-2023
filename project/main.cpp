#include "hardware_map.hpp"

int main()
{
  if constexpr (hal::config::project == "mimic") {
    auto init_result_arm_mimic = initialize_arm_mimic();

    if (!init_result_arm_mimic) {
      hal::halt();
    }

    auto mimic_hardware_map = init_result_arm_mimic.value();
    auto mimic_is_finished = application(mimic_hardware_map);
  } else {
    auto init_result_hello_world = initialize_hw();

      if (!init_result_hello_world) {
        hal::halt();
      }

    auto hello_world_hw_map = init_result_hello_world.value();
    auto world_is_finished = application(hello_world_hw_map);
  }
  return 0;
}

namespace boost {
void throw_exception([[maybe_unused]] std::exception const& p_error)
{
  std::abort();
}
}  // namespace boost
