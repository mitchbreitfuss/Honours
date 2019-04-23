#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

/*  TODO
    Create a function that calls getData to reduce repeated code.


*/ 

//////////////////////////////////////////
SoftwareSerial COM(D5,D6);
String option1;
String option2;
String option3;

const char* ssid1     = "iiNet23F0A7";
const char* password1 = "GK4EN4NAE695GFC";

const char* ssid2     = "iPhone";
const char* password2 = "cool1234";

const char* host1 = "10.1.1.213";
const char* streamId   = "....................";
const char* privateKey = "....................";

void send(float toSend){
    COM.print(String(toSend) + "$D\n");
    Serial.print("Sending data: ");
    Serial.println(toSend);
}

void send(String toSend){
   
    COM.print(toSend+"$M\n");
    
    Serial.print("Sending message: ");
    Serial.println(toSend);
}

typedef struct{
    int currentPower;
    int energyToday;
    int energyTotal;

}invDataPacket;

// Create struct for the electrolyser
// typedef struct{
//     // Add parameters here


// }elecDataPacket;
// This function scapes the web interface for a data value based on context flags
String getData(String line, String searchFlag, int interface){
    // Interface == 1 corresponds to the inverter
    // Interface == 2 corresponds to the electrolyser 

    String dataValue;
    int sliceStart = line.indexOf(searchFlag);
    int sliceEnd;


    switch (interface){

        case 1: // Solar Inverter
            // Scrapes for Current Power value.
            if(searchFlag == "Power Now:</td><td>"){
                sliceEnd = line.indexOf("W</td></tr><tr");
            }

            // Scrapes for Today's Energy value.
            if(searchFlag == "Today's Energy:</td><td>"){
                sliceEnd = line.indexOf("kWh</td></tr><tr");
            }

            // Scrapes for Total Energy value.
            if(searchFlag == "Total Energy:</td><td>"){
                sliceEnd = line.indexOf("kWh</td></tr><tr class=\"tr2\"");
            }
            break;
        case 2: // Electrolyser
            break;
        default:
            break;
    }

    dataValue = line.substring(sliceStart+searchFlag.length(),sliceEnd);
    
    return dataValue;
}

//////////////////////////////////////////
void getFile(String fileName, String fileType, String hostAdd, int interface){  
    int value;
    bool isHTML;
    String currentPower;
    String energyToday;
    String totalEnergy;
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
    String outfile = "";
    while(client.available()){
    delay(10);
    String line = client.readStringUntil('\n');

    if (fileType == "html-data"){
        if(line.indexOf("<html>")> -1){

            isHTML = true;
            Serial.println("html-start");
            
        }

        if(isHTML == true){

            if(interface == 1){ // If the target is the electrolyser this code will execute.
                //Serial.println(line);

                // These following if statements should be condensed to a function as it is repeated code.
                if(line.indexOf("Power Now:</td><td>")>-1){
                    String searchFlag = "Power Now:</td><td>";
                    currentPower = getData(line,searchFlag,interface);
                    Serial.println();
                    Serial.print("[Info] Current Power = ");
                    Serial.println(currentPower);
                    Serial.println();
                    send("");
                    send("Power Now$L");
                    send(currentPower.toFloat());
                    client.stop();
                }

                if(line.indexOf("Today's Energy:</td><td>")>-1){
                    String searchFlag = "Today's Energy:</td><td>";
                    energyToday = getData(line,searchFlag,interface);
                    Serial.println();
                    Serial.print("[Info] Energy Today = ");
                    Serial.println(energyToday);
                    Serial.println();
                    send("Energy Today$L");
                    send(energyToday.toFloat());
                    client.stop();
                }

                if(line.indexOf("Total Energy:</td><td>")>-1){
                    String searchFlag = "Total Energy:</td><td>";
                    totalEnergy = getData(line,searchFlag,interface);
                    Serial.println();
                    Serial.print("[Info] Total Energy = ");
                    Serial.println(totalEnergy);
                    Serial.println();
                    send("Total Energy$L");
                    send(totalEnergy.toFloat());
                    client.stop();
                }
            }

            //Serial.print(line);
            
            
            //COM.print("[HTML] " + line + "$M\n");
            outfile = outfile + line;

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



