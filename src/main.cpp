#include <SoftwareSerial.h>
#include <Arduino.h>
#include "StripController.h"

static const char *const EMPTY_MSG = "";

const int INIT = 0;
const int READ = 1;
const int LED = 2;

const int BUTTON = A0;

const int ARDUINO_LED_PIN = 13;

int state = INIT;
int counter = 0;
String message;
int current_show = 0;

String readStream(Stream &serial) {
  String content = "";
  char character;

  while (serial.available()) {
    character = serial.read();
    content.concat(character);
  }

  if (content != EMPTY_MSG) {
    serial.println(content);
  }
  return content;
}

String str(const char *chars) {
  return String(chars);
}

const int DEFAULT_DELAY = 1000;
void blink() {
  {
    digitalWrite(ARDUINO_LED_PIN, HIGH);       // sets the digital pin 13 on
    delay(DEFAULT_DELAY);                  // waits for a second
    digitalWrite(ARDUINO_LED_PIN, LOW);        // sets the digital pin 13 off
    delay(DEFAULT_DELAY);
  }
}

void setup() {
  // initialize the LED pin as an output:
  pinMode(ARDUINO_LED_PIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(BUTTON, INPUT);

  blink();

  Serial.begin(9600);

  while (!Serial) { ; // wait for serial port to connect. Needed for Leonardo only
  }
  configureStrip();

  blink();
  blink();
}

static unsigned long lastClickTime = 0;

bool checkClicked() {
  Serial.println("Check clicked started!");
  int interval = millis() - lastClickTime;
  if (interval < 100) {
    Serial.println("Not clicked. Interval:" + interval);
    return false;
  }

  int counter = 0;

  for (int i = 0; i < 30; i++) {
    int state = digitalRead(BUTTON);
    if (state == HIGH) {
      counter += 1;
      Serial.println("Button pushed: " + counter);
    }

    delayMicroseconds(10);
  }

  if (counter > 5) {
    lastClickTime = millis();
    Serial.println("Clicked!");
    return true;
  }

  Serial.println("Not clicked!");
  return false;
}

bool checkPushed() {
  int state = analogRead(BUTTON);
  Serial.println(state);

  bool pushed = state == LOW;

  if (pushed) {
    Serial.println("Button pushed!");
  } else {
    Serial.println("Button not pushed!");
  }

  return pushed;
}

const int shows[] = {7, 8, 9, 10, 12, 11, 13, 5, 6, 3};
int current = 0;

void loop() {
  if (checkPushed()) {
    blink();

    if (++current >= 10) {
      current = 0;
    }
  }

  startShow(current);
}

