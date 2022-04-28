#!/usr/bin/env bash

# la8el by danito
# (c) 2022 - Danny Ismarianto Ruhiyat

# Astra DB's Build-A-Thon by DataStax and AngelHack

# initializing the buzzer on GPIO22 (BCM 6)
function buzzer_init {
             if [ ! -f /sys/class/gpio/gpio6/direction ] ; then
                sudo echo 6 > /sys/class/gpio/export
             fi
             sudo echo out > /sys/class/gpio/gpio6/direction
         }

# turning on the buzzer
function buzzer_on {
             sudo echo 1 > /sys/class/gpio/gpio6/value
         }

# turning off the buzzer
function buzzer_off {
             sudo echo 0 > /sys/class/gpio/gpio6/value
         }

buzzer_init

if [ $1 == "on" ] || [ $1 == "ON" ] ; then
   buzzer_on
else
   buzzer_off
fi
