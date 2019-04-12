void setup()
{
	Serial.begin(9600);
    
}
String var;
void loop()
{
	if(Serial.available()>0){
        var = Serial.readStringUntil('\n');
        

    }
            //Serial.println(var);
        delay(3000);

        Serial.println("test");
}
