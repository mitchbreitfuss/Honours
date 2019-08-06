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
// const char* ssid1     = "Solax_AA870E88";
// const char* password1 = "";


const char* ssid2     = "iiNet23F0A7";
const char* password2 = "GK4EN4NAE695GFC";

const char* host1 = "10.1.1.213";
const char* host2 = "10.1.1.213";
const char* streamId   = "....................";
const char* privateKey = "....................";


// Search Flags

// Solar

//Electrolyser
const char* FLOW_SEARCHFLAG = "H2&nbsp;Flow";
const char* H2_TOTAL_SEARCHFLAG = "H2&nbsp;Total";
const char* STACK_VOLTAGE_SEARCHFLAG = "Stack Voltage";
const char* PRESSURE_SEARCHFLAG = "Pressure";
const char* TEMPERATURE_SEARCHFLAG = "Temperature";




const char* hostInverter;


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

void sendLog(float toSend, int interface){
    if(interface == 1){
        COM.print(String(toSend) + "$L1\n");
        Serial.print("Sending data: ");
        Serial.println(toSend);
    }
    if(interface == 2){
        COM.print(String(toSend) + "$L2\n");
        Serial.print("Sending data: ");
        Serial.println(toSend);
    }
}

typedef struct{
    float currentPower;
    float energyToday;
    float energyTotal;

}invDataPacket;

// typedef struct{
//     float
// }

// Create struct for the electrolyser
// typedef struct{
//     // Add parameters here


// }elecDataPacket;
// This function scapes the web interface for a data value based on context flags
String getData(String line, String searchFlag, int interface){
    Serial.println("RUNNING getData");
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
            if(searchFlag == FLOW_SEARCHFLAG || H2_TOTAL_SEARCHFLAG || STACK_VOLTAGE_SEARCHFLAG || TEMPERATURE_SEARCHFLAG){
                searchFlag = ""; // For sicing, reduces the length of the searchFlag to zero
                sliceStart = 0; // As the interface for the Electrolyser is formatted correctly, the data is on its own line so requires less slicing
                sliceEnd = line.indexOf(" ");
            }
            
            break;
        default:
            break;
    }

    

    dataValue = line.substring(sliceStart+searchFlag.length(),sliceEnd);
    dataValue.trim();

    return dataValue;
}

