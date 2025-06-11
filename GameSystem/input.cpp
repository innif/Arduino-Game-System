#include "input.h"

ButtonState buttons;
unsigned long lastButtonTime = 0;
ButtonState lastButtons;

void initInput() {
  pinMode(PIN_BUTTON_UP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
  pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
  pinMode(PIN_BUTTON_RIGHT, INPUT_PULLUP);
  
  // Initialize button states
  buttons = {false, false, false, false, false, false, false, false};
  lastButtons = buttons;
}

void updateInput() {
  // Store previous state
  lastButtons = buttons;
  
  // Read current state (inverted because of INPUT_PULLUP)
  buttons.up = (digitalRead(PIN_BUTTON_UP) == LOW);
  buttons.down = (digitalRead(PIN_BUTTON_DOWN) == LOW);
  buttons.left = (digitalRead(PIN_BUTTON_LEFT) == LOW);
  buttons.right = (digitalRead(PIN_BUTTON_RIGHT) == LOW);
  
  // Detect button press events (just pressed)
  buttons.upPressed = buttons.up && !lastButtons.up;
  buttons.downPressed = buttons.down && !lastButtons.down;
  buttons.leftPressed = buttons.left && !lastButtons.left;
  buttons.rightPressed = buttons.right && !lastButtons.right;
}

bool isButtonPressed(int pin) {
  switch(pin) {
    case PIN_BUTTON_UP: return buttons.up;
    case PIN_BUTTON_DOWN: return buttons.down;
    case PIN_BUTTON_LEFT: return buttons.left;
    case PIN_BUTTON_RIGHT: return buttons.right;
    default: return false;
  }
}

bool wasButtonJustPressed(int pin) {
  switch(pin) {
    case PIN_BUTTON_UP: return buttons.upPressed;
    case PIN_BUTTON_DOWN: return buttons.downPressed;
    case PIN_BUTTON_LEFT: return buttons.leftPressed;
    case PIN_BUTTON_RIGHT: return buttons.rightPressed;
    default: return false;
  }
}