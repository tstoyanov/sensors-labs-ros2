import rclpy
from rclpy.node import Node

from sensor_msgs.msg import LaserScan
from std_srvs.srv import Trigger

import numpy as np
import matplotlib.pyplot as plt

from sensor_labs_msgs.srv import StartNewSeries

from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy


class LaserScanVisualizer(Node):

    def __init__(self):
        super().__init__('laser_scan_visualizer')

        qos_profile = QoSProfile(
            reliability=ReliabilityPolicy.BEST_EFFORT,
            history=HistoryPolicy.KEEP_LAST,
            depth=10
        )

        # Subscriber
        self.subscription = self.create_subscription(
            LaserScan,
            '/scan',
            self.scan_callback,
            qos_profile
        )

        # Service clients
        self.client1 = self.create_service(StartNewSeries, 'start_series', self.start_series_callback)
        self.client2 = self.create_service(Trigger, 'plot_variances', self.plot_callback)

        # Matplotlib setup
        plt.ion()
        self.fig, self.ax = plt.subplots()
        self.scatter, = self.ax.plot([], [], 'b.', markersize=2)

        #set figure properties
        self.ax.set_title("Laser Scan")
        self.ax.set_xlabel("X (m)")
        self.ax.set_ylabel("Y (m)")
        self.ax.set_aspect('equal')
        self.ax.set_xlim(-5, 5)
        self.ax.set_ylim(-5, 5)

        self.get_logger().info("Laser Scan Visualizer Node Started")

    def scan_callback(self, msg: LaserScan):
        # TODO: modify plotting here to show the points corresponding to 
        # the front 3 rays of the laser in red.

        # Convert polar coordinates to Cartesian
        angles = np.arange(msg.angle_min, msg.angle_max, msg.angle_increment)
        ranges = np.array(msg.ranges)

        # Filter invalid values
        valid = np.isfinite(ranges)
        ranges = ranges[valid]
        angles = angles[valid]

        x = ranges * np.cos(angles)
        y = ranges * np.sin(angles)

        # Update plot
        self.scatter.set_data(x, y)

        self.fig.canvas.draw()
        self.fig.canvas.flush_events()
        
    def scan_callback1(self, msg: LaserScan):
        #TODO: implement logic here for recording measurements
        # record to current series, switch to record to next series once callback of service is called
        # compute errors to ground-truth distance
    
    def scan_callback1(self, msg: LaserScan):
        #TODO: implement occupancy mapping logic here 
        #hint: you might want to define your own helper function for raytracing.

    def start_series_callback(self, request, response):
        self.get_logger().info("Starting new series for ",request.series_name)
        #TODO here start a new series of data measurements, initialize variables and cleanup previous series
        return response

    def plot_callback(self, request, response):
        self.get_logger().info("Plotting all data")
        # TODO here generate plot of mean erro and and variance for each series
        return response

def main(args=None):
    rclpy.init(args=args)
    node = LaserScanVisualizer()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    node.destroy_node()
    rclpy.shutdown()
    plt.close()


if __name__ == '__main__':
    main()
