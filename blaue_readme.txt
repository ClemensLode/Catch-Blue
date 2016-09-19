Lösungsvorschlag zur Aufgabe "Eliminiert die Blauen!"

Karlsruhe 9.1.2010, Clemens Lode


Zum Inhalt:

* ClawMisc, ClawGUI und ClawSDL stellen zusammen mit SDL, SDL_Image und SDL_ttf die API dar. Erstere sind Eigenentwicklungen, die sich allerdings noch im Versuchsstudium befinden und deren Code schon etwas älter ist.

* "Blaue" ist das eigentliche Projekt, gestartet wird es im Verzeichnis "Blaue" ("Blaue.exe").

* Visual C++ 2008 Express Lösungs- und Projektdateien sind beigelegt ("Blaue/Blaue.sln").
Die Libraries (SDL etc.) befinden sich ebenfalls in dem Verzeichnis.

* Im "settings" Verzeichnis befinden sich die Einstellungen für die GUI ("ui.cfg") und für die spezielle Anwendung (wie auf dem Aufgabenblatt vermerkt, "blaue.cfg").

* Weitere Dateien sind die DLLs, ein Freetype Font und ein Verzeichnis mit Programmstrings.

* Bedienung:

F1: Pause
F2: Fortfahren
F4: Reset
Leertaste: Einen einzelnen Schritt ausführen
ALT+ENTER: Zwischen Vollbild und Fenstermodus wechseln
ESC: Ende

* Im Programm sind folgende Elemente zu sehen:
- diskrete Raster
- Hindernisse (weiße Kästen)
- Rote und blaue Agenten (rote Kästen, werden dunkler (geräuschlos), wenn sie sich nicht bewegen)
- rote/blaue Pfade
- aktueller Zustand (suchen, fliehen, verstecken etc.)
- Hörreichweite (hellblau/hellrot)
- Sichtreichweite (dunkelblau/dunkelrot)
- Überlagerte Reichweiten (Mischfarben)

* Troubleshooting:
Sollte, aus welchen Gründen auch immer, etwas mit den Einstellungen in den .cfg Dateien kaputt sein, kann man diese Dateien einfach löschen, das Programm erstellt neue mit funktionierenden Standardwerten.

Viel Spaß beim Testen ^_^