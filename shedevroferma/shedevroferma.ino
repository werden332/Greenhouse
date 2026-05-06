#include <stdio.h>
#include <DHT.h>

class Thermometer {
private:
	int pin;
public:
	float temperature; //celcius
  DHT dht;
public:
	Thermometer(int pin) : dht(pin, DHT11), pin(pin) { temperature = 0; } // конструктор, присваивает переменной начальное значение
public:
	void get_temperature() { temperature = dht.readTemperature(); }
};

class HygrometerAir {
private:
	int pin;
public:
	int humidity;
  DHT dht;
public:
	HygrometerAir(int pin) : dht(pin, DHT11), pin(pin) { humidity = 0; }
public:
	void get_humidity() { humidity = dht.readHumidity(); }
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



void control_temperature(Thermometer &t, Heater &h, Fan &f){
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

void control_air_humidity(HygrometerAir &hum, Pump &p, Fan&f){
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

void control_soil_humidity(HygrometerSoil &hum, Pump &p, Fan&f){
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
  if(lig.light < 500){ //TODO границы
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

//Thermometer thermometer();
//HygrometerAir airHumidity();
HygrometerSoil soilHumidity(A1);
Light light(A0);


void setup(){
  Serial.begin(9600);
}

void loop() {
		//thermometer.get_temperature();
		//airHumidity.get_humidity();
    soilHumidity.get_humidity();
		light.get_light();

		//control_temperature(thermometer, heater, fan);
		//control_air_humidity(airHumidity, pump, fan);
    control_soil_humidity(soilHumidity, pump, fan);
		control_light(light, lamp);

		heater.power();
		fan.power();
		lamp.power();
		pump.power();

    Serial.print(" влажность почвы "); Serial.print(soilHumidity.humidity);
    //Serial.print(" влажность воздуха "); Serial.print(airHumidity.humidity);
    //Serial.print(" температура "); Serial.print(thermometer.temperature);
    Serial.print(" освещенность "); Serial.println(light.light);
}#include <stdio.h>


class Thermometer {
public:
	int temperature; //celcius
public:
	Thermometer() { temperature = 0; } // конструктор, присваивает переменной начальное значение
public:
	void get_temperature() { temperature = analogRead(/**/); }
};

class Humidity {
public:
	int humidity;
public:
	Humidity() { humidity = 0; }
public:
	void get_humidity() { humidity = analogRead(/*TODO: PIN*/); }
};

class Light {
public:
	int light;
public:
	Light() { light = 0; }
public:
	void get_light() { light = analogRead(/**/); }
};



class Lamp {
private:
	int pin;
public:
	bool on_lamp;
public:
	Lamp(int pin) : pin(pin) { on_lamp = false; }; // присваивает лампе определенный пин и объявляет переменные
public:
	void power();
};

class Fan {
private:
  int pin;
public:
	bool on_fan;
public:
	Fan(int pin) : pin(pin) { on_fan = false; };
public:
	void power();
};

class Heater {
private:
  int pin;
public:
	bool on_heat;
public:
	Heater(int pin) : pin(pin) { on_heat = false; };
public:
	void power();
};

class Pump {
private:
  int pin;
public:
	bool on_pump;
public:
	Pump(int pin) : pin(pin) { on_pump = false; };
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



void control_temperature(Thermometer &t, Heater &h, Fan &f){
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

void control_air_humidity(Humidity &hum, Pump &p, Fan&f){
  if(hum.humidity > /**/ && hum.humidity < /**/){
    p.on_pump = false;
    f.on_fan = false;
  } 
  else if(hum.humidity < /**/){
    p.on_pump = true;
    f.on_fan = false;
  }
  else if(hum.humidity > /**/){
    p.on_pump = false;
    f.on_fan = true;
  }
}

void control_soil_humidity(Humidity &hum, Pump &p, Fan&f){
  if(hum.humidity > /**/ && hum.humidity < /**/){
    p.on_pump = false;
    f.on_fan = false;
  } 
  else if(hum.humidity < /**/){
    p.on_pump = true;
    f.on_fan = false;
  }
  else if(hum.humidity > /**/){
    p.on_pump = false;
    f.on_fan = true;
  }
}

void control_light(Light &lig, Lamp&l){
  if(lig.light < /**/){ //TODO границы
    l.on_lamp = true;
  }
  else{
    l.on_lamp = false;
  }
}


Fan fan();
Lamp lamp();
Heater heater(); // TODO PIN
Pump pump();

Thermometer thermometer;
Humidity airHumidity;
Humidity soilHumidity;
Light light;


void setup(){

}

void loop() {
		thermometer.get_temperature();
		airHumidity.get_humidity();
    soilHumidity.get_humidity();
		light.get_light();

		control_temperature(thermometer, heater, fan);
		control_air_humidity(airHumidity, pump, fan);
    control_soil_humidity(soilHumidity, pump, fan);
		control_light(light, lamp);

		heater.power();
		fan.power();
		lamp.power(); // TODO PIN
		pump.power();
}
