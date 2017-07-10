void setup() {
  analogReference(INTERNAL);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.print(millis());
  Serial.print(",");
  Serial.println(map(analogRead(A0), 685, 355, 0, 250));
  delay(500);
}
