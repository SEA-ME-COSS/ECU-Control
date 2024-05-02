import can
import cantools
import time

# DBC 파일 로딩
db = cantools.database.load_file('example.dbc')

# CAN 네트워크 설정
bus = can.interface.Bus(channel='vcan0', bustype='socketcan')

def send_speed_message(speed):
    # DBC에서 메시지 찾기
    message = db.get_message_by_name('SpeedMessage')

    # 데이터 인코딩
    data = message.encode({'Speed': speed})

    # CAN 메시지 생성 및 전송
    msg = can.Message(arbitration_id=message.frame_id, data=data, is_extended_id=False)
    bus.send(msg)
    print(f"Sent {speed} on CAN ID {hex(message.frame_id)}")

if __name__ == "__main__":
    while True:
        send_speed_message(10)  # Speed 값을 10으로 설정
        time.sleep(1)  # 1초마다 메시지 전송

