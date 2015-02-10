#include <SharpIR.h>

int pin = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Initialize Sensors
  
  //delay(3000);
  //SharpIR temp_sensor(pin, 25, 93, 20150);
  //sharp = temp_sensor;
  //Serial.println("Initial distance: " + temp_sensor.distance());
  //analogReference(DEFAULT);
  
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
    SharpIR sharp1(1, 25, 93, 20150);
    Serial.println(sharp1.distance());
    SharpIR sharp2(2, 25, 93, 20150);
    Serial.println(sharp2.distance());
    
    //int raw = analogRead(A0);
    //float voltFromRaw=map(raw, 0, 1023, 0, 5000);
    
    delay(500);
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
