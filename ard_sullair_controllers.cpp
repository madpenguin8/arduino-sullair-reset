///////////////////////////////////////////////////////
// Arduino sketch to reset maintenance warnings on Sullair controllers.
// This sketch was developed following the Sequencing and Protocol
// manual p/n 02250161-162.
// Copyright 2012 Mike Diehl
// madpenguin8@yahoo.com
///////////////////////////////////////////////////////

// RTS pin
const int rtsPin = 2;

// Button inputs
const int maintWarnPin = 3;
const int oilFilterPin = 4;
const int separatorPin = 5;
const int airFilterPin = 6;
const int oilPin = 7;
const int oilAnalysisPin = 8;

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
    // Setup RTS pin.
    pinMode(rtsPin, OUTPUT);
    digitalWrite(rtsPin, LOW);

    // Setup input pins
    pinMode(maintWarnPin, INPUT);
    pinMode(oilFilterPin, INPUT);
    pinMode(separatorPin, INPUT);
    pinMode(airFilterPin, INPUT);
    pinMode(oilPin, INPUT);
    pinMode(oilAnalysisPin, INPUT);

    // Setup serial port
    Serial.begin(9600);
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
        readButtonInputs();
    }
}

void readButtonInputs()
{
    // Read button states and act on them
    // Return from the function on first high button
    if(digitalRead(maintWarnPin)){
        sendMessge(maintWarnPin);
        return;
    }
    
    if(digitalRead(oilFilterPin)){
        sendMessge(oilFilterPin);
        return;
    }
    
    if(digitalRead(separatorPin)){
        sendMessge(separatorPin);
        return;
    }
    
    if(digitalRead(airFilterPin)){
        sendMessge(airFilterPin);
        return;
    }
    
    if(digitalRead(oilPin)){
        sendMessge(oilPin);
        return;
    }
    
    if(digitalRead(oilAnalysisPin)){
        sendMessge(oilAnalysisPin);
        return;
    }
}

void sendMessge(int messageType)
{
    // Reset the debounce timer
    previousMillis = millis();

    // Send selected message type
    switch (messageType){
        case maintWarnPin:
            resetMaintenanceWarning();
            break;
        case oilFilterPin:
            resetOilFilter();
            break;
        case separatorPin:
            resetSeparator();
            break;
        case airFilterPin:
            resetAirFilter();
            break;
        case oilPin:
            resetOil();
            break;
        case oilAnalysisPin:
            resetOilAnalysis();
            break;
        default:
            return;
    }
}

// Serial communications methods
// Set RTS pin HIGH, transmit data then bring RTS back to LOW
void resetMaintenanceWarning()
{
    // Set RS485 shifter to transmit
    digitalWrite(rtsPin, HIGH);
    // Begin serial message
    Serial.write(2); // <STX> marks begining of message
    Serial.print(maintWarnMsg);
    Serial.print(checksum(maintWarnMsg), HEX);
    Serial.println();
    Serial.flush();
    // Wait a millisecond
    delay(1);
    // Return RS485 shifter to receive
    digitalWrite(rtsPin, LOW);
}

void resetOilFilter()
{
    digitalWrite(rtsPin, HIGH);
    Serial.write(2);
    Serial.print(oilFilterMsg);
    Serial.print(checksum(oilFilterMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    digitalWrite(rtsPin, LOW);
}

void resetSeparator()
{
    digitalWrite(rtsPin, HIGH);
    Serial.write(2);
    Serial.print(separatorMsg);
    Serial.print(checksum(separatorMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    digitalWrite(rtsPin, LOW);
}

void resetAirFilter()
{
    digitalWrite(rtsPin, HIGH);
    Serial.write(2);
    Serial.print(airFilterMsg);
    Serial.print(checksum(airFilterMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    digitalWrite(rtsPin, LOW);
}

void resetOil()
{
    digitalWrite(rtsPin, HIGH);
    Serial.write(2);
    Serial.print(oilMsg);
    Serial.print(checksum(oilMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    digitalWrite(rtsPin, LOW);
}

void resetOilAnalysis()
{
    digitalWrite(rtsPin, HIGH);
    Serial.write(2);
    Serial.print(oilAnalysisMsg);
    Serial.print(checksum(oilAnalysisMsg), HEX);
    Serial.println();
    Serial.flush();
    delay(1);
    digitalWrite(rtsPin, LOW);
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
