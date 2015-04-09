Fermer le logiciel "Arduino"

Copier les fichiers contenue dans le répertoire "HID file for arduino IDE" dans le répertoire
"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino"


Double clic sur "Hotas_Joystick.ino" (ouverture du logiciel "Arduino")

Installation de la lib "MsTimer2"
Extraire le dossier "MSTimer2.zip" se trouvant dans le répertoire "Other Lib"
- Dans le logiciel "Arduino" -> "Croquis" -> "Importer bibliothèque..." -> "Ajouter bibliothèque..."
- Sélectionner le dossier "MsTiler2"

Configuration de la cible
- Sélectionner "Arduino Léonardo" dans "Outils" -> "Type de carte"
- Sélectionner le port dans "Outils" -> "Port"

Téléverser

Activation du système de calibration
- Immédiatement après la fin du téléversement, ouvir le "Moniteur série" en 9600 baud
- Le système attend 10 secondes avant de démarrer. Il est nécessaire d'envoyer un caractère pour activer le système de calibration durant ce laps de temps.
- Suivre les indications