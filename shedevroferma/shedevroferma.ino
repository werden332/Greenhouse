#include <stdio.h>
#include <DHT.h>


class AirSensor {
private:
  int pin;
  DHT dht;
public:
  float humidity;
  float temperature;
public:
  AirSensor(int pin): dht(pin, DHT11), pin(pin){
    humidity = 0;
    temperature = 0;
  }
public:
  void begin() { dht.begin(); }
public:
  void update() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

    // Проверка: если датчик вернул ошибку (NaN), не обновляем переменные
  if (!isnan(h) && !isnan(t)) {
    humidity = h;
    temperature = t;
    }
  }
};

class HygrometerSoil {
private:
	int pin;
public:
	int humidity;
public:
	HygrometerSoil(int pin) : pin(pin) { humidity = 0; }
public:
	void get_humidity() { humidity = analogRead(pin); }
};

class Light {
private:
	int pin;
public:
	int light;
public:
	Light(int pin) : pin(pin) { light = 0; }
public:
	void get_light() { light = analogRead(pin); }
};



class Lamp {
private:
	int pin;
public:
	bool on_lamp;
public:
	Lamp(int pin) : pin(pin) { 
    on_lamp = false;
    pinMode(pin, OUTPUT);
  }; // присваивает лампе определенный пин и объявляет переменные
public:
	void power();
};

class Fan {
private:
  int pin;
public:
	bool on_fan;
public:
	Fan(int pin) : pin(pin) {
     on_fan = false;
     pinMode(pin, OUTPUT);
  };
public:
	void power();
};

class Heater {
private:
  int pin;
public:
	bool on_heat;
public:
	Heater(int pin) : pin(pin) {
    on_heat = false;
    pinMode(pin, OUTPUT);
  };
public:
	void power();
};

class Pump {
private:
  int pin;
public:
	bool on_pump;
public:
	Pump(int pin) : pin(pin) { 
    on_pump = false;
    pinMode(pin, OUTPUT);
  };
public:
	void power();
};



void Heater::power()
{
	if (on_heat) {digitalWrite(pin, HIGH);}
	else {digitalWrite(pin, LOW);}
}

void Pump::power()
{
	if (on_pump) {digitalWrite(pin, HIGH);}
	else {digitalWrite(pin, LOW);}
}

void Lamp::power()
{
	if (on_lamp) {digitalWrite(pin, HIGH);}
	else {digitalWrite(pin, LOW);}
}

void Fan::power(){
  if (on_fan) {digitalWrite(pin, HIGH);}
	else {digitalWrite(pin, LOW);}
}



void control_temperature(AirSensor &t, Heater &h, Fan &f){
  if(t.temperature > 22 && t.temperature < 27){
    h.on_heat = false;
    f.on_fan = false;
  } 
  else if(t.temperature < 20){
    h.on_heat = true;
    f.on_fan = false;
  }
  else if(t.temperature > 30){
    h.on_heat = false;
    f.on_fan = true;
  }
}

void control_air_humidity(AirSensor &hum, Pump &p, Fan &f){
  if(hum.humidity > 1 && hum.humidity < 1){ //TODO
    p.on_pump = false;
    f.on_fan = false;
  } 
  else if(hum.humidity < 1){
    p.on_pump = true;
    f.on_fan = false;
  }
  else if(hum.humidity > 1){
    p.on_pump = false;
    f.on_fan = true;
  }
}

void control_soil_humidity(HygrometerSoil &hum, Pump &p, Fan &f){
  if(hum.humidity > 990 && hum.humidity < 1040){
    p.on_pump = false;
    f.on_fan = false;
  } 
  else if(hum.humidity <= 990){
    p.on_pump = false;
    f.on_fan = true;
  }
  else if(hum.humidity >= 1040){
    p.on_pump = true;
    f.on_fan = false;
  }
}

void control_light(Light &lig, Lamp&l){
  if(lig.light < 500){
    l.on_lamp = false;
  }
  else{
    l.on_lamp = true;
  }
}


Fan fan(7);
Lamp lamp(6);
Heater heater(4);
Pump pump(5);

AirSensor air(2);
HygrometerSoil soil(A1);
Light light(A0);


void setup(){
  Serial.begin(9600);
  air.begin();
}

unsigned long lastUpdate = 0;

void loop() {
  if(millis() - lastUpdate >= 2000){
    lastUpdate = millis();
    air.update();
  }
  soil.get_humidity();
	light.get_light();

	control_temperature(air, heater, fan);
	control_air_humidity(air, pump, fan);
  control_soil_humidity(soil, pump, fan);
	control_light(light, lamp);

	heater.power();
	fan.power();
	lamp.power();
	pump.power();

if(millis() - lastUpdate >= 2000){
  Serial.print(" влажность почвы "); Serial.print(soil.humidity);
  Serial.print(" влажность воздуха "); Serial.print(air.humidity);
  Serial.print(" температура "); Serial.print(air.temperature);
  Serial.print(" освещенность "); Serial.println(light.light);
  }
}
