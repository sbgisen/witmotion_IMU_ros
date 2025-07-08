import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    # Launch 引数の宣言
    namespace_arg = DeclareLaunchArgument(
        'namespace',
        default_value='',
        description='Namespace for the node'
    )

    config = os.path.join(
        get_package_share_directory('witmotion_ros'),
        'config',
        'config.yml'
    )

    node = Node(
        package='witmotion_ros',
        executable='witmotion_ros_node',
        namespace=LaunchConfiguration('namespace'),
        parameters=[config]
    )

    ld = LaunchDescription()
    ld.add_action(namespace_arg)
    ld.add_action(node)
    return ld
