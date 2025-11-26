# Daikin anti defrost
This project aims to reduce the defrost frequency of Daikin ATXD 25/35 units running in heat mode.

# The issue
ATXD begins with the EEV set at a default position. During the next 40 mins the valve closes at a constant speed until the hotgas temperature begins to rise. The unit tends to close the EEV too far, resulting in a strong superheating in the evaporator, which casues the temperature at the evaporator sensor to drop. When the units senses the rise in hotgas temperature, it starts to open the EEV again, which leads to reduced superheat and a falling hotgas temperature. The unit reacts quite slowly and begins an oscillative feedback loop.
The drop of the tempearure in the evaporator is often misinterpreted as ice on the evaporator, causing the unit to start a defrost cycle.

<img width="1200" height="1000" alt="ATXD25_hotgas" src="https://github.com/user-attachments/assets/5fa049bc-2dad-4321-849b-8fbef39972be" />

# Proposed solution
<img width="905" height="688" alt="Arduino_ATXD_1" src="https://github.com/user-attachments/assets/c1507dd8-eeb4-49fe-8a34-e4e510db97d8" />
