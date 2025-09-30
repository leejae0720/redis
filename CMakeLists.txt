cmake_minimum_required(VERSION 3.10)
project(redis_ros2_pubsub)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)
find_library(HIREDIS_LIB hiredis)

add_executable(redis_ros2_pubsub_node 
  src/main.cpp
  src/redis_worker.cpp
  src/ros2_worker.cpp)

target_link_libraries(redis_ros2_pubsub_node ${HIREDIS_LIB})
ament_target_dependencies(redis_ros2_pubsub_node rclcpp std_msgs)

install(TARGETS redis_ros2_pubsub_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
