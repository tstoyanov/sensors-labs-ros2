cmake_minimum_required(VERSION 3.8)
project(lab00)

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
)

# Find dependencies
foreach(Dependency IN ITEMS ${THIS_PACKAGE_INCLUDE_DEPENDS})
  find_package(${Dependency} REQUIRED)
endforeach()

########## For message generation ################
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/SampleMessage.msg"
  DEPENDENCIES builtin_interfaces std_msgs
)
ament_export_dependencies(rosidl_default_runtime)
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
##################################################

################ example library ##################
# add a library. This gets built and can be linked to inside and outside the package
add_library(example_library SHARED
  src/subscriber.cpp
)

# set the include directories for building the library
target_include_directories(example_library PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

# set compile flags this way
target_compile_features(example_library PUBLIC c_std_99 cxx_std_17)

# let colcon know what this depends on for linking
ament_target_dependencies(example_library PUBLIC ${THIS_PACKAGE_INCLUDE_DEPENDS})
##################################################

################ example node   ##################
add_executable(publisher src/publisher_node.cpp)
target_include_directories(publisher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(publisher PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(publisher PUBLIC ${THIS_PACKAGE_INCLUDE_DEPENDS})
#needed to use the custom message within this package
target_link_libraries(publisher PUBLIC "${cpp_typesupport_target}")
##################################################


###### example node that links to library ########
add_executable(subscriber src/subscriber_node.cpp)
target_include_directories(subscriber PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(subscriber PUBLIC c_std_99 cxx_std_17)
target_link_libraries(subscriber PUBLIC example_library)
ament_target_dependencies(subscriber PUBLIC ${THIS_PACKAGE_INCLUDE_DEPENDS})
##################################################

##############  Install targets ##################
install(TARGETS example_library
  EXPORT export_lab00
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# install executables
install(TARGETS publisher subscriber
  DESTINATION lib/${PROJECT_NAME})

# install a folder with configurations or launch
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

##################################################

#bellow makes sure packages that depend on us know what to link and include
ament_export_targets(export_lab00 HAS_LIBRARY_TARGET)
ament_export_dependencies(${THIS_PACKAGE_INCLUDE_DEPENDS})

ament_package()
