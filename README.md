# la8el
The la8el is a physical & digital labelling system for global supply chain industry solution; this repository is to complete the requirements for Astra DB's Build-A-Thon 2022 hackathon submission.

The la8el's device prototype are two types:
1. la8el anchor device, using ESP32 as a controlling unit
2. la8el tag device, using Raspberry Pi Zero 2 W as a computing / processing unit

Please follow these steps to prepare the working label tag device prototype:

#### 1.1. Install the latest "Raspberry Pi Imager" application ####

* [Windows](https://downloads.raspberrypi.org/imager/imager_latest.exe): `https://downloads.raspberrypi.org/imager/imager_latest.exe`
* [Linux](https://downloads.raspberrypi.org/imager/imager_latest_amd64.deb): `https://downloads.raspberrypi.org/imager/imager_latest_amd64.deb`
* [MacOS](https://downloads.raspberrypi.org/imager/imager_latest.dmg): `https://downloads.raspberrypi.org/imager/imager_latest.dmg`
* Raspberry Pi OS, do this command on 'terminal': `sudo apt install rpi-imager`


#### 1.2. Download the latest operating system for Raspberry Pi (Raspbian OS)  ####

* [Direct download](https://downloads.raspberrypi.org/raspios_oldstable_armhf/images/raspios_oldstable_armhf-2022-04-07/2022-04-04-raspios-buster-armhf.img.xz): `https://downloads.raspberrypi.org/raspios_oldstable_armhf/images/raspios_oldstable_armhf-2022-04-07/2022-04-04-raspios-buster-armhf.img.xz`
* [Torrent download](https://downloads.raspberrypi.org/raspios_oldstable_armhf/images/raspios_oldstable_armhf-2022-04-07/2022-04-04-raspios-buster-armhf.img.xz.torrent): `https://downloads.raspberrypi.org/raspios_oldstable_armhf/images/raspios_oldstable_armhf-2022-04-07/2022-04-04-raspios-buster-armhf.img.xz.torrent`


#### 1.3. Deploy ('burn') the latest version of Raspbian OS into a 32GB micro SD using Raspberry Pi Imager application ####

* Extract the `2022-04-04-raspios-buster-armhf.img.xz` file
* Use 32GB Micro SD Class-10 (better using a **High Endurance** type; usually used for CCTV / live camera purposes)
* Run the Raspberry Pi Imager application as administrator
* Operating System > CHOOSE OS > Use custom > select `2022-04-04-raspios-buster-armhf.img` file
* SD Card > CHOOSE SD CARD > select SDHC SCSI Disk Device - 31.9 GB
* Click WRITE and please wait until writing and verification process finish


#### 1.4. Put the micro SD into micro SD slot in Raspberry Pi and turn it on ####

* Use Raspberry Pi Zero 2 W
* Use 5V 3A power adapter and plug in to the Micro USB port
* Plug in the HDMI cable into Raspberry Pi's Mini HDMI port to connect to a display monitor (please use HDMI-to-MicroHDMI converter)
* Plug in the USB Keyboard into Raspberry Pi's USB port
* Plug in the power adapter to AC electrical socket


#### 1.5. Configure the Raspbian OS ####

* Choose the appropriate configuration for: setting country, location / region, language, keyboard layout, user password, etc
* Connect to wireless network using WiFi to available Access Point
* Activate `SSH` and `VNC` for easy access (so you can copy paste from other computer); select from start menu (Raspberry pi's logo at upper left screen) > Preferences > Raspberry Pi Configuration > Interfaces tab > enable **SSH** and **VNC**


#### 1.6. Updating the Raspian OS ####

After connected to the internet using WiFi, using the "terminal" application give the following commands:

    sudo apt update
    sudo apt upgrade
    sudo apt dist-upgrade
    sudo apt autoremove


#### 1.7. Install "xscreensaver" application and disable the blank / sleep mode display ####

Using the "terminal" application give the following command:

    sudo apt install xscreensaver

When installation finished, please disable the screensaver also disable blank / sleep-mode display:

Menu > Preferences > Screensaver > Display Modes tab > Mode > Disable Screen Save


#### 1.8. Install RasPi Blinka from AdaFruit (for OLED 240x240) ####

Using the "terminal" application give the following command:

    sudo pip3 install --upgrade setuptools
    sudo pip3 install --upgrade adafruit-python-shell
    wget https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/raspi-blinka.py
    sudo python3 raspi-blinka.py
    sudo pip3 install adafruit-circuitpython-rgb-display
    
