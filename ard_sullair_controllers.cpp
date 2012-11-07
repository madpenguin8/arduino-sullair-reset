///////////////////////////////////////////////////////
// Arduino sketch to reset maintenance warnings on Sullair controllers.
// This sketch was developed following the Sequencing and Protocol
// manual p/n 02250161-162.
// Copyright 2012 Mike Diehl
// madpenguin8@yahoo.com
///////////////////////////////////////////////////////

//// Message Order ////
// Maintenance Warning - 0
// Oil Filter - 1
// Separator - 2
// Air Filter -3
// Oil - 4
// Oil Analysis - 5

char* messages[] = {"01P54,2000,", "01P55,2000,",
                    "01P56,8000,", "01P57,2000,",
                    "01P58,8000,", "01P59,2000,"};

// Previous uptime value for timer.
long previousMillis = 0;

// Timer interval in milliseconds
long interval = 1000;

void  setup()
{
    // Setup serial port
    Serial.begin(9600);

    // Setup pin 2 as RTS out.
    DDRD |= B00000100;

    // Setup input pins 3-7
    DDRD &= B00000111;

    // Set RTS LOW
    PORTD &= B11111011;

    // Set remianing pins to output and high (for powersave)
    DDRB = B11111111;
    PORTB = B11111111;

    // Disable the ADC (for powersave)
    ADCSRA = 0;
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
    // Read button states and act on first high
    // Send maintenance warning clear first, then
    // send the actual message.

    if(PIND & B00001000){ // Pin 3
        sendResetMessage(0);
        sendResetMessage(1);
    }

    else if(PIND & B00010000){ // Pin 4
        sendResetMessage(0);
        sendResetMessage(2);
    }

    else if(PIND & B00100000){ // Pin 5
        sendResetMessage(0);
        sendResetMessage(3);
    }

    else if(PIND & B01000000){ // Pin 6
        sendResetMessage(0);
        sendResetMessage(4);
    }

    else if(PIND & B10000000){ // Pin 7
        sendResetMessage(0);
        sendResetMessage(5);
    }
}

void sendResetMessage(int selected)
{
    // Set RS485 shifter to transmit
    PORTD |= B00000100;
    // Begin serial message
    Serial.write(2); // <STX> marks begining of message
    Serial.print(messages[selected]);
    Serial.println(checksum(messages[selected]), HEX);
    Serial.flush();
    // Wait 2 milliseconds for CR-LF to clear.
    delay(2);
    // Return RS485 shifter to receive
    PORTD &= B11111011;
    // Read the controllers response, we might use it later
    while(Serial.available()){
		delay(1);
		Serial.read();
    }
}

// Generate the message checksum
int checksum (char * str)
{
    byte sum = 0;
    char ch;
    while ((ch = *str++) != 0)
        sum -= ch;
    return sum;
}
