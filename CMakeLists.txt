cmake_minimum_required(VERSION 2.8.3)
project(loulou)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  baxter_core_msgs
  ecn_common
  sensor_msgs
  cv_bridge
  image_transport
)

## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
 CATKIN_DEPENDS baxter_core_msgs ecn_common sensor_msgs cv_bridge image_transport
)

###########
## Build ##
###########

include_directories(
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(loulou_node src/loulou.cpp)


## Specify libraries to link a library or executable target against
target_link_libraries(loulou_node
  ${catkin_LIBRARIES}
)

install(TARGETS loulou_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
