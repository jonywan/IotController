#include <ESP8266WiFi.h>
#include <SPI.h>
#include <ArduinoJson.h>
/* __________________________________________________________________________________________
 *|                                                                                          |                                                                                          | 
 *|                                                                                          | 
 *|                                                                                          |
 *|                                                                                          |
 *|                                                                                          |
 *|                                    Create By Jony Wanna                                  |
 *|                                                                                          |
 *|                                                                                          | 
 *|                                                                                          | 
 *|                                                                                          | 
 *|__________________________________________________________________________________________|
 */



 
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*
  ________________________________________________________________________________________
 |                                                                                        | 
 |                                                                                        |
 |                                                                                        |
 |                              Fill in your details                                      |
 |                                                                                        | 
 |________________________________________________________________________________________|
 */
 /*
    ________________________________________________________________________________________
   |                                                                                        | 
   |                                                                                        |
   |                                                                                        |
   |                              SSID AND PASSWORD OF YOUR NETWORK                         |*/
   
                                      char ssid[] = " Your SSID "; 
              
                                      char pass[] = " Your Pass ";


                                  
 /*|                              ENTER YOUR SIREAL FROM THE EMAIL                          |*/
 
                                      String serial = " Your Serial " ;                                                              
 /*|                                                                                        |
   |                                                                                        |
   |                                                                                        | 
   |________________________________________________________________________________________|
 */


/*--------------------------------------------------------------------------------------------------------------------------------*/






/*
  ________________________________________________________________________________________
 |                                                                                        | 
 |                                                                                        |
 |          Dont Change The Touch And Change The Code After This Message                  |
 |                        Ore The Program Dose Not Work!!!                                |
 |                                                                                        | 
 |________________________________________________________________________________________|
 */
//server Name
char server[] = "dj-security.com";
WiFiClient client;

void setup() {
   Serial.begin(9600);
 // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to...");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");    
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");
  Serial.println(WiFi.localIP());
 
/*-----------------------------------------------------------*/
}


void Online()
{  
    client.connect(server,80);
     if(client.connected())
    {
     
     client.print("GET /JonyProject/ControlPanel.php?");
     client.print("Online="+serial);
     client.println(" HTTP/1.1");
     client.println("Host: dj-security.com");
     client.println(); // Empty line
     client.println(); // Empty line
     
     unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 25000) { //Try to fetch response for 25 seconds
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }
    // Read all the lines of the reply from server and and savi on line just the json
    String line; 
    for(int i=0;i<11&&client.available();i++){  
      line = client.readStringUntil('\r');     
    }
  

    //json read the line by json 6
    StaticJsonDocument<1000> doc;
    deserializeJson(doc,line);
    if(doc["RedLedExists"])
    {
      int pin = doc["RedLedPin"];
      int stat = doc["RedLed"];
      pinMode(pin,OUTPUT);
      digitalWrite(pin,stat==1?HIGH:LOW);
    }
    if(doc["GreenLedExists"])
    {
      int pin = doc["GreenLedPin"];
      int stat = doc["GreenLed"];
      pinMode(pin,OUTPUT);
      digitalWrite(pin,stat==1?HIGH:LOW);
    }
    if(doc["RelayExist"])
    {
      int pin = doc["Relay_Pin"];
      int stat = doc["Relay"];
      pinMode(pin,OUTPUT);
      digitalWrite(pin,stat==1?LOW:HIGH);
    }
    if(doc["RgbLedExist"])
    {
      int pinR = doc["RgbLed_PinR"];
      int pinG = doc["RgbLed_PinG"];
      int pinB = doc["RgbLed_PinB"];
      int r = doc["RgbR"];
      int g = doc["RgbG"];
      int b = doc["RgbB"];
      pinMode(pinR,OUTPUT);
      pinMode(pinG,OUTPUT);
      pinMode(pinB,OUTPUT);
      analogWrite(pinR , r);
      analogWrite(pinG , g);
      analogWrite(pinB , b);
    }
    if(doc["DistanceSensorExist"])
    {
      int TrigPin = doc["DistanceSensor_PinTrig"];
      int EchoPin = doc["DistanceSensor_PinEcho"];
      pinMode(TrigPin,OUTPUT);
      pinMode(EchoPin,INPUT);
      
      digitalWrite(TrigPin,HIGH);
      delayMicroseconds(10);
      digitalWrite(TrigPin,LOW);

      long duration = pulseIn(EchoPin, HIGH);
      int distance= duration*0.034/2; 
   
       
     client.print("GET /JonyProject/ControlPanel.php?");
     client.print("UpdateDistance="+serial+"&Distance="+distance);
     client.println(" HTTP/1.1");
     client.println("Host: dj-security.com");
     client.println(); // Empty line
     client.println(); // Empty line   
     
    }
    if(doc["ForceSensorExist"])
    {
      int fsrPin = A0;
      int fsrReading;
      fsrReading = analogRead(fsrPin);
     client.print("GET /JonyProject/ControlPanel.php?");
     client.print("UpdateForceSensor="+serial+"&SqueezeIntensity="+fsrReading);
     client.println(" HTTP/1.1");
     client.println("Host: dj-security.com");
     client.println(); // Empty line
     client.println(); // Empty line  
      
    }
    delay(500);
   }
   
}
   
void loop() {
   Online();
}
