import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Twist

import can


class ControlPublisher(Node):
    def __init__(self):
        super().__init__('control_publisher_node')
        self.publisher_ = self.create_publisher(Twist, '/control_data', 10)
        self.timer_ = self.create_timer(0.001, self.callback)  # [s]

        self.control_msg = Twist()

        self.control_msg.angular.x = 0.0
        self.control_msg.angular.y = 0.0
        self.control_msg.angular.z = 0.0
        self.control_msg.linear.x = 0.0
        self.control_msg.linear.y = 0.0
        self.control_msg.linear.z = 0.0

        self.publisher_.publish(self.control_msg)

        self.bus = can.interface.Bus(channel='can0', bustype='socketcan')


    def callback(self):
        self.publisher_.publish(self.control_msg)
        # self.get_logger().info('')

        self.control_msg.angular.z = 0.0
        self.control_msg.linear.x = 0.0


def main(args=None):
    rclpy.init(args=args)

    control_publisher = ControlPublisher()

    while True:
        message = control_publisher.bus.recv()
        data = message.data[1] + message.data[2] * 0.01
        if message.data[0] == 1:
            data *= -1

        if message.arbitration_id == 0:
            control_publisher.control_msg.angular.z = data
        if message.arbitration_id == 1:
            control_publisher.control_msg.linear.x = data

        rclpy.spin_once(control_publisher)

    control_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
