cmake_minimum_required(VERSION 3.8)
project(lab03)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
# set a new variable that holds all the build dependencies
# NOTE: this should include all packages from package.xml that are either depends or build depends for the nodes and libraries we are building
set(THIS_PACKAGE_INCLUDE_DEPENDS
    rclcpp
    std_srvs
    sensor_msgs
    cv_bridge
    OpenCV
)

# Find dependencies
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

################ calibration node   ##################
add_executable(calibration src/calibration_node.cpp)
target_include_directories(calibration PUBLIC
  ${OpenCV_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(calibration PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(calibration PUBLIC ${THIS_PACKAGE_INCLUDE_DEPENDS})
target_link_libraries(calibration PUBLIC "${cpp_typesupport_target}")
##################################################


##############  Install targets ##################
# install executables
install(TARGETS calibration
  DESTINATION lib/${PROJECT_NAME})
ament_package()
