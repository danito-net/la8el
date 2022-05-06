# la8el - The Phygital Labeling System
The la8el (wrote with "8" / eight number at the middle; pronounced as "label with 8" or just simply as "label" as usual) is a physical & digital labeling system for global supply chain industry solution; this repository is to complete the requirements for [Astra DB's Build-A-Thon](https://buildathonhack.com/) Round-3 hackathon submission.

The [Apache Cassandra](https://cassandra.apache.org/) is the ideal choice as a backend database for apps that need scale. Aside from offering high performance and reliability, Cassandra’s architecture makes it possible to scale horizontally to handle very high loads.  Since **Astra DB** simplifies cloud-native Cassandra application development, when you combine the IoT / Internet-of-Things system with Astra DB you get all the benefits of Cassandra with the flexibility of a multi-cloud, serverless database-as-a-service (DBaaS).

We need to first create an Astra DB instance and gather all your credentials. If you haven’t already, you can sign up for a [free Astra DB account](https://astra.datastax.com/register/U2FsdGVkX19zeWDUisJfqu6pgafepR9DQlGoSfelRapdhWXHN4tOhOh4TWeg6yvtkItADFtdcZwJLBpicGp2vwe1Q2u3A4le1Q2u3A4l) that will give you 80 GB monthly to work with (credit card information is not needed).

There are three types of la8el's device prototype:
1. la8el JAB-Code reader on conveyor belt, using Raspberry Pi CM4 8GB
2. la8el anchor device, using ESP32 microcontroller as a controlling unit
3. la8el tag device, using Raspberry Pi Zero 2 W as a computing / processing unit and as a digital label display

Please follow these steps to prepare the working la8el JAB-Code reader and label tag device prototype:

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
    
#### 1.9. Install basic tools for software development ####

Using the "terminal" application give the following commands:

    sudo apt install libssl-dev
    sudo apt install autoconf
    sudo apt install libtool
    sudo apt install uuid-dev
    sudo apt install ninja-build
    sudo apt install lrzip 

or in just one line command:

    sudo apt install libssl-dev autoconf libtool uuid-dev ninja-build lrzip

and then do a `ldconfig` command:

    sudo ldconfig


#### 1.10. Compile and install the latest "CMake" tools ####

Using the "terminal" application give the following commands:

    cd ~
    git clone https://github.com/Kitware/CMake.git
    cd CMake
    ./bootstrap
    make
    sudo make install
    sudo ldconfig


#### 1.11. Compile and install the latest ImageMagick ####

ImageMagick is a tools for image processing and converting purposes. Using the "terminal" application give the following commands:

    cd ~
    git clone https://github.com/ImageMagick/ImageMagick.git
    cd ImageMagick
    ./configure
    make
    sudo make install
    sudo ldconfig


#### 1.12. Compile and install the latest "zlib" library ####

Using the "terminal" application give the following commands:

    cd ~
    wget https://zlib.net/zlib-1.2.12.tar.gz
    tar xvzf zlib-1.2.12.tar.gz
    cd zlib-1.2.11
    ./configure
    cmake .
    make
    sudo make install
    sudo ldconfig


#### 1.13. Compile and install the latest "libpng" library  ####

Using the "terminal" application give the following commands:

    cd ~
    wget https://nchc.dl.sourceforge.net/project/libpng/libpng16/1.6.37/libpng-1.6.37.tar.gz
    tar xvzf libpng-1.6.37.tar.gz
    cd libpng-1.6.37
    ./configure
    make
    sudo make install
    sudo ldconfig

#### 1.14. Compile and install the latest "tiff" library  ####

Using the "terminal" application give the following commands:

    cd ~
    wget http://download.osgeo.org/libtiff/tiff-4.3.0.tar.gz
    tar xvzf tiff-4.3.0.tar.gz
    cd tiff-4.3.0
    ./configure
    make
    sudo make install


#### 1.15. Compile and install the latest "jpeg-9d" library  ####

Using the "terminal" application give the following commands:

    cd ~
    wget https://www.ijg.org/files/jpegsrc.v9d.tar.gz
    tar xvzf jpegsrc.v9d.tar.gz
    cd jpeg-9d/
    ./configure
    make
    sudo make install
    sudo ldconfig


#### 1.16. Compile and install the latest "jbigkit" library  ####

Using the "terminal" application give the following commands:

    cd ~
    wget https://www.cl.cam.ac.uk/~mgk25/jbigkit/download/jbigkit-2.1.tar.gz
    tar xvzf jbigkit-2.1.tar.gz
    cd jbigkit-2.1
    make


#### 1.17. Install the "lzma-dev" and "liblzma-dev" libraries ####

Using the "terminal" application give the following commands:

    cd ~
    sudo apt install lzma-dev liblzma-dev


#### 1.18. Compile and install "jabcode" from my repository ####

Using the "terminal" application give the following commands:

    cd ~
    git clone https://github.com/danito-net/jabcode.git
    cd jabcode/src/jabcode/lib
    mkdir BAK
    mv *.a BAK/
    sudo cp /usr/local/lib/libz.a .
    sudo cp /usr/local/lib/libpng16.a .
    sudo cp /usr/local/lib/libtiff.a .
    sudo cp /usr/local/lib/libjpeg.a .
    sudo cp /usr/lib/arm-linux-gnueabihf/liblzma.a . 
    cp ~/jbigkit-2.1/libjbig/libjbig.a .
    sudo chown -R pi.pi ~/*
    cd ..
    make
    cd ../jabcodeWriter
    make
    cp bin/jabcodeWriter ~/
    cd ../jabcodeReader
    make
    cp bin/jabcodeReader ~/
    cd ~


#### 1.19. Testing the `jabcodeWriter` and `jabcodeReader` ####

To create a jabcode image do the following command:

    ~/jabcode/src/jabcodeWriter/bin/jabcodeWriter --input "Astra DB's Build-A-Thon by DataStax and AngelHack" --output message.png

To read a jabcode image do the following command:

    ~/jabcode/src/jabcodeReader/bin/jabcodeReader message.png

Because we already copying the executable binary file to user "pi" home directory, simply do from user pi home directory (using command `cd ~`) the following commands:

To create a jabcode image do the following command:

    ./jabcodeWriter --input "Astra DB's Build-A-Thon by DataStax and AngelHack" --output message.png

To read a jabcode image do the following command:

    ./jabcodeReader message.png

Android smartphone can install JabCode application from my GitHub repository : [JabCode Reader for Android ](https://github.com/danito-net/jabcode/blob/master/android_reader/JabCodeApp.apk)

