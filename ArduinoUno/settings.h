#ifndef SETTINGS_H
#define SETTINGS_H

#include <AccelStepper.h>

// --- Configuration du microstepping ---
// Utilisation des TMC2209 avec un microstepping de 1/16.
const int MICROSTEPPING = 16;  // 1/16 microstepping

// --- Configuration du PCA9685 ---
const int PCA9685_ADDRESS = 0x40;
const int PCA9685_PWM_FREQ = 50; // Fréquence pour les servos (Hz)
const int SERVO_PWM_MIN = 150;   // Valeur PWM minimale pour 0°
const int SERVO_PWM_MAX = 600;   // Valeur PWM maximale pour 180°
const int SERVO_ANGLE_STRUM = 20; // Variation d'angle pour le servo de grattage

// --- Paramètres des moteurs pas à pas ---
// STEPS_PER_MM doit être ajusté selon la configuration mécanique ET le microstepping
const int MAX_SPEED = 500;       // Vitesse maximale (en pas/sec)
const int ACCELERATION = 300;    // Accélération (en pas/sec²)
const int STEPS_PER_MM = 200;    // Exemple : nombre de pas par mm (à ajuster)

// --- Nombre de cordes ---
const int NUM_STRINGS = 4;

// --- Espacement des frettes en mm (exemple, à ajuster) ---
const float fretsSpacings[] = {20, 38, 55, 92, 97, 104, 117, 130, 142, 155, 163, 175};

// --- Configuration de chaque corde ---
struct StringMapping {
  byte firstNote;         // Note MIDI de la corde à vide
  byte fretNbr;           // Nombre de frettes disponibles
  uint8_t pinStep;        // Broche STEP du shield CNC
  uint8_t pinDir;         // Broche DIR du shield CNC
  uint8_t pinEndStop;     // Broche de l'endstop pour le homing
  int homeOffset;         // Décalage (en pas ou mm) entre l'endstop et la position corde à vide
  bool reverseDirection;  // Inverse le sens du moteur (true pour inverser)
  uint8_t servoFingerPin; // Canal PCA9685 pour le servo "doigt"
  uint8_t servoPluckPin;  // Canal PCA9685 pour le servo "grattage"
  int angleServoPluck;       // Angle de base pour le servo de grattage
  int angleServoFingerOpen;  // Angle pour l'état "ouvert" du servo doigt
  int angleServoFingerClose; // Angle pour l'état "fermé" du servo doigt
};

// Exemple de configuration pour 4 cordes
const StringMapping stringMappings[NUM_STRINGS] = {
  {40, 12, 2, 3, 4, 0, false, 0, 1, 90, 90, 70},  // Corde 1 : sens normal
  {45, 12, 5, 6, 7, 0, true,  2, 3, 90, 90, 70},   // Corde 2 : inversion activée
  {50, 12, 8, 9, 10, 0, false, 4, 5, 90, 90, 70},   // Corde 3 : sens normal
  {55, 12, 11, 12, 13, 0, false, 6, 7, 90, 90, 70}   // Corde 4 : sens normal
};

#endif // SETTINGS_H
