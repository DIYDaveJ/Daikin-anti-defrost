# Daikin anti defrost
This project aims to reduce the defrost frequency of Daikin ATXD 25/35 units running in heat mode.

# The problem
ATXD begins each heat cycle with the EEV set to a default position. During the next 40 mins the valve closes at a constant speed until the hotgas temperature begins to rise. The unit tends to close the EEV too far, resulting in a strong superheat in the evaporator, accompanied by a temperature drop at the beginning of the evaporator when the pressure drops. When the units senses the rise in hotgas temperature, it starts to open the EEV again, which leads to reduced superheat and a falling hotgas temperature. The unit reacts quite slowly and begins an oscillative feedback loop.
The drop of the tempearure in the evaporator is often misinterpreted as ice on the evaporator, causing the unit to start a defrost cycle.

<img width="1200" height="1000" alt="ATXD25_hotgas" src="https://github.com/user-attachments/assets/5fa049bc-2dad-4321-849b-8fbef39972be" />

# Proposed solution
A microcontroller is mounted in the outdoor unit. It is powered by the 5V provided by the main PCB. One IO of the microcontoller is connected in parallel to the NTC sensor for the evaporator temperatur. By outputting a PWM signal to the IO, the temperature detected by the Daikin can be modified to any value. An RC circuit on the PWM output filters the signal to allow a reliable voltage measurement of the NTC voltage.

Every now and then the IO can be changed to high impedance input mode, which makes the readout of the real evaporator temperature possible. The Daikin unit only starts a defrost cycle if the temperature is low for a longer time (about 1 min), so this will not interfere with the operation.

If a defrost cycle starts, the temperature of the evaporator will rise. When the temperature rises > 15°C the defrost cycle typically ends.

The code of the microcontroller does following:
1. Set the IO to input. After 100ms, measure the real temperature at the evaporator.
2. Store the temperature in a ring buffer every 5 seconds. The buffer holds the last 256 measurements (21 min).
3. Evaluate the highest temperature in the buffer.
4. Change the IO mode to output and set a voltage (by PWM).
5. Adjust the PWM value so the voltage at the NTC equals the max temperature in the buffer. This is done by incrementing of decrementing the PWM value in a continuous loop.
6. If the max temperature is > 0°C, the PWM output is disabled to allow the unit to measure the actual temperature during a defrost cycle.

<img width="879" height="568" alt="Arduino_ATXD_1" src="https://github.com/user-attachments/assets/f12dae58-bad3-437b-b37b-6f6371956625" />

# Build

First a warning! The power must be cut before the board is installed. The board must under no circumstances be connected with the chassis of the heat pump. This will cause a short circuit, which can destroy the heat pump. Make sure to isolate the board properly before turning on power.

A Pro Micro is soldered to a bread board. The connectors are of JST type. Note how one of the sockets has been modified to make the Daikin sensor cable fit.
![front](https://github.com/user-attachments/assets/19ab016d-0047-402a-9136-e4509f684671)

The two Jst connectors are wired in parallel.
![back](https://github.com/user-attachments/assets/877c7e36-30af-42f2-bd04-60c6951b3156)
Flatten the JST connector at one side with a knife to make it fit in the socket on the heat pump main board.
![cable_mainPCB](https://github.com/user-attachments/assets/33ad99c5-a2c7-4720-9800-d761d8b151ee)
Remove the sensor cable from the main PCB. Attach the cable from the microcontroller
![connect_main](https://github.com/user-attachments/assets/3f92abfe-133a-43bd-bb03-18cd400593fb)
The black cable is attached to the ground on the Daikin PCB. (The two additional pins are not needed)
![connect_1](https://github.com/user-attachments/assets/5a1b1f58-e5d6-406e-b2ee-bb8326617f3e)

The black ground cable must be connected to one of the wires at the upper right corner. I suppose a clamp could be used instead. This is not system ground! No parts of the microcontroller must be connected to the chassis of the heat pump.
![Ground](https://github.com/user-attachments/assets/57aaacb9-752b-4b67-9981-b1b0d326ef81)

Make sure to isolate the board so it doesn't touch the chassis. Use isolation tape or use an enclosure.
