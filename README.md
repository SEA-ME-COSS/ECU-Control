# About

Independent RaspberryPi based ECU for the control of PiRacer

Convention for CAN Frame

steering: `arbitration_id = 0x00`

throttle: `arbitration_id = 0x01`

**data[0]** = sign (0: Positive, 1: Negative)

**data[1]** = data (xx.__)

**data[2]** = data (__.xx)


# Requirements


| **Local**        | [pygame](https://pypi.org/project/pygame/)                                         | Keyboard input for control |
| **Raspberry Pi** | [piracer](https://github.com/twyleg/piracer_py)                                    | Physical PiRacer control   |

- Ubuntu 20.04
- [ROS2 foxy](https://docs.ros.org/en/foxy/Installation/Ubuntu-Install-Debians.html)
- matplotlib & numpy
    
    ```bash
    pip install numpy
    pip install matplotlib
    ```
    
- Eigen
    
    ```bash
    sudo apt update	
    sudo apt install libeigen3-dev
    ```
    
- Ceres
    
    ```bash
    sudo apt update
    sudo apt install libeigen3-dev libgoogle-glog-dev libgflags-dev
    sudo apt install libceres-dev
    ```
    
- ompl
    
    ```bash
    sudo apt install libompl-dev
    ```

# Usage

```shell
./
 ├── simulation_ws/src/
 │   └── sim        # Description and launch file for PiRacer model
 │
 └── teleoperation_ws/src/
     └── teleop     # Remote control example including a controller and a receiver
```

### 3. Run Teleoperation Controller

```bash
# Local
cd teleoperation_ws

source install/local_setup.bash
ros2 run teleop controller
```

When you run the controller, a small pygame window like the following will appear.

<div width="100%" align="center"><img src="/images/controller.png" align="center" width="30%"></div>

Click on this window and press the WASD keys on the keyboard. Observe the movement of both Gazebo PiRacer and physical PiRacer.


# Reference
- [Waveshare PiRacer](https://www.waveshare.com/wiki/PiRacer_AI_Kit)