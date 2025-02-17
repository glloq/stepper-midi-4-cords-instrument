#include <Wire.h>
#include "instrument.h"

Instrument instrument;
String incomingLine = "";

void setup() {
  Serial.begin(115200);  // Communication avec l'Arduino Leonardo
  instrument.initialize();
}

void loop() {
  // Lecture des commandes reçues via Serial
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      processCommand(incomingLine);
      incomingLine = "";
    } else {
      incomingLine += c;
    }
  }
  instrument.update();
}

void processCommand(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;
  // Format des commandes : "P,<note>" pour play ou "S,<note>" pour stop
  if (cmd.charAt(0) == 'P') {
    int firstComma = cmd.indexOf(',');
    int secondComma = cmd.indexOf(',', firstComma + 1);
    if (firstComma == -1 || secondComma == -1) return;
    String noteStr = cmd.substring(firstComma + 1, secondComma);
    String velStr = cmd.substring(secondComma + 1);
    byte note = noteStr.toInt();
    byte velocity = velStr.toInt();
    instrument.play(note);
  } else if (cmd.charAt(0) == 'S') {
    int commaIndex = cmd.indexOf(',');
    if (commaIndex == -1) return;
    String noteStr = cmd.substring(commaIndex + 1);
    byte note = noteStr.toInt();
    instrument.stop(note);
  }
}
