#include <SoftwareSerial.h>

SoftwareSerial COM(D5,D6);
//connect Serial TX to D5
//connect Serial RX to D6


String stringType = "type=string";
String dataType = "type=float";

String message = "Hello World good day my fine young padawan";

float data = 3.1415;


void setup()
{
    int baud = 115200;
    Serial.begin(baud);
	COM.begin(baud);
    
    
    delay(1000);
    Serial.println("ESP Ready");
    COM.println("ESP Ready");


}

void send(float toSend){
    COM.print(String(toSend) + "$D");
  
    Serial.println(toSend);
}

void send(String toSend){
   
    COM.print(toSend+"$M");
    

    Serial.println(toSend);
}

void loop()
{
 
 data = data+1;
 message = message + 'a';

//send(message);

//delay(50);

send(data);

delay(100);
 
}
