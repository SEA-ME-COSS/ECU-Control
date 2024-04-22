# ECU-Control

Independent RaspberryPi based ECU for the control of PiRacer.

## Convention for CAN Frame

steering: `arbitration_id = 0`

throttle: `arbitration_id = 1`

**data[0]** = sign (0: Positive, 1: Negative)

**data[1]** = data (xx.__)

**data[2]** = data (__.xx)

## Note

Configure static ip address

```shell
sudo nano /etc/netplan/50-cloud-init.yaml
```

```yaml
network:
    version: 2
    wifis:
        renderer: networkd
        wlan0:
            access-points:
                TP-Link_77CE:
                    password: ********
            dhcp4: false
            optional: true
            addresses: [192.168.1.114/24]
            gateway4: 192.168.1.1
            nameservers:
                addresses: [8.8.8.8, 8.8.4.4]
```

Auto login

```shell
sudo systemctl edit getty@tty1.service
```

```conf
[Service]
ExecStart=
ExecStart=-/sbin/agetty --noissue --autologin team5 %I $TERM
Type=simple
```