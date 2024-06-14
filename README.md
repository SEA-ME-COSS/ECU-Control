# About

[DIAGRAM HERE]

This repository is for the **ECU-Control** part of the [Autonomous-Driving-System](https://github.com/SEA-ME-COSS/Autonomous-Driving-System) project. The ECU-Control is based on an independent RaspberryPi board and operates a physical PiRacer model vehicle. It controls the throttle and steering of the PiRacer using data received via CAN communication from the [ECU-Core](https://github.com/SEA-ME-COSS/ECU-Core).

# Requirements

- **Ubuntu 20.04**

    Install Ubuntu 20.04 for RaspberryPi using RaspberryPi OS Imager.

- **CAN HAT setup**

    Follow the instruction of [2-CH CAN FD HAT setup](https://www.waveshare.com/wiki/2-CH_CAN_FD_HAT) and enable **Single SPI Mode**

- **Python packages**

    ```bash
    pip install piracer-py
    pip install python-can
    pip install pygame
    ```

# Usage

```bash
# Execute on the ECU-Control
sh can_setup.sh
python3 src/control.py
```

(Optional) You can check the CAN communication using test controller.

```bash
# Execute on the ECU-Core
sh can_setup.sh
python3 src/test_controller.py
```

When you run the controller, a small pygame window like the following will appear. Click on this window and press the WASD keys on the keyboard to operate PiRacer.

<div width="100%" align="center"><img src="/images/controller.png" align="center" width="30%"></div>

# Note

Context of CAN communication

| Message            | Purpose                | Arbitration ID |
|--------------------|------------------------|----------------|
| **steering**       | **Control**            | **0x00**       |
| **throttle**       | **Control**            | **0x01**       |
| x position         | GPS                    | 0x02           |
| y position         | GPS                    | 0x03           |
| orientation        | GPS                    | 0x04           |
| headunit start     | Autonomous driving     | 0x05           |

# Reference
- [Waveshare PiRacer](https://www.waveshare.com/wiki/PiRacer_AI_Kit)