//////////////////////////////////////////
void  getFile(String fileName, String fileType, String hostAdd, int interface){  
    int value;
    bool isHTML;

    // Inverter Variable Declarations
    String currentPower;
    String energyToday;
    String totalEnergy;

    // Electrolyser Variable Declarations
    String flow;
    String H2Total;
    String stackVoltage;
    String pressure;
    String waterStatus;
    String temperature;
    String equipmentStatus;

    Serial.print("Connecting to ");
    Serial.println(hostAdd);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 8000;

    if (!client.connect(hostAdd, httpPort)) {
        Serial.println("connection failed");
    return;
    }
    
    delay(500);
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

    client.print(String("GET /")+ fileName + url + " HTTP/1.1\r\n");
    Serial.println(String("Requesting ") + fileName);
    client.print("Authorization: ");
    client.println("Basic YWRtaW46YWRtaW4=");
    client.println();
    delay(2000);
    
    
    
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
    //Serial.println(line);

    if (fileType == "html-data"){
        if(line.indexOf("<html>")> -1){

            isHTML = true;
            Serial.println("html-start");
            
        }

        if(isHTML == true){

            if(interface == 1){ // If the target is the inverter this code will execute.
                //Serial.println(line);

                // These following if statements should be condensed to a function as it is repeated code.
                if(line.indexOf("Power Now:</td><td>")>-1){
                    String searchFlag = "Power Now:</td><td>";
                    currentPower = getData(line,searchFlag,interface);
                    Serial.println();
                    Serial.print("[Info] Current Power = ");
                    Serial.println(currentPower);
                    Serial.println();
                    //sendLog(currentPower.toFloat());
                    client.stop();
                }


                if(line.indexOf("Today's Energy:</td><td>")>-1){
                    String searchFlag = "Today's Energy:</td><td>";
                    energyToday = getData(line,searchFlag,interface);
                    Serial.println();
                    Serial.print("[Info] Energy Today = ");
                    Serial.print(energyToday);
                    Serial.println();
                    
                    //sendLog(energyToday.toFloat());
                    client.stop();
                }

                if(line.indexOf("Total Energy:</td><td>")>-1){
                    String searchFlag = "Total Energy:</td><td>";
                    totalEnergy = getData(line,searchFlag,interface);
                    Serial.println();
                    Serial.print("[Info] Total Energy = ");
                    Serial.print(totalEnergy);
                    Serial.println();
                    //sendLog(totalEnergy.toFloat());
                    client.stop();
                }
            }
            if(interface==2){
                
                if(line.indexOf("nl/h")>-1){
                    String searchFlag = FLOW_SEARCHFLAG;
                    flow = getData(line, searchFlag,interface);
                    Serial.print("[Flow] = ");
                    Serial.println(flow);
                    //sendLog(flow.toFloat());
                    client.stop();

                }
                if(line.indexOf(" l				</p>")>-1){
                    String searchFlag = H2_TOTAL_SEARCHFLAG;
                    H2Total = getData(line,searchFlag,interface);
                    Serial.print("[H2 Total] = ");
                    Serial.println(H2Total);
                    //sendLog(H2Total.toFloat());
                }
                if(line.indexOf(" V				</p>")>-1){
                    String searchFlag = STACK_VOLTAGE_SEARCHFLAG;
                    stackVoltage = getData(line,searchFlag,interface);
                    Serial.print("[Stack Voltage] = ");
                    Serial.println(stackVoltage);

                    //sendLog(stackVoltage.toFloat());
                }
                if(line.indexOf(" bar				</p>")>-1){
                    String searchFlag = PRESSURE_SEARCHFLAG;
                    pressure = getData(line,searchFlag,interface);
                    Serial.print("[Pressure] = ");
                    Serial.println(pressure);
                    //sendLog(pressure.toFloat());
                }
                if (line.indexOf("C				</p>")>-1){
                    String searchFlag = TEMPERATURE_SEARCHFLAG;
                    temperature = getData(line,searchFlag,interface);
                    Serial.print("[Temperature] = ");
                    Serial.println(temperature);
                    //sendLog(temperature.toFloat());
                    }
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
    // if(fileType == "config"){
    //     if(line.indexOf("option1")>-1){
            
    //         line.replace("option1 = ","");
    //         option1 = line;
    //     }

    //     if(line.indexOf("option2")>-1){
        
    //         line.replace("option2 = ","");
    //         option2 = line;
    //     }

    //     if(line.indexOf("option3")>-1){
    //         Serial.println("WORKING");
    //         line.replace("option3 = ","");
    //         option3 = line;
    //     }
    // }

    
    
    Serial.println("closing connection");

    // Send data communications after obtaining all relevent data

    if(interface==1){
        sendLog(101,1);
        delay(200);
        sendLog(currentPower.toFloat(),1);
        delay(200);
        sendLog(energyToday.toFloat(),1);
        delay(200);
        sendLog(totalEnergy.toFloat(),1);
        delay(200);
        sendLog(102,1);

    }
 

    //if(fileType == "config"){
    //Serial.println(option1);
    //Serial.println(option2);
    //Serial.println(option3);
   // }
   
   
}

void serverConnect(int server){
    String serverName = "";
    String ssid;
    String password;
    String fileName;
    String hostAdd;

    if(server == 1){
        serverName = "Inverter";
        ssid = ssid1;
        password = password1;
        fileName = "monitor.html";
        hostAdd = "10.1.1.213";
    }

    if(server == 2){
        serverName = "Electrolyser";
        ssid = ssid2;
        password = password2;
        fileName = "elweb.html";
        hostAdd =  "10.1.1.213";
    }


    
    Serial.println("Connecting to the "+serverName + "...");
    send("Connecting to the "+serverName + "...");
    WiFi.begin(ssid,password);

    while(WiFi.status() != WL_CONNECTED){
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to AP "+ serverName);
    Serial.println("Wifi Connected\n IP Address: " + WiFi.localIP());
    Serial.println("Success! Connected to server " + server);
    getFile(fileName,"html-data",hostAdd,server);

    WiFi.disconnect();
}


void logData(){
    
    serverConnect(1);
    delay(200);
    serverConnect(1);
}



