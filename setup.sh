#!/bin/bash

ip link set can0 up type can bitrate 500000
ifconfig can0 txqueuelen 65536

python3 ECU-Control.py
