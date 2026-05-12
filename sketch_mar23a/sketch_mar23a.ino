#include <stdio.h>
#include <DHT.h> 

unsigned long startMillis;
int startSeconds = 0;
int startMinutes = 0;
int startHours = 0;
bool timeSet = false;

// Настройки
int heat_sensor_temperature_on = 20;    // температура ниже которой включается обогреватель
int heat_sensor_temperature_off = 40;   // температура выше которой выключается обогреватель
int gigrometer_air_percent_off = 70;    // влажность воздуха выше которой выключается помпа
int gigrometer_soil_percent_off = 70;   // влажность почвы выше которой выключается помпа
int gigrometer_air_percent_on = 30;     // влажность воздуха ниже которой включается помпа
int gigrometer_soil_percent_on = 30;    // влажность почвы ниже которой включается помпа
int time_morning = 8;                   // начало дня (лампа и вентилятор работают)
int time_night = 20;                    // начало ночи
const long interval_aired = 15000;      // время проветривания
int aired_interval = 6;                 // интервал проветривания

class Thermometer {
private:
    int pin;
    float temperature;
    DHT dht;
public:    
    Thermometer(int pin) : pin(pin), temperature(0), dht(pin, DHT11) { 
      dht.begin();
    }  
    float get_temperature() { 
      temperature = dht.readTemperature(); 
      return temperature;
    }  
};

class GigrometerAir {
private:
    int pin;
    float humidity;
    DHT dht;
public:    
    GigrometerAir(int pin) : pin(pin), humidity(0), dht(pin, DHT11) { 
      dht.begin();
    } 
    float get_humidity() { 
      humidity = dht.readHumidity(); 
      return humidity;
    }  
};

class GigrometerSoil {
private:
    int pin;
    float humidity;
public:    
    GigrometerSoil(int pin) : pin(pin), humidity(0) {}
    float get_humidity() { 
      humidity = analogRead(pin); 
      return humidity;
    }  
};

class LightSensor {
private:
    int pin;
    int light;
public:    
    LightSensor(int pin) : pin(pin){} 
    float get_light() { 
      light = analogRead(pin); 
      return light;
    }  
};

class Heater {
private:
    int pin;
    bool is_on;
public:
    Heater(int pin) : pin(pin), is_on(0){}; 
    void set_on(bool condition){ is_on = condition; }
    bool get_heater_condition(){ return is_on; }
    void power(){ digitalWrite(pin, is_on ? HIGH : LOW); }
};

class Fan {
private:
    int pin;
    bool is_on;
    bool is_aired_flag;
public:
    Fan(int pin) : pin(pin), is_on(0){}; 
    void set_on(bool condition){ is_on = condition; }
    bool get_fan_condition(){ return is_on; }
    void power(){ digitalWrite(pin, is_on ? HIGH : LOW); }
    void set_aired(bool condition){ is_aired_flag = condition; }
    bool is_aired(){ return is_aired_flag; }
};

class Pump {
private:
    int pin;
    bool is_on;
public:
    Pump(int pin) : pin(pin), is_on(0){}; 
    void set_on(bool condition){ is_on = condition; }
    bool get_pump_condition(){ return is_on; }
    void power(){ digitalWrite(pin, is_on ? HIGH : LOW); }
};

class Lump {
private:
    int pin;
    bool is_on;
public:
    Lump(int pin) : pin(pin), is_on(0){}; 
    void set_on(bool condition){ is_on = condition; }
    bool get_lump_condition(){ return is_on; }
    void power(){ digitalWrite(pin, is_on ? HIGH : LOW); }
};

int humid_temp_pin = 2; 
int humid_soil_pin = A1;
int light_sensor_pin = A0;

int fan_pin = 7; 
int heater_pin = 4; 
int lump_pin = 6; 
int pump_pin = 5; 

void setup() {             
  Serial.begin(9600); 
  pinMode(light_sensor_pin, INPUT);
  pinMode(humid_soil_pin, INPUT);
  pinMode(fan_pin, OUTPUT);
  pinMode(heater_pin, OUTPUT);
  pinMode(lump_pin, OUTPUT);
  pinMode(pump_pin, OUTPUT);
}

Thermometer thermometer(humid_temp_pin);
GigrometerAir gigrometer_air(humid_temp_pin);
GigrometerSoil gigrometer_soil(humid_soil_pin);
LightSensor light_sensor(light_sensor_pin);

Fan fan(fan_pin);
Heater heater(heater_pin);
Pump pump(pump_pin);
Lump lump(lump_pin);

