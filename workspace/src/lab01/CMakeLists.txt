cmake_minimum_required(VERSION 3.8)
project(lab01)

#set the build type to Release (optimized) with debug information
set(CMAKE_BUILD_TYPE RelWithDebInfo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# set a new variable that holds all the build dependencies
# NOTE: this should include all packages from package.xml that are either depends or build depends for the nodes and libraries we are building
set(THIS_PACKAGE_INCLUDE_DEPENDS
    rclcpp
    std_msgs
    sensor_msgs
)

# Find dependencies
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

########## For message generation ################
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Encoder.msg"
  DEPENDENCIES builtin_interfaces std_msgs
)
ament_export_dependencies(rosidl_default_runtime)
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
##################################################

################ encoder node   ##################
add_executable(encoder src/encoder_node.cpp)
target_include_directories(encoder PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(encoder PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(encoder PUBLIC ${THIS_PACKAGE_INCLUDE_DEPENDS})
target_link_libraries(encoder PUBLIC "${cpp_typesupport_target}")
##################################################

################ odometry node   ##################
add_executable(odometry src/odometry_node.cpp)
target_include_directories(odometry PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(odometry PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(odometry PUBLIC ${THIS_PACKAGE_INCLUDE_DEPENDS})
##################################################

##############  Install targets ##################
# install executables
install(TARGETS encoder odometry
  DESTINATION lib/${PROJECT_NAME})

##################################################

#bellow makes sure packages that depend on us know what to link and include
ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS})
ament_package()
