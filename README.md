# Battery Level Daemon v1.5

**_A daemon, that notifies the user about battery charge based on threshholds._**
It has a very very negligible footprint on system resources, on my Ryzen 5, it's CPU usage is about 0.0% always, and 4MB ram memory

## Purpose

----

Notify the user, when the laptop battery charge crosses particular MIN and MAX thresholds.

## Using

----

Compile,
```sh
make    # to compile
# or can compile AND run once
make run    # compile AND, run once
```
Then add the ./alertd executable to auto start at boot time


## Explanation

----

By default, the thresholds are 21 and 60. That is, if your laptop battery goes below 21%, it will notify the user to connect the charger/adapter.
And, if the battery is charged till 60%, notify the user to unplug the device.

Since lower charge cycles can sometimes drastically increase the battery life a bit more, in case of Li Polymer batteries currently in wide use.



## Problem I tried to solve

----

Many companies, like ASUS provide control under the Windows Operating System, to control thresholds of how much the battery should charge at max, similar is available from tlp, on Lenovo ThinkPad.

This program aims to atleast let the cautious users know when the thresholds are reached, though it can't control the charger current in any way, that upon the user to act.


## Technical Perspective - Working

----

1. It first creates itself into a daemon process, more of which can be found with simple searches online, or StackOverFlow
2. Then in an infinite loop, read the file /sys/class/power_supply/BAT0, AFTER PARTICULAR INTERVAL OF TIME  (Each 2.5 seconds WHEN, MIN_Threshhold < battery_charge < MAX_Threshhold )
3. When either below MIN, or above MAX, particular functions are called, and in these cases,
    1. If lower than 20%,
        * CHARGING, then check again after 20 minutes
        * NOT CHARGING, then keep notifying each 1.5 minutes
    2. If higher than 80%,
        * NOT CHARGING, then check again after 20 minutes
        * CHARGING, then keep notifying each 1.5 minutes

    > The 20% and 80% are the defaults, can easily be changed

* The notifications are of normal priority, and short enough to not bug you, if you want to ignore


> Version 1.5 is the first version :smile:
