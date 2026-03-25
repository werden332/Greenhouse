#include <stdio.h>
#include <DHT.h> 

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
public:
    Fan(int pin) : pin(pin), is_on(0){}; 
    void set_on(bool condition){ is_on = condition; }
    bool get_fan_condition(){ return is_on; }
    void power(){ digitalWrite(pin, is_on ? HIGH : LOW); }
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

void control_temperature()
{
    if (thermometer.get_temperature() > 40) {
        heater.set_on(0);
        fan.set_on(1);
    }

    if (thermometer.get_temperature() < 20) {
        heater.set_on(1);
        fan.set_on(1);
    }
}

void control_light()
{
    if (light_sensor.get_light() < 500) {
        lump.set_on(1);
    }
    else{
        lump.set_on(0);
    }
}

unsigned long previousMillis = 0;
const long interval = 1000; 
void control_humidity()
{
    if (gigrometer_air.get_humidity() > 70 && gigrometer_soil.get_humidity() > 70) {
        pump.set_on(0);
    }

    if (gigrometer_air.get_humidity() < 30 && gigrometer_soil.get_humidity() < 30) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
         pump.set_on(!pump.get_pump_condition());
        }
    }
}



void loop()
{    
  thermometer.get_temperature();
  gigrometer_air.get_humidity();
  gigrometer_soil.get_humidity();
  light_sensor.get_light();

  control_temperature();
  control_humidity();
  control_light();

  lump.power();
  heater.power();
  pump.power();
  fan.power();
    
    
}
