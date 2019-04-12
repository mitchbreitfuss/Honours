#include <SoftwareSerial.h>

SoftwareSerial COM(D5,D6);
//connect Serial TX to D5
//connect Serial RX to D6

SoftwareSerial COM2(D7,D8);


String stringType = "type=string";
String dataType = "type=float";

String message = "Hello World good day my fine young padawan";
String recieved;
float data = 3.1415;
int recieve = 0;

void setup()
{
    int baud = 9600;
    Serial.begin(baud);
	COM.begin(baud);
    COM2.begin(baud);
    
    
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


 
    COM2.listen();
    while(COM2.available() > 0){
        String in = COM2.readStringUntil('\n');
        Serial.println(in);
    }


 









 
}
