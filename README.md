# ECU-Control

Independent RaspberryPi based ECU for the control of PiRacer.

## Convention for CAN Frame

steering: `arbitration_id = 0x00`

throttle: `arbitration_id = 0x01`

**data[0]** = sign (0: Positive, 1: Negative)

**data[1]** = data (xx.__)

**data[2]** = data (__.xx)