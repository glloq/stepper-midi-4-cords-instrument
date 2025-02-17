#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <Adafruit_PWMServoDriver.h>
#include "settings.h"
#include "stringController.h"

class Instrument {
public:
  Adafruit_PWMServoDriver pca;
  StringController* strings[NUM_STRINGS];

  Instrument() : pca(PCA9685_ADDRESS) {
    for (int i = 0; i < NUM_STRINGS; i++) {
      strings[i] = nullptr;
    }
  }

  // Initialisation du PCA9685, création des StringControllers et homing des moteurs.
  void initialize() {
    pca.begin();
    pca.setPWMFreq(PCA9685_PWM_FREQ);
    for (int i = 0; i < NUM_STRINGS; i++) {
      strings[i] = new StringController(i, &stringMappings[i], &pca);
      strings[i]->initialize();
    }
  }

  ~Instrument() {
    for (int i = 0; i < NUM_STRINGS; i++) {
      if (strings[i]) {
        delete strings[i];
      }
    }
  }

  // Joue la note sur la corde disponible qui requiert le moins de déplacement.
  // Si toutes les cordes sont occupées, la note est ignorée.
  void play(byte note) {
    int bestIndex = -1;
    int minDistance = 100000;
    for (int i = 0; i < NUM_STRINGS; i++) {
      if (strings[i]->canPlay(note) && strings[i]->currentNote == -1) {
        int dist = strings[i]->distanceToFret(note);
        if (dist < minDistance) {
          minDistance = dist;
          bestIndex = i;
        }
      }
    }
    if (bestIndex != -1) {
      strings[bestIndex]->play(note);
    }
    // Sinon, toutes les cordes sont occupées → on ignore la note.
  }

  // Arrête la note jouée sur la corde correspondante.
  void stop(byte note) {
    for (int i = 0; i < NUM_STRINGS; i++) {
      if (strings[i]->isPlaying(note)) {
        strings[i]->stop();
        break;
      }
    }
  }

  // Met à jour l'état de toutes les cordes (déplacement des moteurs, action des servos).
  void update() {
    for (int i = 0; i < NUM_STRINGS; i++) {
      if (strings[i])
        strings[i]->update();
    }
  }
};

#endif // INSTRUMENT_H
