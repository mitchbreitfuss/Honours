#include <Arduino.h>

/*  TODO
    Create a function that calls getData to reduce repeated code.


*/ 

//////////////////////////////////////////

typedef struct{
    int currentPower;
    int energyToday;
    int energyTotal;

}invDataPacket;

// Create struct for the electrolyser
// typedef struct{
//     // Add parameters here


// }elecDataPacket;


//////////////////////////////////////////
void getFile(String fileName, String fileType, String hostAdd, int interface){  
    int value;
    bool isHTML;
    String currentPower;
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
    //delay(10);
    String line = client.readStringUntil('\n');

    if(fileType == "html-data"){
        if(line.indexOf("<html>")> -1){

            isHTML = true;
            Serial.println("html-start");
            
        }

        if(isHTML == true){

            if(interface == 1){ // If the target is the electrolyser this code will execute.

                // These following if statements should be condensed to a function as it is repeated code.
                if(line.indexOf("Power Now:</td><td>")>-1){
                    String searchFlag = "Power Now:</td><td>";
                    currentPower = getData(line,searchFlag,interface);
                    Serial.println();
                    Serial.println("[Info] Current Power = ");
                    Serial.println(currentPower);
                    client.stop();

                }

                if(line.indexOf("Today's Energy:</td><td>")>-1){
                    String searchFlag = "Today's Energy:</td><td>";
                    currentPower = getData(line,searchFlag,interface);
                    Serial.println();
                    Serial.println("[Info] Today's Power = ");
                    Serial.print(currentPower);
                    Serial.println();
                    client.stop();

                }
            }

            //Serial.print(line);
            
            
            COM.print("[HTML] " + line + "$M\n");
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
                sliceEnd = line.indexOf("kWh</td></tr><tr");
            }
            break;
        case 2: // Electrolyser
            break;
        default:
            break;
    }

    dataValue = line.substring(sliceStart+searchFlag.length(),line.indexOf(sliceEnd));

    return dataValue;
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
