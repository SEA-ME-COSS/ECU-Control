import can
import cantools
import time

# DBC 파일 로딩
db = cantools.database.load_file('example.dbc')

# CAN 네트워크 설정
bus = can.interface.Bus(channel='vcan0', bustype='socketcan')


def process_message(frame):
    message = db.get_message_by_frame_id(frame.arbitration_id)
    if message:
        decoded_signals = message.decode(frame.data)
        print(f"Received Message: {message.name}")
        for signal_name, value in decoded_signals.items():
            signal = message.get_signal_by_name(signal_name)
            print(f"\t{signal_name} = {value} {signal.unit}")


def main():
    last_received = None
    last_received_time = time.time()
    message_timeout = 2  # 2초 타임아웃

    while True:
        msg = bus.recv()
        current_time = time.time()

        if msg:
            message = db.get_message_by_frame_id(msg.arbitration_id)
            if message:
                if (message.name == "SpeedMessage2" or
                        (message.name == "SpeedMessage1" and
                         (last_received != "SpeedMessage2" or current_time - last_received_time >= message_timeout))):
                    process_message(msg)
                    last_received = message.name
                    last_received_time = current_time

        if last_received == "SpeedMessage2" and current_time - last_received_time >= message_timeout:
            last_received = None


if __name__ == "__main__":
    main()
