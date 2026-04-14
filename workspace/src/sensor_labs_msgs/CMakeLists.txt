cmake_minimum_required(VERSION 3.8)
project(sensor_labs_msgs)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Dependencies for interfaces
find_package(std_msgs REQUIRED)

# Generate service
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/StartNewSeries.srv"
)

# Export runtime dependency
ament_export_dependencies(rosidl_default_runtime)

ament_package()
