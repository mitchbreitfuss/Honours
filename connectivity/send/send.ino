#include <SoftwareSerial.h>
#include <Serial.h>
SoftwareSerial COM(5,6);
//connect Serial TX to D5
//connect Serial RX to D6


String stringType = "type=string";
String dataType = "type=float";

String message = "Hello World good day my fine young padawan";

float data = 3.1415;


void setup()
{
    int baud = 9600;
    Serial.begin(baud);
	COM.begin(baud);
    
    
    delay(1000);
    Serial.println("ESP Ready");
    COM.println("ESP Ready");


}

void send(float toSend){
    COM.println(String(toSend) + "$D");
  
    Serial.println(toSend);
}

void send(String toSend){
   
    COM.println(toSend+"$M");
    

    Serial.println(toSend);
}

void loop()
{
 
 data = data+1;
 message = message + 'a';

//send(message);

//delay(50);
Serial.println("hello");
//send(data);

delay(100);
 
}
