# Colonne Morris
## Composants
- 2 Arduino MEGA
- Shield Ethernet
- Capteur de CO2 SEN0159
- Capteur de particules DSM051A
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

### Arduino Mega 2:
#### Motor Shield
- Placer au dessus de la carte
- Le bulleur en A
- Le ventilateur en B

### Communication serial
- Relier les GND des arduino
- Relier le TX1 du serveur au RX1 de l'arduino moteur

## Problèmes de communication par ethernet
- Désactiver le proxy sur le navigateur si la connexion locale ne s'effectue pas correctement

## Librairies utilisées
- Materialize
- JQuery
- OneWire


