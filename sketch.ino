#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Create a display with 16 columns and 2 rows.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// true  = Wokwi pushbuttons
// false = real active-HIGH, momentary TTP223 touch sensors
const bool useWokwi = true;

// The eight buttons connect to Arduino pins D2 to D9.
const int buttonPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// The buzzer connects to pin D12.
const int buzzerPin = 12;

// Remember the last message for the Repeat button.
String lastMessage = "";

// Start by waiting for all buttons to be released.
// This also prevents a held button from sending repeatedly.
bool waitingForRelease = true;

// Show two lines of text on the LCD.
void showOnDisplay(String firstLine, String secondLine) {
  lcd.clear();

  // Column 0, row 0 means the beginning of the first line.
  lcd.setCursor(0, 0);
  lcd.print(firstLine);

  // Column 0, row 1 means the beginning of the second line.
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
}

// Find which button is pressed.
//  0 = no button; 1-8 = button number; -1 = multiple buttons.
int readButtons() {
  int numberPressed = 0;
  int selectedButton = 0;

  for (int i = 0; i < 8; i++) {
    int pinValue = digitalRead(buttonPins[i]);
    bool pressed = false;

    if (useWokwi == true) {
      // Wokwi buttons connect the pin to GND when pressed.
      if (pinValue == LOW) {
        pressed = true;
      }
    } else {
      // These touch modules output HIGH when touched.
      if (pinValue == HIGH) {
        pressed = true;
      }
    }

    if (pressed == true) {
      numberPressed = numberPressed + 1;
      // Array positions are 0-7, but button numbers are 1-8.
      selectedButton = i + 1;
    }
  }

  if (numberPressed > 1) {
    return -1;
  }
  return selectedButton;
}

// Display a message, remember it, and make a beep.
void sendMessage(String message) {
  lastMessage = message;
  showOnDisplay("My message:", message);
  Serial.print("MESSAGE: ");
  Serial.println(message);

  // tone(pin, frequency, duration in milliseconds)
  tone(buzzerPin, 1000, 150);
}

// setup() runs once when the Arduino starts.
void setup() {
  Serial.begin(115200);

  // Set up each button or touch-sensor pin.
  for (int i = 0; i < 8; i++) {
    if (useWokwi == true) {
      pinMode(buttonPins[i], INPUT_PULLUP);
    } else {
      pinMode(buttonPins[i], INPUT);
    }
  }

  pinMode(buzzerPin, OUTPUT);
  // Start the LCD and turn on its backlight.
  lcd.init();
  lcd.backlight();
  showOnDisplay("Starting...", "Release all pads");

  // Leave real touch sensors untouched during startup.
  delay(1200);
  showOnDisplay("TouchTalk ready", "Touch one pad");
  Serial.println("TouchTalk ready");
  tone(buzzerPin, 1200, 150);
}

// loop() runs repeatedly while the Arduino is on.
void loop() {
  int button = readButtons();

  // Simple debounce: wait briefly, then check again.
  delay(40);
  int secondReading = readButtons();
  if (button != secondReading) {
    // Ignore an unstable reading and check again next time.
    return;
  }

  // No buttons pressed: allow the next action.
  if (button == 0) {
    waitingForRelease = false;
    return;
  }

  // Ignore held buttons until everything is released.
  if (waitingForRelease == true) {
    return;
  }
  waitingForRelease = true;

  if (button == -1) {
    showOnDisplay("One pad at a", "time, please");
    Serial.println("Please press only one button.");
    tone(buzzerPin, 300, 200);
  }
  else if (button == 1) {
    sendMessage("Yes");
  }
  else if (button == 2) {
    sendMessage("No");
  }
  else if (button == 3) {
    sendMessage("I need water");
  }
  else if (button == 4) {
    sendMessage("I need food");
  }
  else if (button == 5) {
    sendMessage("Need the toilet");
  }
  else if (button == 6) {
    sendMessage("Please help me");
    // A longer, higher beep for the Help message.
    tone(buzzerPin, 1500, 450);
  }
  else if (button == 7) {
    // Repeat only if a previous message exists.
    if (lastMessage != "") {
      sendMessage(lastMessage);
    } else {
      showOnDisplay("No message yet", "Choose a pad");
      tone(buzzerPin, 350, 160);
    }
  }
  else if (button == 8) {
    // Forget the previous message and reset the display.
    lastMessage = "";
    showOnDisplay("TouchTalk ready", "Touch one pad");
    Serial.println("Message cleared");
    tone(buzzerPin, 700, 100);
  }
}
