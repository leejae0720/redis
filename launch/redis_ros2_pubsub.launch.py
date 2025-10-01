from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='redis',
            executable='redis_ros2_pubsub',
            name='redis_ros2_pubsub',
            output='screen'
        )
    ])
