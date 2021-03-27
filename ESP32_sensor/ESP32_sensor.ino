#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT22

#define DHTPIN 14
int PIN_LED = 2;
int PIN_Moisture = 35;
int PIN_Light = 15;

const int MoistureMax_Water = 1600;
const int MoistureMin_Air = 3400;
const int Light_Sunny_direct = 200; // Below 200
const int Light_Indirect = 220; // Sunny indirect 200=300
const int LightMin_Night = 4000;



DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_Moisture, INPUT);
  pinMode(PIN_Light, INPUT);
  dht.begin();
}

void  loop() {
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
}




