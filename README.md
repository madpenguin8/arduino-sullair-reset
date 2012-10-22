es-plus-parser
==============

ESPlusDataStringParser is a Cocoa class intended to decode data from a Gardner Denver® 
ES+® compressor controller. Data from the controller is in the form of a string 
of ASCII characters. Support is available for all three controller responses
which includes:

Operating Data

Operating Mode

Service Data

This class was written based on the protocol specification outlined in the 
Gardner Denver® drawing number 305EAU1255. This class should also work with
RS2000® controllers as well. However, the RS2000® controller does not support
motor current monitoring and will always return 0 for this value.

This class does not support stripping of the STX (start) and ETX (end) characters and they
must be stripped before parsing. This class was written to work in conjunction with
an Arduino based JSON server that can be found at http://github.com/madpenguin8/arduino-es-plus-server
This class should also works well through direct access of data via the RS232 or RS485
ports provided by the controller for remote monitoring.


