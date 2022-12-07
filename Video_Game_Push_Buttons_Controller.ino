#include "Keyboard.h"

//declaring button pins 
#define BUTTONS 5
int buttonPin[BUTTONS] = {2, 3, 4, 5, 6};
int previousButtonState[BUTTONS] = {HIGH, HIGH, HIGH, HIGH, HIGH}; 
int buttonKey[BUTTONS] = {49, 50, 51, 52, 48};
void setup() {
  Serial.begin(115200);
  //declare the buttons as input_pullup
  for (int i = 0; i < BUTTONS; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);  
  }
  Keyboard.begin();
}

void loop() {
  //checking the state of the eacg button
  int buttonState[BUTTONS];
  for (int i = 0; i < BUTTONS; i++) {
    buttonState[i] = digitalRead(buttonPin[i]);
  
    // identify button press
    if (buttonState[i] == LOW && previousButtonState[i] == HIGH) {
      // send key press for currently pressed button:
      Keyboard.press(buttonKey[i]);
      delay(50);
      // send key release for currently pressed button
      Keyboard.release(buttonKey[i]);
      // wait for key to be released
      while (digitalRead(buttonPin[i]) == LOW) {
        // do nothing
        delay(50);
      }
      delay(50);
      // reset previous state
      previousButtonState[i] = HIGH;
      // wait for either 5 seconds or another key release
      unsigned long wait = millis();
      while (true) {
        boolean wasKeyReleased = false;
        for (int j = 0; j < BUTTONS; j++) {
          // if any key pressed, wait for its release and break
          buttonState[j] = digitalRead(buttonPin[j]);
          if (buttonState[j] == LOW) {
            delay(50);
            while (buttonState[j] == LOW) {
              buttonState[j] = digitalRead(buttonPin[j]);
              delay(50);
            }
            wasKeyReleased = true;
          }
        }
        if (wasKeyReleased) {
          break;
        }
        Serial.print("waiting since: ");
        Serial.print(wait);
        Serial.print(" < millis() - 5000, which is: ");
        Serial.print(millis() - 5000);
        Serial.print(", the result was: ");
        Serial.println(wait < millis() - 5000);
        delay(50);
        if (wait < millis() - 5000) {
          Serial.println("times up, next...");
          break;
        }
      }
      // send key press for "Enter"
      Keyboard.press(10);
      delay(50);
      Keyboard.release(10);
    }
  }
}
