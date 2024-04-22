import can
import time
from piracer.vehicles import PiRacerStandard


piracer = PiRacerStandard()
piracer.set_steering_percent(0.0)
piracer.set_throttle_percent(0.0)

current_steering = 0.0
current_throttle = 0.0


def process_can_message(msg):
    global current_steering
    global current_throttle

    data = msg.data[1] + msg.data[2] * 0.01
    if msg.data[0] == 1:
        data *= -1

    if msg.arbitration_id == 0:
        current_steering = round(data * -0.9, 2)
        print("steering:", current_steering)

    if msg.arbitration_id == 1:
        current_throttle = round(data * 0.75, 2)
        print("throttle:", current_throttle)


bus = can.interface.Bus(channel='can0', bustype='socketcan')
notifier = can.Notifier(bus, [process_can_message])

while True:
    piracer.set_steering_percent(current_steering)
    piracer.set_throttle_percent(current_throttle)

    time.sleep(0.5)
