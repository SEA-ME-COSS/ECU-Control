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


def print_value():
    global current_steering_data
    global current_throttle_data

    while True:
        with steering_lock:
            print("steering:", current_steering_data.value)
        with throttle_lock:
            print("throttle:", current_throttle_data.value)
        time.sleep(3)


store_process = multiprocessing.Process(target=receive_can_data)
print_process = multiprocessing.Process(target=print_value)

store_process.start()
print_process.start()

store_process.join()
print_process.join()


# ==========================





# def process_can_message(msg):
#     global current_steering
#     global current_throttle

#     data = msg.data[1] + msg.data[2] * 0.01
#     if msg.data[0] == 1:
#         data *= -1

#     if msg.arbitration_id == 0:
#         current_steering = round(data * -0.9, 2)
#         print("steering:", current_steering)

#     if msg.arbitration_id == 1:
#         current_throttle = round(data * 0.75, 2)
#         print("throttle:", current_throttle)


# bus = can.interface.Bus(channel='can0', bustype='socketcan')
# notifier = can.Notifier(bus, [process_can_message])

# while True:
#     piracer.set_steering_percent(current_steering)
#     piracer.set_throttle_percent(current_throttle)

#     time.sleep(0.5)
