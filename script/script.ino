void setup() { Serial.begin(9600); }

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    if (command == "on") {
      digitalWrite(13, HIGH);
    } else if (command == "off") {
      digitalWrite(13, LOW);
    }
  }
}