# RL

###Installation

1. Branch "template_state" runterladen
2. 64bit sfml https://www.sfml-dev.org/download/sfml/2.5.1/ 
   in den lib ordner entpacken und das -2.5.* löschen.
   Also die Ordnerstruktur für z.b. include sollte dann so aussehen:
   RL-init\lib\SFML\include
3. In CleanStart die .sln starten
4. Statusleiste „Projekt/Eigenschaften“ öffnen
5. „Konfiguration“ auf „Alle Konfigurationen“ schalten
6. Links im Reiter „Konfigurationseigenschaften/Allgemein“ die „Windows SDK-Version“
   auswählen die bei euch installiert ist.
7. Mit OK bestätigen.
8. Falls „Debug“ ausgewählt ist auf „Release“ und 64 bit schalten 
   und mit STRG+F5 kompilieren und starten.
9. Jetzt sollte Snake mit Random Agenten starten und spielbar sein

###Doku

In den Klassen Environment.h und Actor.h habe ich versucht so gut wie möglich 
zu beschreiben wie die Klassen miteinander arbeiten.
Der Random_Agent ist ein Beispiel wie ein Agent Aktionen ausführen kann.
Snake ist ein Beispiel wie mehrere Agenten gehandled werden könnten.