int get_current_hour() {
    if (!timeSet) return 12; // если время не установлено, считаем что день
    
    unsigned long elapsed = millis() - startMillis;
    unsigned long totalSeconds = elapsed / 1000;
    int currentHours = (startHours + (startMinutes + (startSeconds + totalSeconds) / 60) / 60) % 24;
    return currentHours;
}

unsigned long previousMillis_aired = 0;
void control_humidity_air(){
    int currentHour = get_current_hour();
    if ((currentHour-startHours) % aired_interval == 0 && !fan.is_aired()){
        unsigned long currentMillis_aired = millis();
        if (currentMillis_aired - previousMillis_aired >= interval_aired) {
            previousMillis_aired = currentMillis_aired;
            fan.set_on(1);
        }
        else{
            previousMillis_aired = 0;
            fan.set_aired(1);
        }
    }
    else {
        fan.set_aired(0);
    }
}

void control_temperature()
{
    int currentHour = get_current_hour();
    bool isDaytime = (currentHour >= time_morning && currentHour < time_night);
    
    if (thermometer.get_temperature() > heat_sensor_temperature_off) {
        heater.set_on(0);
        if (isDaytime) {
            fan.set_on(1);
        }
    }

    if (thermometer.get_temperature() < heat_sensor_temperature_on) {
        heater.set_on(1);
        if (isDaytime) {
            fan.set_on(1);
        }
    }
    else if (fan.is_aired()){
        fan.set_on(0);
    }
}

void control_light()
{
    int currentHour = get_current_hour();
    
    if (light_sensor.get_light() < 500 && currentHour >= time_morning && currentHour < time_night) {
        lump.set_on(1);
    }
    else {
        lump.set_on(0);
    }
}

unsigned long previousMillis_soil = 0;
const long interval_soil = 1000; 

void control_humidity_soil()
{
    float airHumidity = gigrometer_air.get_humidity();
    float soilHumidity = (gigrometer_soil.get_humidity() / 1023.0) * 100;
    
    if (soilHumidity > gigrometer_soil_percent_off) {
        pump.set_on(0);
    }

    if (soilHumidity < gigrometer_soil_percent_on) {
        unsigned long currentMillis_soil = millis();
        if (currentMillis_soil - previousMillis_soil >= interval_soil) {
            previousMillis_soil = currentMillis_soil;
            pump.set_on(!pump.get_pump_condition());
        }
    }
}
unsigned long previousMillis_print = 0;
const long interval_print = 1000;

void print_data(){
    unsigned long currentMillis_print = millis();
    if (currentMillis_print - previousMillis_print >= interval_print) {
        previousMillis_print = currentMillis_print;
        Serial.print("Температура: ");
        Serial.print(thermometer.get_temperature());
        Serial.println(" °C");
        Serial.print("Влажность почвы: ");
        Serial.print((gigrometer_soil.get_humidity() / 1023.0) * 100);
        Serial.println(" %");
        Serial.print("Влажность воздуха: ");
        Serial.print(gigrometer_air.get_humidity());
        Serial.println(" %");
        if (timeSet){
            Serial.print("Текущее время: ");
            int currentHour = get_current_hour();
            Serial.println(currentHour);
        }
        Serial.println("---");
    }
}
void printTime() {
  if (startHours < 10) Serial.print("0");
  Serial.print(startHours);
  Serial.print(":");
  if (startMinutes < 10) Serial.print("0");
  Serial.print(startMinutes);
  Serial.print(":");
  if (startSeconds < 10) Serial.print("0");
  Serial.println(startSeconds);
}

void time_cycle(){
    if (!timeSet) {
        if (Serial.available()) {
            String input = Serial.readStringUntil('\n');
            input.trim();

            int h = input.substring(0, 2).toInt();
            int m = input.substring(3, 5).toInt();
            int s = input.substring(6, 8).toInt();
            
            if (h >= 0 && h <= 23 && m >= 0 && m <= 59 && s >= 0 && s <= 59) {
                startHours = h;
                startMinutes = m;
                startSeconds = s;
                startMillis = millis();
                timeSet = true;
                Serial.print("Время установлено: ");
                printTime();
            } else {
                Serial.println("Неверный формат! Попробуйте ещё раз.");
            }
        }
    }
}

void loop()
{    
    time_cycle(); 

    control_temperature();
    control_humidity_soil();
    control_humidity_air();
    control_light();
    

    lump.power();
    heater.power();
    pump.power();
    fan.power();

    print_data();
}