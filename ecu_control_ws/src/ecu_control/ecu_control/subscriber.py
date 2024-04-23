import rclpy
from rclpy.node import Node

from geometry_msgs.msg import Twist

import time
from piracer.vehicles import PiRacerStandard


class ControlSubscriber(Node):
    def __init__(self):
        super().__init__('control_subscriber_node')
        self.subscription_ = self.create_subscription(Twist, '/control_data', self.callback, 10)
        self.subscription_  # prevent unused variable warning

        self.piracer = PiRacerStandard()
        self.piracer.set_steering_percent(0.0)
        self.piracer.set_throttle_percent(0.0)
        time.sleep(0.5)

        # -------------------------------------------------- READY SIGN
        self.piracer.set_steering_percent(0.9)
        time.sleep(1)
        self.piracer.set_steering_percent(-0.9)
        time.sleep(1)
        self.piracer.set_steering_percent(0.0)
        # -------------------------------------------------- READY SIGN


    def callback(self, control_msg):
        self.piracer.set_steering_percent(control_msg.angular.z * -0.9)
        self.piracer.set_throttle_percent(control_msg.linear.x * 0.75)
        # self.get_logger().info('')


def main(args=None):
    rclpy.init(args=args)

    control_subscriber = ControlSubscriber()

    rclpy.spin(control_subscriber)

    control_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
