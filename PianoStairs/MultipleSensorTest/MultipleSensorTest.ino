#include <SharpIR.h>
#define NUMSENSORS 8

int pins[] = { 8, 9, 10, 11, 12, 13, 14, 15 };
SharpIR sensors[NUMSENSORS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Initialize Sensors
  
  delay(3000);
  for (int i = 0; i < NUMSENSORS; i++) {
    SharpIR temp_sensor(pins[i], 25, 93, 20150);
    sensors[i] = temp_sensor;
    Serial.print("Initial distance: ");
    Serial.println(temp_sensor.distance());
  }
  Serial.println("\n**************************\n");
  delay(3000);
}

void loop() {
  Serial.println("\n**************************\n");
  for (int i = 0; i < NUMSENSORS; i++) {
    Serial.println(sensors[i].distance());
  }
  delay(3000);  
}



/*
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Initialize Sensors
  
  delay(5000);
  SharpIR temp_sensor(pin, 25, 93, 20150);
  sharp = temp_sensor;
  threshold = temp_sensor.distance() - 4;
  Serial.println("Initial distance: " + temp_sensor.distance());
  
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
    if (sharp.distance() < threshold) {
      Serial.println("Motion detected at" + sharp.distance());      
    }
    else {
      Serial.println("No motion detected");
    }
    delay(1000);
}
*/
