/*
  Timer

  This sketch is an adjustable timer (0-102 seconds) that displays how many seconds it has been since the timer was started.
  To use it: set the length of the timer by turning the potentiometer. The set duration will appear on the screen. To start the timer, simply press the pushbutton.
  When the timer ends, the display will blink four times and then be ready for the next timer.

  The circuit:
  - Display pin A -> Arduino digital pin 5
  - Display pin B -> Arduino digital pin 6
  - Display pin C -> Arduino digital pin 7
  - Display pin D -> Arduino digital pin 8
  - Display pin E -> Arduino digital pin 9
  - Display pin F -> Arduino digital pin 10
  - Display pin G -> Arduino digital pin 11
  - Display pin DP -> Arduino digital pin 12
  - Display digit pin 1 -> 270Ω -> Arduino pin 1
  - Display digit pin 2 -> 270Ω -> Arduino pin 2
  - Display digit pin 3 -> 270Ω -> Arduino pin 3
  - Display digit pin 4 -> 270Ω -> Arduino pin 4

  - GND -> pushbutton -> Arduino pin 13 -> 10KΩ -> VCC

  - Outer pin of 10KΩ potentiometer -> VCC
  - Other outer pin of 10KΩ potentiometer -> GND
  - Wiper pin of 10KΩ potentiometer -> Arduino A0

  created 11/21/2025
  by Nyjah

  This example sketch is in the public domain.
*/

#include <AutoPlex7.h>

// Set up display
int displayType = COMMON_CATHODE; // Change to "COMMON_ANODE" if using a common anode display
int D1 = 1;
int D2 = 2;
int D3 = 3;
int D4 = 4;
int A = 5;
int B = 6;
int C = 7;
int D = 8;
int E = 9;
int F = 10;
int G = 11;
int DP = 12;

// Create variables
unsigned long previousMillis = 0;
const long interval = 1000;
long seconds = 0;
int buttonState = 0;
int timing = 0;

void setup() {
  pinMode(13, INPUT_PULLUP); // Set the pin the pushbutton is connected to as an input

  display.begin(); // Initialize the display
  display.testDisplay(); // Run test to ensure display functionality
  delay(1000); // Wait one second
  display.clearDisplay(); // Clear the display
}

void loop() {
  unsigned long potValue = analogRead(A0); // Read analog value at A0
  unsigned long timer = (potValue * 100) / 1000; // Convert analog value from A0 into 0-102 seconds
  unsigned long currentMillis = millis(); // Get the current time
  buttonState = digitalRead(13); // Check state of button

  display.showNumber(timer); // Display the current set timer duration

  // Check if button is pressed
  if (buttonState == HIGH) {
    timing = 1; // Activate the timer
  }

  // Check if timer should be counting
  if (timing == 1) {
    if (currentMillis - previousMillis >= interval) {
      // Save the last time the counter was updated
      previousMillis = currentMillis;

      seconds++; // Increment the seconds counter
    }
    display.showNumber(seconds); // Print the number of seconds on the screen

    // Check if the number of seconds counted match what the timer is set to
    if (seconds == timer) {
      display.clearDisplay(); // Remove the seconds being counted from the display
      // Blink the timer value on the screen to signal the end of the timer
      display.showNumber(timer); delay(250); display.clearDisplay(); delay(250); display.showNumber(timer); delay(250); display.clearDisplay(); delay(250);
      display.showNumber(timer); delay(250); display.clearDisplay(); delay(250); display.showNumber(timer); delay(250); display.clearDisplay(); delay(250);
      // Reset timer variables to 0
      timing = 0;
      previousMillis = 0;
      seconds = 0;
    }
  }
}
