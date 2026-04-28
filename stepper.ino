#include <AccelStepper.h>

// CORRECTED PIN MAPPING FOR WEMOS D1 R32
// Using the "D" pin numbers as labeled on the board
// D2 -> GPIO26, D3 -> GPIO25, D4 -> GPIO17, D5 -> GPIO16

#define IN1 26   // D2 on WEMOS D1 R32
#define IN2 25   // D3 on WEMOS D1 R32  
#define IN3 17   // D4 on WEMOS D1 R32
#define IN4 16   // D5 on WEMOS D1 R32

const int stepsPerRevolution = 2048;  // For 28BYJ-48 stepper motor

// For ULN2003 driver with 4 pins, use HALF4WIRE (type 8)
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

int motorSpeed = 600;  // Steps per second

void setup() {
  Serial.begin(115200);
  delay(1000);  // Give serial time to connect
  
  // Configure pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(motorSpeed);
  stepper.setAcceleration(500);
  
  Serial.println("\n========== STEPPER CONTROL ==========");
  Serial.println("  f or forward  - Rotate forward one revolution");
  Serial.println("  r or reverse  - Rotate backward one revolution");
  Serial.println("  speed X       - Set speed (100-1000 steps/sec)");
  Serial.println("==================================================\n");
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readString();
    cmd.trim();
    cmd.toLowerCase();
    
    if (cmd == "f" || cmd == "forward") {
      Serial.println("Rotating FORWARD...");
      stepper.moveTo(stepper.currentPosition() + stepsPerRevolution);
      while (stepper.distanceToGo() != 0) {
        stepper.run();
      }
      Serial.println("Complete!");
    }
    else if (cmd == "r" || cmd == "reverse") {
      Serial.println("Rotating BACKWARD...");
      stepper.moveTo(stepper.currentPosition() - stepsPerRevolution);
      while (stepper.distanceToGo() != 0) {
        stepper.run();
      }
      Serial.println("Complete!");
    }
    else if (cmd.startsWith("speed")) {
      int newSpeed = cmd.substring(6).toInt();
      if (newSpeed >= 100 && newSpeed <= 1000) {
        motorSpeed = newSpeed;
        stepper.setSpeed(motorSpeed);
        stepper.setMaxSpeed(motorSpeed);
        Serial.print("Speed set to: ");
        Serial.print(motorSpeed);
        Serial.println(" steps/second");
      } else {
        Serial.println("Invalid! Use 100-1000 steps/sec");
      }
    }
  }
  
  stepper.runSpeedToPosition();
}