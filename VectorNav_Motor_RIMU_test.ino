#include <stdio.h>
#include <Arduino.h>
#include <elapsedMillis.h>

#define MOTOR_DIR_0 1
#define MOTOR_STEP_0 2

#define MOTOR_DIR_1 3
#define MOTOR_STEP_1 4

#define MOTOR_DIR_2 9
#define MOTOR_STEP_2 10

#define VECTORNAV0 Serial5 // RX -> 21, TX -> 20
#define VECTORNAV1 Serial3 // RX -> 15, TX -> 14
#define VECTORNAV2 Serial8 // RX -> 34, TX -> 35

uint8_t direction = 1;
uint32_t frequency = 1000;
uint32_t dutyCycle = 128;  // 50% duty cycle (128 out of 255)

// ======== timing ========
const int loopTime = 10; // Defined void loop function time [ms]
elapsedMillis loopRate;   // Variable to track loop time

void motors_INIT() {
  // Motor 0
  analogWriteFrequency(MOTOR_STEP_0, frequency);
  analogWrite(MOTOR_STEP_0, dutyCycle);
  pinMode(MOTOR_DIR_0, OUTPUT);
  digitalWrite(MOTOR_DIR_0, direction);
  analogWrite(MOTOR_STEP_0, 128);
  // Motor 1
  analogWriteFrequency(MOTOR_STEP_1, frequency);
  analogWrite(MOTOR_STEP_1, dutyCycle);
  pinMode(MOTOR_DIR_1, OUTPUT);
  digitalWrite(MOTOR_DIR_1, direction);
  analogWrite(MOTOR_STEP_1, 128);
  // Motor 2
  analogWriteFrequency(MOTOR_STEP_2, frequency);
  analogWrite(MOTOR_STEP_2, dutyCycle);
  pinMode(MOTOR_DIR_2, OUTPUT);
  digitalWrite(MOTOR_DIR_2, direction);
  analogWrite(MOTOR_STEP_2, 128);
}

void VectorNav_INIT() {
  // VectorNav 0
  VECTORNAV0.begin(9600);
  VECTORNAV0.print("$VNWRG,05,9600*58"); // This should be configuring the imu itself
  const char *configCommand = "$VNWRG,07,1*XX\r\n"; // Replace with VN-100 command for enabling acceleration
  VECTORNAV0.print(configCommand);
  Serial.println("VectorNav0 configured");

  // VectorNav 1
  VECTORNAV1.begin(9600);
  VECTORNAV1.print("$VNWRG,05,9600*58");
  VECTORNAV1.print(configCommand);
  Serial.println("VectorNav1 configured");

  // VectorNav 2
  VECTORNAV2.begin(9600);
  VECTORNAV2.print("$VNWRG,05,9600*58");
  VECTORNAV2.print(configCommand);
  Serial.println("VectorNav2 configured");
}


void sendVN100Command(const char *command) {
  // Calculate the checksum
  char checksum = 0;
  for (const char *p = command + 1; *p != '\0' && *p != '*'; ++p) {
    checksum ^= *p; // XOR each character between '$' and '*'
  }

  // Send the full command with checksum
  VECTORNAV0.print(command);          // Send the command string 
  VECTORNAV0.printf("%02X\r\n", checksum); // Append the checksum and CRLF

  // Send the full command with checksum
  VECTORNAV1.print(command);          
  VECTORNAV1.printf("%02X\r\n", checksum); 

  // Send the full command with checksum
  VECTORNAV2.print(command);          
  VECTORNAV2.printf("%02X\r\n", checksum);
}

void VectorNav_READ(){
  uint32_t incomingByte;
  if (VECTORNAV0.available() > 0) {
    incomingByte = VECTORNAV0.read();
    Serial.print("VECTORNAV0 UART received: ");
    Serial.println(incomingByte, DEC);
  }
  if (VECTORNAV1.available() > 0) {
    incomingByte = VECTORNAV1.read();
    Serial.print("VECTORNAV1 UART received: ");
    Serial.println(incomingByte, DEC);
  }
  if (VECTORNAV2.available() > 0) {
    incomingByte = VECTORNAV2.read();
    Serial.print("VECTORNAV2 UART received: ");
    Serial.println(incomingByte, DEC);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  motors_INIT();
  VectorNav_INIT();

}

void loop() {
  sendVN100Command("$VNRRG,02");
  VectorNav_READ();

  while (loopRate < loopTime) {
  }
  loopRate = 0;
}
