#include <ezButton.h>
#include <BleMouse.h>

#define VRX_PIN  39 // ESP32 pin GPIO39 (ADC3) connected to VRX pin
#define VRY_PIN  36 // ESP32 pin GPIO36 (ADC0) connected to VRY pin
#define SW_PIN   17 // ESP32 pin GPIO17 connected to SW pin

#define LEFT_THRESHOLD  1000
#define RIGHT_THRESHOLD 3000
#define UP_THRESHOLD    1000
#define DOWN_THRESHOLD  3000

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

ezButton button(SW_PIN);

int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value
int command = COMMAND_NO;

BleMouse bleMouse("TEST6", "DT", 66);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Mouse");
  bleMouse.begin();

  // Set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);

  button.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  // Check if BLE Mouse is connected
  if (!bleMouse.isConnected()) {
    return;
  }

  button.loop(); // MUST call the loop() function first

  // Read X and Y analog values
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  // Reset commands
  command = COMMAND_NO;

  // Check left/right commands
  if (valueX < LEFT_THRESHOLD) {
    command = command | COMMAND_LEFT;
  } else if (valueX > RIGHT_THRESHOLD) {
    command = command | COMMAND_RIGHT;
  }

  // Check up/down commands
  if (valueY < UP_THRESHOLD) {
    command = command | COMMAND_UP;
  } else if (valueY > DOWN_THRESHOLD) {
    command = command | COMMAND_DOWN;
  }

  // Process command
  if (command & COMMAND_LEFT) {
    Serial.println("COMMAND LEFT");
    bleMouse.move(10, 0); // Move mouse left
  }

  if (command & COMMAND_RIGHT) {
    Serial.println("COMMAND RIGHT");
    bleMouse.move(-10, 0); // Move mouse right
  }

  if (command & COMMAND_UP) {
    Serial.println("COMMAND UP");
    bleMouse.move(0, -10); // Move mouse up
  }

  if (command & COMMAND_DOWN) {
    Serial.println("COMMAND DOWN");
    bleMouse.move(0, 10); // Move mouse down
  }

  // Handle button click for right-click
  if (button.isPressed()) {
    Serial.println("Button Pressed: Left Click");
    bleMouse.click(MOUSE_LEFT);// Perform right-click
  }

  // Add a short delay to make the movement smoother
  delay(50);
}
