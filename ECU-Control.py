import multiprocessing
import time
import can
from piracer.vehicles import PiRacerStandard


current_steering_data = multiprocessing.Value('f', 0.0)
current_throttle_data = multiprocessing.Value('f', 0.0)
steering_lock = multiprocessing.Lock()
throttle_lock = multiprocessing.Lock()

bus = can.interface.Bus(channel='can0', bustype='socketcan')

piracer = PiRacerStandard()
piracer.set_steering_percent(0.0)
piracer.set_throttle_percent(0.0)
time.sleep(0.5)

# -------------------------------------------------- READY SIGN
piracer.set_steering_percent(0.9)
time.sleep(1)
piracer.set_steering_percent(-0.9)
time.sleep(1)
piracer.set_steering_percent(0.0)
# -------------------------------------------------- READY SIGN


def receive_can_data():
    global current_steering_data
    global current_throttle_data

    while True:
        message = bus.recv()
        data = message.data[1] + message.data[2] * 0.01
        if message.data[0] == 1:
            data *= -1

        if message.arbitration_id == 0:
            with steering_lock:
                current_steering_data.value = data
        if message.arbitration_id == 1:
            with throttle_lock:
                current_throttle_data.value = data

        print("receive")


def control():
    global current_steering_data
    global current_throttle_data

    while True:
        with steering_lock:
            piracer.set_steering_percent(current_steering_data.value * -0.9)
        with throttle_lock:
            piracer.set_throttle_percent(current_steering_data.value * 0.75)
        time.sleep(0.5)

        print("control")


receive_process = multiprocessing.Process(target=receive_can_data)
# control_process = multiprocessing.Process(target=control)

receive_process.start()
# control_process.start()

receive_process.join()
# control_process.join()
