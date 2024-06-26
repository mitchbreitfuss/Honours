#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "header.h"


//connect Serial TX to D5
//connect Serial RX to D6


String file;


String stringType = "type=string";
String dataType = "type=float";

String message = "Hello World good day my fine young padawan";
String recieved;
float data = 3.1415;
int recieve = 0;



void setup()
{
    delay(1000);
    int baud = 9600;
    Serial.begin(baud);
	  COM.begin(baud);

    
    delay(2000);
    Serial.println("ESP Ready");
    COM.println("ESP Ready");


}


int server = 0;
float toSend = 3.14;
void loop(){

   
    if(COM.available()){
        delay(100);
        String recieved = COM.readStringUntil('\n');
        delay(100);
        Serial.println(recieved);
        
        if(recieved.indexOf("$C")>0){
            recieved.replace("$C","");
            float newDataValue = recieved.toFloat();
            if(newDataValue !=0){
                toSend = newDataValue;
            }
            
            Serial.println("[Recieved] " + recieved);

        }
        
        
        if(recieved.indexOf("$S")>0){ //Checks type of recieved message
            recieved.replace("$S","");
            server = recieved.toFloat();
            Serial.println("Connecting to server " + server);
            
        }

    }

    delay(100);
    // toSend = toSend + 0.01;
    // Serial.print(String(toSend)+"$D\n");
    // send(toSend);
    

    switch (server){
        case 1:
            // This corresponds to the inverter.
            serverConnect(1);
            
            server = 0;
            WiFi.disconnect();
            break;
    
        case 2:
            serverConnect(2);
            server = 0;
            WiFi.disconnect();
            break;
        case 100:
            serverConnect(1);
            WiFi.disconnect();
            delay(10000);
            
            serverConnect(2);
            WiFi.disconnect();
            
            
            
            server = 0;
            break;
            
        case 0:
            Serial.println("[Info] Idle, ready for command");
            
            COM.println("[Arduino]Idle, ready for command.");
            server = -1;
            break;
        case -1:
            //Serial.println("server = -1");
            //COM.println("TESTING");
            break;
    }
        
    









 
}
