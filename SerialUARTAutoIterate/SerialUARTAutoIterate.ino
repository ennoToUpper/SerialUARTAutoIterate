/*
  Code based on this: https://forum.arduino.cc/t/serial-input-basics-updated/382007/3
  
  This program iterates over all the baudrates and configurations.
  If the data it received is readable the baudrate and the configuration is found. 
  It does not detect the baudrate automatically but allows the user to see the used settings.
  The config is shown in hex (i.e. 0x02) and can be used like that but you can also hover over the variables (SERIAL_6N1 etc.) 
  and you'll see which one corresponds to the given hex value. Example: 0x02 -> SERIAL_6N1; 0x2C -> SERIAL_7E2

  Example:
  //Not correct
  baudrate: 115200 config: 0x00 data: C%&W§$%65"§$6X§39§$& 

  //correct 
  baudrate: 9600 config: 0x62 data: This is a readable string

  //Settings would be used like this: Serial.begin(9600, SERIAL_8O2);
  */

String dataReceived = "";      
bool outputData = true;
int counter = 0;

int baudrates[] = {300, 600, 750, 1200, 2400, 4800, 9600, 19200, 31250, 38400, 57600, 74880, 115200};
int baudcount = 0;
int currentBaud = 300;
int configCount = 0;
uint8_t configuration = 0x00;

uint8_t configs[] = 
  {
  SERIAL_5N1, SERIAL_6N1, SERIAL_7N1, SERIAL_8N1,
  SERIAL_5N2, SERIAL_6N2, SERIAL_7N2, SERIAL_8N2,
  SERIAL_5E1, SERIAL_6E1, SERIAL_7E1, SERIAL_8E1,
  SERIAL_5E2, SERIAL_6E2, SERIAL_7E2, SERIAL_8E2,
  SERIAL_5O1, SERIAL_6O1, SERIAL_7O1, SERIAL_8O1,
  SERIAL_5O2, SERIAL_6O2, SERIAL_7O2, SERIAL_8O2
  };

void startConfigSerial()
{ 

  if(configCount > 23)
  {
    configCount = 0;
    baudcount++;

    if (baudcount > 12)
    {
      baudcount = 0;
    }

    currentBaud = baudrates[baudcount];
  }

  configuration = configs[configCount];

  configCount++;

  Serial.begin(currentBaud, configuration);
}

void setup() { 
  Serial.begin(9600);

  dataReceived.reserve(200);
}

void loop() {
  if (outputData) {
    sendData();        
    startConfigSerial();

    dataReceived = "baudrate: ";
    dataReceived += currentBaud; 
    dataReceived += " config: "; 
    dataReceived += configuration; 
    dataReceived += " data: "; 
    outputData  = false;
  }
}

void sendData()
{
  Serial.end();
  Serial.begin(9600);    
  Serial.println();
  dataReceived += "   ### End of Data ###";
  Serial.println(dataReceived);
  Serial.end();
}

void serialEvent() {
  while (Serial.available()) {

    int readRes = Serial.read();
    char inChar = (char) readRes;   

    dataReceived += inChar;
      
    counter++;

    if (counter > 63)
    {
      counter = 0;
      outputData
     = true;
    }
  }
}
