
#include <Bridge.h>
#include <BlynkSimpleYun.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

char auth[] = ""; // TODO - insert Blynk Token

// GPS Setup
static const int RXPin = 5, TXPin = 4; // connect GPS ~ RX - 4, TX - 3
static const uint32_t GPSBaud = 9600; // change to 4800 if not working

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// location of device
float myLocationLat;
float myLocationLng;

// location of phone
float latitude;
float longitude;

int pinLED = 3;
int proDebug = 1; // TODO - debugging?
int geolocation;

// automatically set parameters to a square around home
float maxLat;
float minLat;
float maxLng;
float minLng;

BLYNK_WRITE(V0) // store the geolocation to variable
{
  latitude = param[0].asFloat();
  longitude = param[1].asFloat();

  // debugging? - Consider un-commenting the lines below and test manually
    //latitude = 67.357899;
    //longitude = -8.259199;
}

void readGPS()
{
  while(ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      processGPS();
    }
  }

  if(millis() > 5000 && gps.charsProcessed() < 10)
  {
    if(proDebug == 1)
    {
      Serial.println("Error - GPS Module Not Detected");
    }
    
    while(1) {};
  }
}

void processGPS()
{
  if(gps.location.isValid())
  {
    // read GPS co-ordinates
    myLocationLat = gps.location.lat();
    myLocationLng = gps.location.lng();

    // automatically set parameters to a square around home
    maxLat = myLocationLat + 0.0001;
    minLat = myLocationLat - 0.0001;
    maxLng = myLocationLng + 0.0001;
    minLng = myLocationLng - 0.0001;

    processLocation();
  }
  else
  {
    Serial.println("Error - Invalid GPS Data");
    while(1) {};
  }
}

void processLocation()
{
  if(latitude <= maxLat && latitude >= minLat && // check if the phone is within home's parameters
    longitude <= maxLng && longitude >= minLng)
  {
    geolocation = 1;
  }
  else
  {
    geolocation = 0;
  }
  
  if(geolocation == 1) // turn LED on if phone is in perameter
  {
    digitalWrite(pinLED, HIGH);
  }
  else
  {
    digitalWrite(pinLED, LOW);
  }
}

void setup()
{
  pinMode(pinLED, OUTPUT);
  ss.begin(GPSBaud); // initialise Software Serial
  Blynk.begin(auth); // initialise Blynk

  if(proDebug == 1)
  {
    Serial.begin(9600);
    while(!Serial) {};
    delay(1000);
    
    Serial.println("MobileLightsOnWhenApproachingHome");
  }

  delay(1000);
}

void loop()
{
  Blynk.run();

  readGPS();

  if(proDebug == 1)
  {
    Serial.print("actual Lat  "); Serial.println(latitude, 8);
    Serial.print("actual Lng  "); Serial.println(longitude, 8);

    Serial.print("Max Lng     "); Serial.println(maxLng, 8);
    Serial.print("Min Lng     "); Serial.println(minLng, 8);
    Serial.print("Max Lat     "); Serial.println(maxLat, 8);
    Serial.print("Min Lat     "); Serial.println(minLat, 8);

    Serial.print("Device Lat  "); Serial.println(myLocationLat, 8);
    Serial.print("Device Lng  "); Serial.println(myLocationLng, 8);
    
    Serial.println(geolocation);
  }

  delay(2000);
}
