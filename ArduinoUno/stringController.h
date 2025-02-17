#ifndef STRINGCONTROLLER_H
#define STRINGCONTROLLER_H

#include <AccelStepper.h>
#include <Bounce2.h>
#include <Adafruit_PWMServoDriver.h>
#include "settings.h"

class StringController {
public:
  int stringIndex;
  const StringMapping* mapping;
  AccelStepper stepper;
  Bounce endStop;
  Adafruit_PWMServoDriver* pca;

  int currentFret;   // Indice du fret actuel (0 = corde à vide)
  int currentNote;   // Note MIDI actuellement jouée (-1 si libre)
  bool isMoving;
  bool fingerPressed;

  // Constructeur
  StringController(int index, const StringMapping* map, Adafruit_PWMServoDriver* pcaDriver)
    : stringIndex(index),
      mapping(map),
      // Note : le microstepping est géré par le driver TMC2209 lui-même, mais STEPS_PER_MM doit être cohérent.
      stepper(AccelStepper::DRIVER, mapping->pinStep, mapping->pinDir),
      pca(pcaDriver),
      currentFret(0),
      currentNote(-1),
      isMoving(false),
      fingerPressed(false)
  {
    // Appliquer les réglages pour le moteur
    stepper.setMaxSpeed(MAX_SPEED);
    stepper.setAcceleration(ACCELERATION);

    // Inverser le sens si nécessaire
    if(mapping->reverseDirection) {
      stepper.setPinsInverted(true, false, false);
    }
    
    // Configuration de l'endstop via Bounce2
    endStop.attach(mapping->pinEndStop, INPUT_PULLUP);
    endStop.interval(5);
  }

  // Procédure de homing via l'endstop (on suppose que l'endstop est actif en LOW)
  void initialize() {
    while(digitalRead(mapping->pinEndStop) == HIGH) {
      stepper.move(-1);
      stepper.run();
    }
    stepper.setCurrentPosition(0);
    currentFret = 0;
  }

  // Vérifie si la note est jouable sur cette corde
  bool canPlay(byte note) {
    return (note >= mapping->firstNote && note < mapping->firstNote + mapping->fretNbr);
  }

  // Vérifie si cette corde joue la note donnée
  bool isPlaying(byte note) {
    return (currentNote == note);
  }

  // Calcule la distance (en pas) entre la position actuelle et la position cible du fret
  int distanceToFret(byte note) {
    int fretIndex = note - mapping->firstNote;
    if (fretIndex < 0 || fretIndex >= mapping->fretNbr) return 100000;
    int targetPos = mapping->homeOffset;
    if (fretIndex > 0) {
      targetPos += fretsSpacings[fretIndex - 1] * MICROSTEPPING;
    }
    return abs(targetPos - stepper.currentPosition());
  }

  // Commande pour jouer une note : déplacement vers le fret cible puis action des servos
  void play(byte note) {
    if (currentNote != -1) {
      stop(); // Si une note est déjà jouée, la stopper
    }
    int fretIndex = note - mapping->firstNote;
    if (fretIndex < 0 || fretIndex >= mapping->fretNbr) return;
    int targetPos = mapping->homeOffset;
    if (fretIndex > 0) {
      targetPos += fretsSpacings[fretIndex - 1] * MICROSTEPPING;
    }
    stepper.moveTo(targetPos);
    isMoving = true;
    currentNote = note;
  }

  // Commande d'arrêt : relâche la note en réinitialisant les servos
  void stop() {
    fingerPressed = false;
    pca->setPWM(mapping->servoFingerPin, 0, angleToPWM(mapping->angleServoFingerOpen));
    pca->setPWM(mapping->servoPluckPin, 0, angleToPWM(mapping->angleServoPluck));
    currentNote = -1;
  }

  // Méthode à appeler régulièrement pour mettre à jour le déplacement et les actions servo
  void update() {
    if (isMoving) {
      stepper.run();
      if (stepper.distanceToGo() == 0) { // Position cible atteinte
        isMoving = false;
        // Actionner le servo doigt pour appuyer sur la corde
        pca->setPWM(mapping->servoFingerPin, 0, angleToPWM(mapping->angleServoFingerClose));
        fingerPressed = true;
        // Alternance pour le servo de grattage (mouvement de médiator)
        static bool toggle = false;
        int pluckAngle = mapping->angleServoPluck;
        if (toggle) {
          pluckAngle += SERVO_ANGLE_STRUM;
        } else {
          pluckAngle -= SERVO_ANGLE_STRUM;
        }
        pca->setPWM(mapping->servoPluckPin, 0, angleToPWM(pluckAngle));
        toggle = !toggle;
      }
    }
  }

  // Convertit un angle (0–180°) en valeur PWM linéaire entre SERVO_PWM_MIN et SERVO_PWM_MAX
  int angleToPWM(int angle) {
    return SERVO_PWM_MIN + ((SERVO_PWM_MAX - SERVO_PWM_MIN) * angle) / 180;
  }
};

#endif // STRINGCONTROLLER_H
