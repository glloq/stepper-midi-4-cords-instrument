# stepper-midi-4-cords-instrument
l'objectif est de faire jouer un instrument a 4 cordes comme un ukulele, banjo ou basse avec des moteurs pas a pas et des servomoteurs

## materiel

Nous utiliserons 2 arduino pour ce projet, un arduino uno avec un cnc shield v3 et un pca9685 pour controller les actionneurs et un arduino leonardo ou micro pour la gestions des messages midi par usb.
<img src="https://github.com/glloq/stepper-midi-4-cords-instrument/blob/main/img/schemas%20cnc%20shield.png" alt="cncShield" width=70% height=70%/>  


## code 

les reglages seront regroupé dans le fichier settings.h, il faudra adapter certain reglages en fonction du montage mecanique choisi (sens de rotation du moteur, distance entre open string et fin de course, les parametres des servo) et des parametres de l'instrument (position des frettes, numero midi de la corde, nombre de frettes utilisé sur la corde)
