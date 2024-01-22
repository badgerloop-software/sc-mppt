# MPPT Algorithm
Authors: David Eapen, Ethan Yun Ku, Jovita D'souza, Wilson Guo

## Problem
A key issue with solar panels is that the voltage-current curve is non-linear. Thus, to maximize efficiency,
an algorithm is needed to find the voltage corresponding to maximum power output. We have a few things going
for us.

1. The power curve is concave (single maximum like a stretched downward parabola), and is generally always increasing
when moving towards the maximum power point (MPP)
2. Sensors give the instantaneous voltage, current, and power output.
3. Have control over the output voltage via PWM.

## Solution
The maximum power point tracking algorithm (MPPT) has varying implementations with the same premise.

1. Start at some arbitrary charging voltage
2. Based on measurements, shift the voltage by an amount that is predicted to increase power
3. See if it actually increased power.
4. Repeat.

This repo currently implements the Perturb and Observe (PO) MPPT algorithm, with a two modifications. Firstly, the step
size is updated based on change in power. Secondly, the algorithm switches to constant current mode when the battery is 
over a specified charge threshold to avoid overcharging. This implementation is chosen for its simplicity and speed, but 
subject to change based on performance findings.

MPPT PO Algorithm
1. Start at some arbitrary charging voltage
2. First shift output voltage by a predefined step size
3. If power output increases, do another larger magnitude step in the same direction
4. Otherwise if power decreases, do a smaller magnitude step in the opposite direction
5. Repeat 3 and 4 indefinitely

Constant Current Algorithm
Same as MPPT, but replace the feedback variable of power with average battery current (target 0).

## Additional Features
Overvoltage fault reset triggered through CAN with message ID 0x600
Discharging of output capacitors triggered through CAN with message ID 0x601

Some additional resources for MPPT algorithms
https://ww1.microchip.com/downloads/en/appnotes/00001521a.pdf

https://en.wikipedia.org/wiki/Maximum_power_point_tracking


## Usage
All tunables for the repo are contained in [const.h](./include/const.h). Contact firmware subteam for any questions.