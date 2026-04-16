# ToDo-Applikation

Das Ziel ist die Entwicklung einer ToDo-Applikation.  
In diesem Repository sind bereits die Grundlagen dafür vorbereitet.

## Funktionsumfang
Am Ende der Implementierung soll es möglich sein:
- eine Aufgabe **zu löschen**
- alle Aufgaben **zu löschen**

## Nutzungsmöglichkeiten
Die Applikation muss auf zwei Arten bedienbar sein:
1. Über ein Benutzerinterface (UI)
2. Über eine REST API (die wird auch mit `curl` angesprochen)

## Persistenz
Der aktuelle Stand sieht vor, dass alle Aufgaben beim Beenden des Programms gelöscht werden.  
Ziel ist es jedoch, die Aufgaben persistent zu speichern, sodass sie auch nach einem Neustart des Programms weiterhin vorhanden sind.  
