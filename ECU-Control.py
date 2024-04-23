import multiprocessing
import time
import can
from piracer.vehicles import PiRacerStandard


# current_steering_data = multiprocessing.Value('f', 0.0)
# current_throttle_data = multiprocessing.Value('f', 0.0)
# steering_lock = multiprocessing.Lock()
# throttle_lock = multiprocessing.Lock()

bus = can.interface.Bus(channel='can0', bustype='socketcan')


def receive_can_data():
    while True:
        message = bus.recv()
        print("Received message - ID:", message.arbitration_id, "Data:", message.data)


# 값을 저장하는 함수
# def store_value():
#     global shared_variable
#     while True:
#         with lock:
#             shared_variable.value += 1
#         time.sleep(1)  # 1초마다 값을 증가시킴


# def print_value():
#     global shared_variable
#     while True:
#         with lock:
#             print("Current value:", shared_variable.value)
#         time.sleep(3)  # 3초마다 현재 값을 출력

# 프로세스 생성
store_process = multiprocessing.Process(target=receive_can_data)
# print_process = multiprocessing.Process(target=print_value)

# 프로세스 시작
store_process.start()
# print_process.start()

# 메인 프로세스 종료 대기
store_process.join()
# print_process.join()


# ==========================


# piracer = PiRacerStandard()
# piracer.set_steering_percent(current_steering0.0)
# piracer.set_throttle_percent(0.0)
# time.sleep(0.5)

# # Ready Sign
# piracer.set_steering_percent(0.9)
# time.sleep(1)
# piracer.set_steering_percent(-0.9)
# time.sleep(1)
# piracer.set_steering_percent(0.0)

# current_steering = 0.0
# current_throttle = 0.0


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
