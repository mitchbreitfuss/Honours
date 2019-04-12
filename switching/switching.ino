#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>


const char* ssid1     = "Breitfuss";
const char* password1 = "mitchell";

const char* ssid2     = "iPhone";
const char* password2 = "cool1234";

const char* host1 = "10.1.1.213";
const char* streamId   = "....................";
const char* privateKey = "....................";






SoftwareSerial COM(D5,D6);
//connect Serial TX to D5
//connect Serial RX to D6


String file;
bool isHTML;

String stringType = "type=string";
String dataType = "type=float";

String message = "Hello World good day my fine young padawan";
String recieved;
float data = 3.1415;
int recieve = 0;

String option1;
String option2;
String option3;

void setup()
{
    delay(1000);
    int baud = 115200;
    Serial.begin(baud);
	COM.begin(baud);

    
    delay(2000);
    Serial.println("ESP Ready");
    COM.println("ESP Ready");


}

void getFile(String fileName, String fileType, String hostAdd){  
    int value;
    Serial.print("Connecting to ");
    Serial.println(hostAdd);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 8000;

    if (!client.connect(hostAdd, httpPort)) {
        Serial.println("connection failed");
    return;
    }
    client.println(String("GET /") + fileName + " HTTP/1.1");

    // We now create a URI for the request
    String url = "/input/";
    url += streamId;
    url += "?private_key=";
    url += privateKey;
    url += "&value=";
    url += value;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET /")+ fileName + url + " HTTP/1.1\r\n" +
                "Host: " + hostAdd + "\r\n" + 
                "Connection: close\r\n\r\n");
    Serial.println(String("Requesting ") + fileName);
    unsigned long timeout = millis();
    while (client.available() == 0) {
    if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
    }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
    delay(10);
    String line = client.readStringUntil('\n');

    if(fileType == "html-data"){
        if(line.indexOf("<html>")> -1){

            isHTML = true;
            Serial.println("html-start");
            
        }

        if(isHTML == true){
            
            Serial.print(line);
            COM.print("[HTML] " + line + "$M\n");
        }
        if(line.indexOf("</html>")>-1){
            isHTML = false;
            //Serial.println("html-start");
            //Serial.print(file);
            Serial.println();
            Serial.println("html-stop");
            client.stop();
        }
        
        
        
    }
        if(fileType == "config"){
        if(line.indexOf("option1")>-1){
            
            line.replace("option1 = ","");
            option1 = line;
        }

        if(line.indexOf("option2")>-1){
        
            line.replace("option2 = ","");
            option2 = line;
        }

        if(line.indexOf("option3")>-1){
            Serial.println("WORKING");
            line.replace("option3 = ","");
            option3 = line;
        }
    }

    }
    Serial.println("closing connection");
    //if(fileType == "config"){
    //Serial.println(option1);
    //Serial.println(option2);
    //Serial.println(option3);
   // }
}


void send(float toSend){
    COM.print(String(toSend) + "$D");
  
    Serial.println(toSend);
}

void send(String toSend){
   
    COM.print(toSend+"$M");
    
    Serial.print("Sending message: ");
    Serial.println(toSend);
}

int server = 0;
float toSend = 3.14;
void loop(){


    if(COM.available()){
        String recieved = COM.readStringUntil('\n');
        
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
    toSend = toSend + 0.01;
    COM.print(String(toSend)+"$D\n");
    

    switch (server){
        case 1:
            WiFi.begin(ssid1,password1);

            while(WiFi.status() != WL_CONNECTED){
                delay(100);
                Serial.print(".");

            }
            Serial.println();
            Serial.println("Connected to AP 1");
            Serial.println("Wifi Connected\n IP Address: " + WiFi.localIP());
            Serial.println("Success! Connected to server " + server);
            getFile("elweb.html","html-data","10.0.0.100");

            server = 0;
            WiFi.disconnect();
            break;
    
        case 2:
            WiFi.begin(ssid2,password2);

            while(WiFi.status() != WL_CONNECTED){
                Serial.print('.');
                delay(300);
            }
            Serial.println("Connected to AP 2");
            Serial.print("\nWifi Connected\n IP Address: " + WiFi.localIP());
            Serial.print("\nWifi");

            WiFi.disconnect();
            server = 0;
            
        case 0:
            Serial.println("[Info] Idle, ready for command");
            server = -1;
            break;
        case -1:
            break;
    }
        
    









 
}
