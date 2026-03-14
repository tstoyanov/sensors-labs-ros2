import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

def generate_launch_description():
    # Declare arguments
    declared_arguments = []
    declared_arguments.append(
        DeclareLaunchArgument(
            "gui",
            default_value="true",
            description="Start RViz2 automatically with this launch file.",
        )
    )

    # Initialize Arguments
    gui = LaunchConfiguration("gui")

    # Example: start a node and pass it a set of parameters
    #
    publisher_node = Node(
        package="lab00",
        executable="publisher",
        parameters=[],
        output="both",
#To run node with gdb in debug mode, uncomment bellow
#        prefix=['xterm -e gdb --args'],
    )

    package_dir = get_package_share_directory('lab00')
    bag_path = os.path.join(package_dir, 'launch', 'sample.bag')

    play_bag = ExecuteProcess(
        cmd=[
            'ros2', 'bag', 'play', bag_path,
            '--loop',                # Restart when finished
            '--rate', '0.5',         # Play at 0.5x speed
            '--clock',               # Publish to /clock (useful for simulation)
        ],
        output='screen'
    )
    
    rviz_file = os.path.join(get_package_share_directory('lab00'), 'launch',
                             'display.rviz')

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        name="rviz2",
        output="log",
        condition=IfCondition(gui),
        arguments=['--display-config', rviz_file]
    )

    nodes = [
        publisher_node,
        play_bag,
        rviz_node
    ]

    return LaunchDescription(declared_arguments + nodes)
