#include "Keyboard.h"

// declaring button pins and other const macros
#define BUTTONS 4
#define I8  4
#define I16 2
#define I32 1
#define LED_PLAY_TIMEOUT_MULTIPLIER 50
#define ANSWER_DISPLAY_TIME 5000
#define BUTTON_DEBOUNCING 50

// declaring global variables for macro keyboard
uint8_t buttonPin[BUTTONS] = {2, 3, 4, 5};
uint8_t buttonLedPin[BUTTONS] = {6, 7, 8, 9};
uint8_t buttonState[BUTTONS] = {HIGH, HIGH, HIGH, HIGH};
uint8_t previousButtonState[BUTTONS] = {HIGH, HIGH, HIGH, HIGH}; 
uint8_t buttonKey[BUTTONS] = {49, 50, 51, 52};
uint8_t buttonPressStep = 0; 
uint8_t buttonPressed = 99;
unsigned long buttonDebounceTimeout = 0;
unsigned long answerTimeout = 0;
unsigned long ledPlayTimeout = 0;
uint16_t ledPlayIndex = 0;
// declaring functions
void setLedsState(uint8_t state);
void playLights();

// declaring on led light show parameters
uint8_t LEDShow[] = {
  0b1111, I8,
  0b1111, I16,
  0b0000, I16,
  0b1111, I8,
  0b1111, I16,
  0b0000, I16,
  0b1000, I8,
  0b1000, I16,
  0b0000, I16,
  0b0100, I8,
  0b0100, I16,
  0b0000, I16,
  0b0010, I8,
  0b0010, I16,
  0b0000, I16,
  0b0001, I8,
  0b0001, I16,
  0b0000, I16,
  0b0001, I8,
  0b0001, I16,
  0b0000, I16,
  0b0010, I8,
  0b0010, I16,
  0b0000, I16,
  0b0100, I8,
  0b0100, I16,
  0b0000, I16,
  0b1000, I8,
  0b1000, I16,
  0b0000, I16,
  0b1111, I16,
  0b1111, I32,
  0b0000, I32,
  0b1111, I16,
  0b1111, I32,
  0b0000, I32,
  0b1000, I16,
  0b1000, I32,
  0b0000, I32,
  0b0100, I16,
  0b0100, I32,
  0b0000, I32,
  0b0010, I16,
  0b0010, I32,
  0b0000, I32,
  0b0001, I16,
  0b0001, I32,
  0b0000, I32,
  0b0001, I16,
  0b0001, I32,
  0b0000, I32,
  0b0010, I16,
  0b0010, I32,
  0b0000, I32,
  0b0100, I16,
  0b0100, I32,
  0b0000, I32,
  0b1000, I16,
  0b1000, I32,
  0b0000, I32
};

void setup() {
  //declare the buttons as input_pullup
  for (int i = 0; i < BUTTONS; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);  
    pinMode(buttonLedPin[i], OUTPUT); 
  }
  Keyboard.begin();
}

void loop() {
  switch (buttonPressStep) {
    case 0 : //checking the state of the each button, if button pressed then start sequence
      for (int i = 0; i < BUTTONS; i++) {
        buttonState[i] = digitalRead(buttonPin[i]);
        // identify button press
        if (buttonState[i] == LOW && previousButtonState[i] == HIGH) {
          // turn off all LEDs and turn on the pressed buttons LED
          setLedsState(1 << i);
          // send key press for currently pressed button:
          Keyboard.press(buttonKey[i]);
          buttonPressStep = 1;
          buttonDebounceTimeout = millis() + BUTTON_DEBOUNCING;
          buttonPressed = i;
        }
      }
      if (buttonPressed == 99) {
        playLights();
      }
      break;
    case 1 : // send key release for currently pressed button
      if (buttonDebounceTimeout < millis()) {
        Keyboard.release(buttonKey[buttonPressed]);
        buttonPressStep = 2;
      }
      break;
    case 2 : 
      // wait for button to be released
      // when released then reset previous state, set next button press step and timing
      if (digitalRead(buttonPin[buttonPressed]) == HIGH) {
        previousButtonState[buttonPressed] = HIGH;
        buttonDebounceTimeout = millis() + BUTTON_DEBOUNCING;
        buttonPressStep = 3;
      }
      break;
    case 3 : // debounce released button
      if (buttonDebounceTimeout < millis()) {
        answerTimeout = millis() + ANSWER_DISPLAY_TIME;
        buttonPressStep = 4;
      }
      break;
    case 4 : // wait for next answer timeout or another button press
      if (answerTimeout < millis()) {
        buttonPressStep = 7;
      } else {
        for (int i = 0; i < BUTTONS; i++) {
          // if any key pressed, wait for its release and break
          buttonState[i] = digitalRead(buttonPin[i]);
          if (buttonState[i] == LOW) {
            buttonPressStep = 5;
            buttonDebounceTimeout = millis() + BUTTON_DEBOUNCING;
          }
        }
      }
      break;
    case 5 : // wait for keys to be released
      if (buttonDebounceTimeout < millis()) {
        boolean buttonsReleased = true;
        for (int i = 0; i < BUTTONS; i++) {
          buttonState[i] = digitalRead(buttonPin[i]);
          if (buttonState[i] == LOW) {
            buttonsReleased = false;
          }
        }
        if (buttonsReleased) {
          buttonPressStep = 6;
          buttonDebounceTimeout = millis() + BUTTON_DEBOUNCING;
        }
      }
      break;
    case 6 : // debounce keys release
      if (buttonDebounceTimeout < millis()) {
        buttonPressStep = 7;
      }
      break;
    case 7 : // send enter pressed
      Keyboard.press(10);
      buttonDebounceTimeout = millis() + BUTTON_DEBOUNCING;
      buttonPressStep = 8;
      break;
    case 8 : // send enter released and turn off the LED
      if (buttonDebounceTimeout < millis()) {
        Keyboard.release(10);
        digitalWrite(buttonLedPin[buttonPressed], LOW);
        buttonPressed = 99;
        buttonPressStep = 0;
      }
      break;
    default:
      buttonPressStep = 0;
      break;
  }
}

void setLedsState(uint8_t state) {
  for (int k = 0; k < BUTTONS; k++) {
    digitalWrite(buttonLedPin[k], (state & (1 << k)) > 0 ? HIGH : LOW);
  }
}

void playLights() {
  if (ledPlayTimeout < millis()) {
    ledPlayIndex = (ledPlayIndex + 2 ) % sizeof(LEDShow) / sizeof(LEDShow[0]);
    setLedsState(LEDShow[ledPlayIndex]);
    ledPlayTimeout = millis() + LEDShow[ledPlayIndex+1] * LED_PLAY_TIMEOUT_MULTIPLIER;
  } 
}
