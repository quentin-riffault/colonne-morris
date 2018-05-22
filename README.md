# Colonne Morris
## Composants
- 2 Arduino MEGA
- Shield Ethernet
- Capteur de CO2 SEN0159
- Capteur de particules DSM051A
- Capteur de température
- Shield moteur
- Ventilateur 12V DC
- Bulleur 12V DC

## Câblage
### Arduino Mega 1:
#### Shield Ethernet
Placer au dessus de la carte
#### Capteur de CO2
- Brancher au 5V/GND
- Brancher le fil de données en A0

#### DSM051A
- Brancher au 5V/GND
- Brancher le fil de données sur D39

#### Capteur de température
- Brancher au 5V/GND
- Brancher le fil de données sur la broche OneWire (D7)
### Arduino Mega 2:
#### Motor Shield
- Placer au dessus de la carte
- Le bulleur en A
- Le ventilateur en B

### Communication serial
- RX1 à TX1
- TX1 à RX1