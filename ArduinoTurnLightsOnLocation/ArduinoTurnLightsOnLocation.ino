// Universum | Universum Projects > lightsOnEnter

// Andrei Florian 14/APR/2018 - 15/APR/2018

  // 1.1 4/MAY/2018 Andrei Florian
  
#include <Bridge.h>
#include <BlynkSimpleYun.h>

char auth[] = ""; // insert project token

float latitude;
float longitude;

float myLocationLat = 53.357918;
float myLocationLng = -6.258278;

int pinLED = 3;
int proDebug = 1;
int geolocation = 1;

float maxLat = myLocationLat + 0.001;
float minLat = myLocationLat - 0.001;
float maxLng = myLocationLng + 0.001;
float minLng = myLocationLng - 0.001;

BLYNK_WRITE(V0) 
{
  latitude = param[0].asFloat();
  longitude = param[1].asFloat();

  latitude = 53.357918;
  longitude = -6.258278;
}

void processLocation()
{
  if(latitude <= maxLat && latitude >= minLat &&
    longitude <= maxLng && longitude >= minLng)
  {
    geolocation = 1;
  }
  else
  {
    geolocation = 0;
  }
  
  if(geolocation == 1)
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
  Blynk.begin(auth); // initialise Blynk

  if(proDebug == 1)
  {
    Serial.begin(9600);
    while(!Serial) {};
    delay(1000);
    
    Serial.println("Universum");
    delay(500);
    Serial.println("  lightsOnEnter");
    delay(500);
    Serial.println("    Andrei Florian");
    delay(1000);
    Serial.println("");
    Serial.print("maxLat  "); Serial.println(maxLat, 8);
    Serial.print("minLat  "); Serial.println(minLat, 8);
    Serial.print("maxLng  "); Serial.println(maxLng, 8);
    Serial.print("minLng  "); Serial.println(minLng, 8);
    Serial.print("actual Lat  "); Serial.println(latitude, 8);
    Serial.print("actual Lng  "); Serial.println(longitude, 8);
    Serial.println("");
  }

  delay(1000);
}

void loop()
{
  Blynk.run();
  processLocation();

  if(proDebug == 1)
  {
    Serial.print("actual Lat  "); Serial.println(latitude, 8);
    Serial.print("actual Lng  "); Serial.println(longitude, 8);
    Serial.println(geolocation);
  }
}
