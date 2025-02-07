#include <Arduino.h>

#define PWM_5510_GCS 10
#define PID_5510_V 15

uint32_t frequency = 1000;
uint32_t dutyCycle = 128;  // 50% duty cycle (out of 255)

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  
  pinMode(PWM_5510_GCS, OUTPUT);
  pinMode(PID_5510_V, INPUT);
  
  analogWriteFrequency(PWM_5510_GCS, frequency);
  analogWrite(PWM_5510_GCS, dutyCycle);
}

void loop() {
  if (Serial.available() > 0) {
    frequency = Serial.parseInt();  // Read full integer value
    if (frequency > 0) {  // Ignore invalid values
      //frequency = newFrequency;
      analogWriteFrequency(PWM_5510_GCS, frequency);
      
      Serial.print("Received: ");
      Serial.println(frequency);
    }
  } else {
    Serial.print("Read: ");
    float voltage = analogRead(PID_5510_V) * (3 / 1023.0);
    Serial.println(voltage);
  }
}

