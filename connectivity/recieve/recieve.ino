#include <SoftwareSerial.h>
#include <Serial.h>
SoftwareSerial COM(5,6);
// connect Serial TX to D5
// connect Serial RX to D6

String stringType = "type=string";
String dataType = "type=float";
String messageFlag = "$M";



void setup()
{
    Serial.begin(115200);
	COM.begin(115200);
    COM.println("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    Serial.println("Nano Ready");


}
String recieved;

void loop()
{     
    if(COM.available()){ 

        String recieved = COM.readStringUntil('\n');

        if(recieved.indexOf("$D")>0){ //Checks type of recieved message
            recieved.replace("$D","");
            recieved = recieved.toFloat();
            
        } 

        if(recieved.indexOf("$M")>0){
            recieved.replace("$M","");

        }
        
        
    if(recieved != "\n"){  
        Serial.println(recieved);
    }
    
    }

    
delay(200);
}