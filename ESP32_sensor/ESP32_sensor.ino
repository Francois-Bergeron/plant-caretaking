#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE     DHT22
#define DHTPIN      14



int PIN_LED = 2;
int PIN_Moisture = 35;
int PIN_Light = 15;

const int MoistureMax_Water = 1600;
const int MoistureMin_Air = 3400;
const int Light_Sunny_direct = 200; // Below 200
const int Light_Indirect = 220; // Sunny indirect 200=300
const int LightMin_Night = 4000;


DHT dht(DHTPIN, DHTTYPE);

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

void MQTT_connect();


void setup() {
  Serial.begin(9600);
  // wait for serial monitor to open
  while(! Serial);
  // Connection to WiFi network
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

//  // connect to io.adafruit.com
//  Serial.print("Connecting to Adafruit IO");
//  io.connect();
//  // wait for a connection
//  while(io.status() < AIO_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }
//  // we are connected
//  Serial.println();
//  Serial.println(io.statusText());
  
  // Defining sensor pins  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_Moisture, INPUT);
  pinMode(PIN_Light, INPUT);
  dht.begin();

}

void  loop() {
  MQTT_connect();
  
  // PIN_LED Bliking (Test)
  digitalWrite(PIN_LED, HIGH);
  delay(100);
  digitalWrite(PIN_LED, LOW);

  // Print Statement
  Serial.println(" --------  ");

  // Moisture sensor Reading
  int  analogMoisture = analogRead(PIN_Moisture); 
  Serial.print("Soil Humidity value: ");
  Serial.print(analogMoisture);
  int  Moisture_pct = map(analogMoisture, MoistureMin_Air, MoistureMax_Water, 0, 100);
  if (Moisture_pct>=100){
    Moisture_pct=100;
  }
  else if (Moisture_pct<=0){
    Moisture_pct=0;
  }
  Serial.print("     Soil Moisture: ");
  Serial.print(Moisture_pct);
  Serial.println("%");
  delay(1000);

  // Air Temperature and Humidity
  float air_tempC = dht.readTemperature();
  float air_humid = dht.readHumidity();
  float air_hic = dht.computeHeatIndex(air_tempC, air_humid, false); // Compute heat index in Celsius (isFahreheit = false)
  Serial.print("Air Humidity: ");
  Serial.print(air_humid);
  Serial.print("%     Air Temperature: ");
  Serial.print(air_tempC);
  Serial.print("degC     Heat index: ");
  Serial.print(air_hic);
  Serial.println("degC");
  delay(1000);

  // Light Sensor
  float  light_sensor = analogRead(PIN_Light);
  Serial.print("Light value: ");
  Serial.println(light_sensor);

  //Sending data to Adafruit IO
  temperature.publish(air_tempC);
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
 
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}
