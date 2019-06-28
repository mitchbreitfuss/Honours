#include <ESP8266WiFi.h>

const char* ssid     = "iiNet23F0A7";
const char* password = "GK4EN4NAE695GFC";

const char* host = "10.1.1.213";
const char* streamId   = "....................";
const char* privateKey = "....................";


String option1;
String option2;
String option3;
String file;

bool isHTML;


int value = 0;

void getFile(String fileName, String filePath, String fileType){  
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8000;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.println(String("GET ") + filePath + " HTTP/1.1");

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
  client.print(String("GET ")+ filePath + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
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
  if(fileType == "config"){
    Serial.println(option1);
    Serial.println(option2);
    Serial.println(option3);
  }
}






void setup() {
  Serial.begin(115200);
  delay(10);


  
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  getFile("config","/config","config");
}
int val = 1;

void  loop(){

    if(val == 1){
      getFile("sampledata.html","sampledata.html","html-data");

    }
    val+=1;
  delay(200);
  delay(10000);
     
	
}