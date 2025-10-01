cmake_minimum_required(VERSION 3.10)
project(redis)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)
find_library(HIREDIS_LIB hiredis)

add_executable(redis_ros2_pubsub 
  src/main.cpp
  src/redis_worker.cpp
  src/ros2_worker.cpp)

target_link_libraries(redis_ros2_pubsub ${HIREDIS_LIB})
ament_target_dependencies(redis_ros2_pubsub rclcpp std_msgs)

install(TARGETS redis_ros2_pubsub
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
