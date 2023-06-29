#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ThingSpeak.h>

LiquidCrystal_I2C lcd(0x27,20,4);

//const char* ssid = "$$J12345";
//const char* password = "87651234";

const char* ssid = "RB";
const char* password = "rb2580sb";

unsigned long channelID = 2073466;
const char* writeAPIKey = "KNDEBPA3O9C21E4E";

const int t_sensorPin = 34;
const int p_sensorPin = 35;
const int alarmPin=23;
const int tempBus=4;


OneWire oneWire(tempBus);
DallasTemperature sensors(&oneWire);

WiFiClient client;


String server = "http://maker.ifttt.com";
String eventName = "data_storage";
String IFTTT_Key = "d07Ncw_kdo8bY32qXwfBuz";
String IFTTTUrl="https://maker.ifttt.com/trigger/data_storage/json/with/key/d07Ncw_kdo8bY32qXwfBuz";


//int a=0,b=1;
float Temp=0, Turbidity=0,pH=7;
float t_volt,p_volt;
float p_value=0;


void setup() {
  Serial.begin(115200);
  sensors.begin();
  delay(1000);
  pinMode (23,OUTPUT);
  pinMode(p_value, INPUT);
  
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("WiFi connected");
  lcd.clear();
  lcd.begin(20,4);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.print("Hello, World!");
  
  ThingSpeak.begin(client);
}

void loop() {
  
  int readings = 10;
  float value = 0;
  Turbidity =0;
  t_volt = 0;
  p_volt = 0;
  
  //float turbidityValue= analogRead (sensorPin);Turbidity=turbidityValue*(5.0/1024.0)*3; Number of readings to average
  //for (int i = 0; i < readings; i++) {
    //value += analogRead(sensorPin);
    //Turbidity=(value/(i+1))*(5.0/1024.0)*3;
    //Serial.println(Turbidity);
    //delay(10);
  //}
  //value = analogRead(sensorPin);
  //Turbidity=value*(5.0/1024.0);
    
    for(int i=0; i<100; i++)
    {
        t_volt += (float)analogRead(t_sensorPin);
    }
    t_volt = t_volt/100;
    Turbidity=t_volt*(5.0/1024.0);

    for(int i=0; i<100; i++)
    {
        p_volt += (float)analogRead(p_sensorPin);
    }
  p_volt = p_volt/100;
  p_value = p_volt*3.3/4095;
  pH=-5.70*p_value + 20 ;


  sensors.requestTemperatures(); 
  Temp = sensors.getTempCByIndex(0);
  
  Serial.print("    Turbidity :-  ");
  Serial.println(Turbidity);
  Serial.print("    pH :-  ");
  Serial.println(pH);
  Serial.print("    Temperature :-  ");
  Serial.print(Temp);
  Serial.println("ºC");

  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Turbidity :-  ");
  lcd.print(Turbidity);
  lcd.setCursor(0, 1);      
  lcd.print("pH :-  ");
  lcd.print(pH);
  lcd.setCursor(0, 2);      
  lcd.print("Temp :-  ");
  lcd.print(Temp);
  
  
  ThingSpeak.setField(1, Temp);
  ThingSpeak.setField(2, Turbidity);
  ThingSpeak.setField(3, pH);

  ThingSpeak.writeFields(channelID, writeAPIKey);
  sendDataToSheet();

  if( Temp<24 || Temp>30 || Turbidity <6 || pH <6 || pH>9)
  {
    digitalWrite(alarmPin,HIGH);
   }
   else
   {
    digitalWrite(alarmPin,LOW);
   }
  
  delay(2500); 
  lcd.clear();
}



void sendDataToSheet(void)
{
  String url = server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + String((float)Temp) + "&value2="+String((float)Turbidity) +"&value3=" + String((float)pH);  
  Serial.println(url);
 
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin(url); //HTTP

  Serial.print("[HTTP] GET...\n");
  
  int httpCode = http.GET();

  if(httpCode > 0) {
    
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

}
/*
float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}*/
