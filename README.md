# Daikin anti defrost
This project aims to reduce the defrost frequency of Daikin ATXD 25/35 units running in heat mode.

# The issue
ATXD begins with the EEV set at a default position. During the next 40 mins the valve closes at a constant speed until the hotgas temperature begins to rise. The unit tends to close the EEV too far, resulting in a strong superheating in the evaporator, which casues the temperature at the evaporator sensor to drop. When the units senses the rise in hotgas temperature, it starts to open the EEV again, which leads to reduced superheat and a falling hotgas temperature. The unit reacts quite slowly and begins an oscillative feedback loop.
The drop of the tempearure in the evaporator is often misinterpreted as ice on the evaporator, causing the unit to start a defrost cycle.

<img width="1200" height="1000" alt="ATXD25_hotgas" src="https://github.com/user-attachments/assets/5fa049bc-2dad-4321-849b-8fbef39972be" />

# Proposed solution
A microcontroller is mounted in the outdoor unit. It is powered by the 5V provided by the main PCB. One IO of the microcontoller is connected in parallel to the NTC sensor for the evaporator temperatur. By outputting a PWM signal to the IO, the temperature detected by the Daikin can be modified to any value. An RC circuit on the PWM output filters the signal to allow a reliable voltage measurement of the NTC voltage.

Every now and then the IO can be changed to high impedance input mode, which makes the readout of the real evaporator temperature possible. The Daikin unit only starts a defrost cycle if the temperature is low for a longer time (about 1 min), so this will not interfere with the operation.

If a defrost cycle starts, the temperature of the evaporator will rise. When the temperature rises > 15°C the defrost cycle typically ends.

The code of the microcontroller does following:
1. Set the IO to input. Measure the real temperature of the evaporator.
2. Store the temperature in a ring buffer every 10 seconds. The buffer holds the last 120 measurements (20 min).
3. Evaluate the highest temperature in the buffer.
4. Change the IO mode to output.
5. Adjust the PWM signal so the voltage across the NTC equals the max temperature in the buffer. This is done by incrementing of decrementing the PWM value in a continuous loop.
6. If the temperature rises > 0°C, the PWM output is disabled to allow the unit to measure the actual temperature during a defrost cycle.

<img width="905" height="688" alt="Arduino_ATXD_1" src="https://github.com/user-attachments/assets/c1507dd8-eeb4-49fe-8a34-e4e510db97d8" />
