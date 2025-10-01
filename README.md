# redis_ros2_pubsub
This branch integrates independent communications of ROS and Redis into a single codebase.

## Features
The communication in this branch can be used as an example for future development. Functions have been implemented so that ROS2 communicates with ROS2, and Redis communicates with Redis.

## Prerequisites
 - OS: Ubuntu OS or Window (Author using Ubuntu 22.04 vesion) 
 - ROS2 (Author using humble version)

## Getting Started

```bash
$git -b redis_ros2_dual_pubsub clone https://github.com/leejae0720/redis.git
$cd ..
$colcon build --packages-select redis

$source install/setup.bash
```

## Run
When this package is launched, communication between Redis and ROS2 will take place simultaneously.

```bash
# In terminal, launch
$ros2 launch redis redis_ros2_pubsub.launch.py
```

## Usage
checking subscribe data

```bash
# ros2 subscribe checking & print topic list
$ros2 topic list
$ros2 topic echo test_topic

# redis subscribe checking & print channels list
$redis-cli PUBSUB channels
$redis-cli SUBSCRIBE test_channel
```

------

## Contributor
 - Name: Jaehong Lee (이재홍)
 - Email: leejae0720@gmail.com
