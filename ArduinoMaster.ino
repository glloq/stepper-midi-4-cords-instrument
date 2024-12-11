#include <MIDIUSB.h>

void setup() {
  Serial.begin(9600);      // Initialisation de la communication série pour le débogage
  Serial1.begin(31250);    // Initialisation de la communication série pour MIDI (31250 bauds)
}

void loop() {
  // Lire les messages MIDI reçus via USB
  midiEventPacket_t rxEvent = MidiUSB.read();  // Lire un paquet MIDI

  // Vérifier si un événement MIDI a été reçu
  if (rxEvent.header != 0) {  // Si un événement MIDI valide est disponible
    // Envoi du message complet (entête + données) via Serial1
    Serial1.write(rxEvent.header);  // En-tête du message (NoteOn/NoteOff)
    Serial1.write(rxEvent.byte1);   // Note (Data1)
    Serial1.write(rxEvent.byte2);   // Vélocité (Data2) => est ce que c'est utile dans notre cas ? 
  }

  MidiUSB.flush();  // Vider le tampon MIDI pour la prochaine lecture
}
