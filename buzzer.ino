void beepShort() {

  digitalWrite(BUZZER_PIN, HIGH);
  delay(80);
  digitalWrite(BUZZER_PIN, LOW);
}

void handleSoundTouch(int x, int y) {

  handleBackTouch(x, y);

  if (inRect(x, y, 60, 70, 200, 60)) {

    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
  }
}