///////////////////////////////////////////////////////////////
//////////color////////////////
///////////////////////////////////////////
/*
  Eecher's TCS3200 program
  adapted from code found at reibot.org

  This section will return the pulseIn reading of the selected color.
  It will turn on the sensor at the start taosMode(1), and it will power off the sensor at the end taosMode(0)
  color codes: 0=white, 1=red, 2=blue, 3=green
  if LEDstate is 0, LED will be off. 1 and the LED will be on.
  taosOutPin is the ouput pin of the TCS3200.
*/
/*
float colorRead(int taosOutPin) {
  //turn on sensor and use highest frequency/sensitivity setting
  taosMode(3);

  //setting for a delay to let the sensor sit for a moment before taking a reading.
  int sensorDelay = 50;

  //set the S2 and S3 pins to select the color to be sensed
  //(color == 3) {//green
  digitalWrite(S3, HIGH); //S3
  digitalWrite(S2, HIGH); //S2

  // create a var where the pulse reading from sensor will go
  float readPulse;

  //  turn LEDs on or off, as directed by the LEDstate var (IKKE Frydenlund)
  digitalWrite(LED, HIGH);

  // wait a bit for LEDs to actually turn on, as directed by sensorDelay var
  delay(sensorDelay);

  // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
  readPulse = pulseIn(taosOutPin, LOW, 80000);

  //if the pulseIn times out, it returns 0 and that throws off numbers. just cap it at 80k if it happens
  if (readPulse < .1) {
    readPulse = 80000;
  }

  //turn off color sensor and LEDs to save power
  taosMode(0);

  // return the pulse value back to whatever called for it...
  return readPulse;
}

// Operation modes area, controlled by hi/lo settings on S0 and S1 pins.
//setting mode to zero will put taos into low power mode. taosMode(0);

void taosMode(int mode) {
  if (mode == 0) {
    //power OFF mode-  LED off and both channels "low"
    digitalWrite(LED, LOW);
    digitalWrite(S0, LOW); //S0
    digitalWrite(S1, LOW); //S1
  } else if (mode == 1) {
    //this will put in 1:1, highest sensitivity
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, HIGH); //S1
  } else if (mode == 2) {
    //this will put in 1:5
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, LOW); //S1
  } else if (mode == 3) {
    //this will put in 1:50
    digitalWrite(S0, LOW); //S0
    digitalWrite(S1, HIGH); //S1
  }
}

void ColorBalance(){
  colorIndex=5000/colorRead(taosOutPin);
}
*/
