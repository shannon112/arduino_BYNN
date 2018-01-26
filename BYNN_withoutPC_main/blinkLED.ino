void blinkLeft() {
  digitalWrite(midLED, LOW);
  digitalWrite(leftLED, HIGH);
  digitalWrite(rightLED, LOW);
  digitalWrite(whyLED, LOW);

}

void blinkMid() {
  digitalWrite(midLED, HIGH);
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
  digitalWrite(whyLED, LOW);

}

void blinkRight() {
  digitalWrite(midLED, LOW);
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, HIGH);
  digitalWrite(whyLED, LOW);

}

void blinkWhy() {
  digitalWrite(midLED, LOW);
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
  digitalWrite(whyLED, HIGH);
}
