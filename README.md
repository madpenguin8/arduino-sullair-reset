arduino-sullair-reset
=====================

This is an Arduino sketch for resetting Maintenance reminders on Sullair® SupervisorII® and WS® controllers.

Connection to the controller is via a RS485 connection.

A level shifter is required to convert Arduino's TTL logic to RS485 levels, something like a SP3485 or similar. 

Digital Pin 2 is allocated for direction control of the level shifter.

Digital Pins 3-7 trigger which messages are sent to the controller.

Simple debounce logic is provided but the messages will repeat once a second, this needs to be a one-shot.

