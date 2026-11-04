// Pinnen configureren (beide aan dezelfde kant van het breadboard)
const int BUTTON_PIN = 26;   // Knop tussen GPIO 26 en GND
const int MOTOR_PIN  = 27;   // Aangesloten op IN1 van de DRV8833

// PWM-instellingen
const int PWM_FREQ = 20000;  // 20 kHz
const int PWM_RES  = 8;      // 8-bit (0 - 255)
const int MOTOR_SNELHEID = 200; // Snelheid (0 tot 255)

// Timings
const unsigned long AAN_TIJD = 10000; // 10 seconden

bool motorActief = false;
unsigned long startTijd = 0;
int vorigeKnopStatus = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // PWM configureren op GPIO 27
  ledcAttach(MOTOR_PIN, PWM_FREQ, PWM_RES);
  ledcWrite(MOTOR_PIN, 0); // Standaard uit
}

void loop() {
  int huidigeKnopStatus = digitalRead(BUTTON_PIN);

  // Detecteer knopdruk
  if (vorigeKnopStatus == HIGH && huidigeKnopStatus == LOW && !motorActief) {
    Serial.println("Knop ingedrukt: Motor gestart.");
    motorActief = true;
    startTijd = millis();
    
    ledcWrite(MOTOR_PIN, MOTOR_SNELHEID);
    delay(50); // Debounce
  }
  vorigeKnopStatus = huidigeKnopStatus;

  // Schakel na 10 seconden automatisch weer uit
  if (motorActief && (millis() - startTijd >= AAN_TIJD)) {
    ledcWrite(MOTOR_PIN, 0);
    motorActief = false;
    Serial.println("Tijd voorbij: Motor gestopt.");
  }
}