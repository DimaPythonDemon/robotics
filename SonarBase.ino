#include <NewPing.h>

#define PIN_TRIG 12
#define PIN_ECHO 11

#define MAX_DISTANCE 200 // or 2m

// creating object, that has interface of getting distance
// parameters are pins that are connected

NewPing sonar(PIN_TRIG, PIN_ECHO, MAX_DISTANCE);

void setup() {
  // already persist in the main code snippet, but its for sure
  Serial.begin(9600);
}

void loop() {

  // start delay for correct work
  delay(50);

  unsigned int distance = sonar.ping_cm();

  Serial.print(distance/100);
  Serial.println("m");

}