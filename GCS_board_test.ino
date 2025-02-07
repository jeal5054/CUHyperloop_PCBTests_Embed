/********************************************
        GCS REV : A - PUMP CONTROL TESTS
            author: JEFFREY ALLEN
               2025 - 01 - 23
********************************************/
#include <stdio.h>
#include <Arduino.h>
// PIN DEFINITIONS 
//      **5510 and 5513 refer to the Aquatec pumps we origionally used, changed
#define PID_5510_PIN 15
#define PWM_5510_PIN 16

#define PID_5513_PIN 40
#define PWM_5513_PIN 41

#define KAYTOLA_FL_PIN 24
#define MACNAUGHT_FL_PIN 25

#define ESTOP_PIN 27

const uint32_t maxFrequency = 1 * 1000000; // 1 MHz
const uint32_t dutyCycle = 128;  // 50% duty cycle (128 out of 255), probably don't change this

uint32_t PWM_5510_FREQ = 1000;
uint32_t PWM_5513_FREQ = 1000;


void setup() {
  Serial.begin(9600);
  // ANALOG READS
  pinMode(PID_5510_PIN, INPUT);
  pinMode(PID_5513_PIN, INPUT);

  pinMode(KAYTOLA_FL_PIN, INPUT);
  pinMode(MACNAUGHT_FL_PIN, INPUT);

  analogWrite(PWM_5510_PIN, dutyCycle);
  analogWrite(PWM_5513_PIN, dutyCycle);
  uint32_t freq = 1000;
  PUMP_5510_TEST(freq);
  PUMP_5513_TEST(freq);
}

void PUMP_5510_TEST(uint32_t frequency) {
  analogWriteFrequency(PWM_5510_PIN, frequency);
  delay(100);
}

void PUMP_5513_TEST(uint32_t frequency) {
  analogWriteFrequency(PWM_5513_PIN, frequency);
  delay(100);
}

void analogRead_Pump(void) {
  float pump1Value = analogRead(PID_5510_PIN) * (3 / 1023.0); // Read analog value from pump 1
  float pump2Value = analogRead(PID_5513_PIN) * (3 / 1023.0); // Read analog value from pump 2

  // Print data for Serial Plotter
  Serial.print("Pump1: ");
  Serial.print(pump1Value);
  Serial.print(", Pump2: ");
  Serial.println(pump2Value);
}

void loop() {
  // Monitor analog pump values
  analogRead_Pump();
  uint32_t freq = 1000;
  PUMP_5510_TEST(freq);
  PUMP_5513_TEST(freq);
  
  // Example: Sweep frequencies from 1 kHz to 10 kHz in 100 Hz increments
  for (uint32_t freq = 1000; freq <= 10000; freq += 100) {
    Serial.print("Testing frequency: ");
    Serial.println(freq);
    
    // Test pumps at the current frequency
    PUMP_5510_TEST(freq);
    PUMP_5513_TEST(freq);
  }

  // Add a delay to avoid spamming tests
  delay(1000);  // Wait 1 second before starting the next sweep
}



