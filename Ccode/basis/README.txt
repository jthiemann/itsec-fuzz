Einfache C implementierung von Martin

	-Kann als Grundlage anderer Projekte dienen
	-arbeitet auch mit virtuelCAN (vcan)
	-ürsprünglich für zum test der maximalen Geschwindigkeit gebaut 

can Recieve

	-erwartet definierte Anzahl CAN Nachrichten in while Schleife
	-beendet bei erhalt einer Nachricht mit Inhalt 07ff
	-zeigt minimale Statistik an
	-alle Parameter hardcoded
	
can Send
	
	-schickt in einer Schleife CAN Nachrichten
	-letzte Nachricht soll 07ff enhalten
	-(ID == NACHRICH) für alle gesendeten Nachrichten
	
Benutzung

	-entweder Native CAN Schnittstellen oder vcan initalisieren 
	-Code auf Schnittstellennamen anpassen
	-compilen make
	-reciever starten ./targetname
	-sender starten --
	-falls reciever nicht selbst beendet, obwohl sender fertig ist, wurde die 0x07ff Nachricht nicht übertragen
	-in diesem Fall manuell mit z,B cansend 123#7ff beenden
	
VCAN

	-es liegt ein script bei, dass zwei VCAN nodes erstellt
	-diese werden mit cangw (Gateway) bidirektional auf allen ID's verbunden
	-router regeln können aber auch genauer spezifiziert werden
	-evtl sind ein paar apt-get's nötig ( am besten ein VCAN tutorial suchen )
	