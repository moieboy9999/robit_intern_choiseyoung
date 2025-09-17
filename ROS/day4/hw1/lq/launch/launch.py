from launch import LaunchDescription
from launch.actions import Shutdown
from launch_ros.actions import LifecycleNode
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        LifecycleNode(package='lifecycle', executable='lifecycle_talker',
                      name='lc_talker', namespace='', output='screen'),
        Node(package='lifecycle', executable='lifecycle_listener', output='screen'),
        Node(package='lifecycle', executable='lifecycle_service_client', output='screen',
             on_exit=Shutdown()),
    ])