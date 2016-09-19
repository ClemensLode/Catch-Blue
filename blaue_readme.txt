L�sungsvorschlag zur Aufgabe "Eliminiert die Blauen!"

Karlsruhe 9.1.2010, Clemens Lode


Zum Inhalt:

* ClawMisc, ClawGUI und ClawSDL stellen zusammen mit SDL, SDL_Image und SDL_ttf die API dar. Erstere sind Eigenentwicklungen, die sich allerdings noch im Versuchsstudium befinden und deren Code schon etwas �lter ist.

* "Blaue" ist das eigentliche Projekt, gestartet wird es im Verzeichnis "Blaue" ("Blaue.exe").

* Visual C++ 2008 Express L�sungs- und Projektdateien sind beigelegt ("Blaue/Blaue.sln").
Die Libraries (SDL etc.) befinden sich ebenfalls in dem Verzeichnis.

* Im "settings" Verzeichnis befinden sich die Einstellungen f�r die GUI ("ui.cfg") und f�r die spezielle Anwendung (wie auf dem Aufgabenblatt vermerkt, "blaue.cfg").

* Weitere Dateien sind die DLLs, ein Freetype Font und ein Verzeichnis mit Programmstrings.

* Bedienung:

F1: Pause
F2: Fortfahren
F4: Reset
Leertaste: Einen einzelnen Schritt ausf�hren
ALT+ENTER: Zwischen Vollbild und Fenstermodus wechseln
ESC: Ende

* Im Programm sind folgende Elemente zu sehen:
- diskrete Raster
- Hindernisse (wei�e K�sten)
- Rote und blaue Agenten (rote K�sten, werden dunkler (ger�uschlos), wenn sie sich nicht bewegen)
- rote/blaue Pfade
- aktueller Zustand (suchen, fliehen, verstecken etc.)
- H�rreichweite (hellblau/hellrot)
- Sichtreichweite (dunkelblau/dunkelrot)
- �berlagerte Reichweiten (Mischfarben)

* Troubleshooting:
Sollte, aus welchen Gr�nden auch immer, etwas mit den Einstellungen in den .cfg Dateien kaputt sein, kann man diese Dateien einfach l�schen, das Programm erstellt neue mit funktionierenden Standardwerten.

Viel Spa� beim Testen ^_^