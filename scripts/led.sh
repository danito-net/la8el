#!/usr/bin/env bash

# la8el by danito
# (c) 2022 - Danny Ismarianto Ruhiyat

# Astra DB's Build-A-Thon by DataStax and AngelHack


if [ $# -eq 0 ] ; then
   echo "ERROR: No arguments supplied!"
   exit 7
fi

# Initializing the LED on GPIO
function led_init {
             if [ ! -f /sys/class/gpio/gpio$1/direction ] ; then
                echo $1 > /sys/class/gpio/export
             fi
             echo out > /sys/class/gpio/gpio$1/direction
         }

# Turning on the LED
function led_on {
             echo 1 > /sys/class/gpio/gpio$1/value
         }

# turning off the led
function led_off {
             echo 0 > /sys/class/gpio/gpio$1/value
         }

if [ $1 -eq 1 ]; then led_gpio=5 ; fi
if [ $1 -eq 2 ]; then led_gpio=13 ; fi
if [ $1 -eq 3 ]; then led_gpio=19 ; fi
if [ $1 -eq 4 ]; then led_gpio=6 ; fi

echo $led_gpio

led_init $led_gpio

if [ $2 == "on" ] || [ $2 == "ON" ] ; then
   led_on $led_gpio
else
   led_off $led_gpio
fi
