Fermer le logiciel "Arduino"

Copier les fichiers contenue dans le r�pertoire "HID file for arduino IDE" dans le r�pertoire
"C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino"


Double clic sur "Hotas_Joystick.ino" (ouverture du logiciel "Arduino")

Installation de la lib "MsTimer2"
Extraire le dossier "MSTimer2.zip" se trouvant dans le r�pertoire "Other Lib"
- Dans le logiciel "Arduino" -> "Croquis" -> "Importer biblioth�que..." -> "Ajouter biblioth�que..."
- S�lectionner le dossier "MsTiler2"

Configuration de la cible
- S�lectionner "Arduino L�onardo" dans "Outils" -> "Type de carte"
- S�lectionner le port dans "Outils" -> "Port"

T�l�verser

Activation du syst�me de calibration
- Imm�diatement apr�s la fin du t�l�versement, ouvir le "Moniteur s�rie" en 9600 baud
- Le syst�me attend 10 secondes avant de d�marrer. Il est n�cessaire d'envoyer un caract�re pour activer le syst�me de calibration durant ce laps de temps.
- Suivre les indications