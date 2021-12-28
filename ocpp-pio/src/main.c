#include <Arduino.h>

void setup() {
	pinMode(PA0, OUTPUT);
}

void loop() {
	digitalWrite(PA0, HIGH); 
	delay(1000);
	digitalWrite(PA0, LOW);
	delay(1000);
}