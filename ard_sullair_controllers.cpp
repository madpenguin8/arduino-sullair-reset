///////////////////////////////////////////////////////
// Arduino sketch to reset maintenance warnings on Sullair controllers.
// This sketch was developed following the Sequencing and Protocol
// manual p/n 02250161-162.
// Copyright 2012 Mike Diehl
// madpenguin8@yahoo.com
///////////////////////////////////////////////////////

// Serial messages excluding trailing checksum, they are generated later.
char maintWarnMsg[] = {"01P54,2000,"};
char oilFilterMsg[] = {"01P55,2000,"};
char separatorMsg[] = {"01P56,8000,"};
char airFilterMsg[] = {"01P57,2000,"};
char oilMsg[] = {"01P58,8000,"};
char oilAnalysisMsg[] = {"01P59,2000,"};

// Previous uptime value for timer.
long previousMillis = 0;

// Timer interval in milliseconds
long interval = 1000;

void  setup()
{
    // Setup serial port
    Serial.begin(9600);

    // Setup pin 2 as RTS out.
    DDRD = DDRD | B00000100;

    // Setup input pins 3-8
    DDRD = DDRD & B00000111;
    DDRB = DDRB & B11111110;

    // Set RTS LOW
    PORTD &= B11111011;
}

void loop()
{
    // Get the current uptime
    unsigned long currentMillis = millis();
	
    // Reset previousMillis in the event of a rollover.
    if(previousMillis > currentMillis){
        previousMillis = 0;
    }

    // Provides debounce
    if(currentMillis - previousMillis > interval){
        previousMillis = millis();
        readButtonInputs();
    }
}

void readButtonInputs()
{
    // Read button states and act on them
    // Return from the function on first high button

    if(PIND & B00001000){ // Pin 3
        resetMaintenanceWarning();
        return;
    }

    if(PIND & B00010000){ // Pin 4
        resetOilFilter();
        return;
    }

    if(PIND & B00100000){ // Pin 5
        resetSeparator();
        return;
    }

    if(PIND & B01000000){ // Pin 6
        resetAirFilter();
        return;
    }

    if(PIND & B10000000){ // Pin 7
        resetOil();
        return;
    }

    if(PINB & B00000001){ // Pin 8
        resetOilAnalysis();
        return;
    }
}

// Serial communications methods
// Set RTS pin HIGH, transmit data then bring RTS back to LOW
void resetMaintenanceWarning()
{
    // Set RS485 shifter to transmit
    PORTD |= B00000100;
    // Begin serial message
    Serial.write(2); // <STX> marks begining of message
    Serial.print(maintWarnMsg);
    Serial.print(checksum(maintWarnMsg), HEX);
    Serial.println();
    Serial.flush();
    // Wait a millisecond
    delay(1);
    // Return RS485 shifter to receive
    PORTD &= B11111011;
}

void resetOilFilter()
{
    PORTD |= B00000100;
    Serial.write(2);
    Serial.print(oilFilterMsg);
    Serial.print(checksum(oilFilterMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    PORTD &= B11111011;
}

void resetSeparator()
{
    PORTD |= B00000100;
    Serial.write(2);
    Serial.print(separatorMsg);
    Serial.print(checksum(separatorMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    PORTD &= B11111011;
}

void resetAirFilter()
{
    PORTD |= B00000100;
    Serial.write(2);
    Serial.print(airFilterMsg);
    Serial.print(checksum(airFilterMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    PORTD &= B11111011;
}

void resetOil()
{
    PORTD |= B00000100;
    Serial.write(2);
    Serial.print(oilMsg);
    Serial.print(checksum(oilMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    PORTD &= B11111011;
}

void resetOilAnalysis()
{
    PORTD |= B00000100;
    Serial.write(2);
    Serial.print(oilAnalysisMsg);
    Serial.print(checksum(oilAnalysisMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    PORTD &= B11111011;
}

// Generate the message checksum
int checksum (char * str)
{
    byte sum = 0 ;
    char ch ;
    while ((ch = *str++) != 0)
        sum -= ch ;
    return sum ;
}
