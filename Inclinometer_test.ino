#include <elapsedMillis.h>
// ======== Sensors ========
const int numberOfMotorThermistors = 2;
const int numberOfGrippertransducers = 2;
const int numberOfMethaneSensors = 2;
const int numberOfInclinometers = 2;

// ======== inclinometers ========
const int inclinometerMax = 929;
const int inclinometerMin = 185;

// ======== timing ========
const int loopTime = 10; // Defined void loop function time [ms]
elapsedMillis loopRate;   // Variable to track loop time

// ======== ProtoBuf ========
// frontInclinometer data = frontInclinometer_init_zero;

struct Inclinometer{
    int X_in_pin;
    int Y_in_pin;
    int reset;
    int reading;
    float degrees;
};

Inclinometer inclinometers[numberOfInclinometers]{
    {
        .X_in_pin = 40,
        .Y_in_pin = 41,
        .reset = 14,
        .reading = 0,
        .degrees = 0.0,
    },
    {
        .X_in_pin = 38,
        .Y_in_pin = 39,
        .reset = 37,
        .reading = 0,
        .degrees = 0.0,
    },
};

void setup() {
    Serial.begin(9600);

    for(int i = 0; i < numberOfInclinometers; i++) {
        pinMode(inclinometers[i].X_in_pin, INPUT);
        pinMode(inclinometers[i].Y_in_pin, INPUT);
    }
    Serial.println("Setup complete");
}

void loop() {
    for(int i = 0; i <numberOfInclinometers; i++) {
        Inclinometer* ir = &inclinometers[i];
        ReadInclinometers(ir);
    }

    // for(int i = 0; i<numberOfMethaneSensors; i++) {
    //     readMethaneSensors();
    // }

    while (loopRate < loopTime) {
    }
    loopRate = 0;

}

void ReadInclinometers(Inclinometer* ir) {
    // Read sensor and store its value
    ir->reading = analogRead(ir->X_in_pin);

    float resolution = 360.0/(inclinometerMax - inclinometerMin);   // in degrees per step
    float middle = (inclinometerMax - inclinometerMin)/2 + inclinometerMin;           // Middle of steps

    if(ir->reading < middle) {
      ir->degrees = float((ir->reading - inclinometerMin) * resolution); 
    }
    else {
      ir->degrees = float((ir->reading - inclinometerMax) * resolution);
    }
    Serial.print("Degrees: ");
    Serial.println(ir->degrees);
}
