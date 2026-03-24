#include <DHT.h> 
int humid_temp = 2; 
int fan = 7; 
int humid_soil = A1; 
int heater = 4; 
int light = 6; 
int pump = 5; 
int light_data = A0;

DHT dht(humid_temp, DHT11);
unsigned long previousMillis = 0;
void setup() {             
  Serial.begin(9600);
  dht.begin(); 
  pinMode(fan, OUTPUT);
  pinMode(heater, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(pump, OUTPUT);


}

void loop() {
  Serial.print("Humidity air: "); 
  Serial.println(dht.readHumidity());
  Serial.print("Temperature air: ");
  Serial.println(dht.readTemperature());
  Serial.print("Humidity soil: ");
  Serial.println(analogRead(humid_soil));
  Serial.print("Light sensor: ");
  Serial.println(analogRead(light_data));
  
}
