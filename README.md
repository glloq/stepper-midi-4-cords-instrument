# stepper-midi-4-cords-instrument
l'objectif est de faire jouer un instrument a 4 cordes comme un ukulele, banjo, guitare tenor ou basse avec des moteurs pas a pas et des servomoteurs

## materiel

Nous utiliserons 2 arduino pour ce projet, un arduino uno avec un cnc shield v3 et un pca9685 pour controller les actionneurs et un arduino leonardo ou micro pour la gestions des messages midi par usb.
<img src="https://github.com/glloq/stepper-midi-4-cords-instrument/blob/main/img/schemas%20cnc%20shield.png" alt="cncShield" width=70% height=70%/>  

Pour augmenter la vitesse il faudrait utiliser une alimentation 24V
Pour reduire le bruit il faudrait utiliser des TMC2209 en microstepping (1/16 idealement) pour atteindre du 600 a 1000mm/s


## code 

 #### bibliothèques nécessaires 
 - MidiUSB
 - AccelStepper
 - Bounce2
 - Adafruit_PWMServoDriver

 #### Reglages 
les reglages seront regroupé dans le fichier settings.h, il faudra adapter certain reglages en fonction du montage mecanique choisi (sens de rotation du moteur, distance entre open string et fin de course, les parametres des servo) et des parametres de l'instrument (position des frettes, numero midi de la corde, nombre de frettes utilisé sur la corde)

Tout les reglages de chaque corde sera stocké selon la structure suivante

```
struct StringMapping {
  byte firstNote;         // Note MIDI de la corde à vide
  byte fretNbr;           // Nombre de frettes disponibles
  uint8_t pinStep;        // Broche STEP du shield CNC
  uint8_t pinDir;         // Broche DIR du shield CNC
  uint8_t pinEndStop;     // Broche de l'endstop pour le homing
  int homeOffset;         // Décalage (en mm) entre l'endstop et la position corde à vide
  bool reverseDirection;  // Inverse le sens du moteur (true pour inverser)
  uint8_t servoFingerPin; // Canal PCA9685 pour le servo "doigt"
  uint8_t servoPluckPin;  // Canal PCA9685 pour le servo "grattage"
  int angleServoPluck;       // Angle de base pour le servo de grattage
  int angleServoFingerOpen;  // Angle pour l'état "ouvert" du servo doigt
  int angleServoFingerClose; // Angle pour l'état "fermé" du servo doigt
};
```
