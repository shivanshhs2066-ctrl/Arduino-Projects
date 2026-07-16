// Car traffic light
int carGreen = 8;
int carYellow = 9;
int carRed = 10;

// Pedestrian lights
int walkLED = 11;
int dontWalkLED = 12;

void setup() {
  pinMode(carGreen, OUTPUT);
  pinMode(carYellow, OUTPUT);
  pinMode(carRed, OUTPUT);

  pinMode(walkLED, OUTPUT);
  pinMode(dontWalkLED, OUTPUT);
}

void loop() {

  // Cars GO
  digitalWrite(carGreen, HIGH);
  digitalWrite(carYellow, LOW);
  digitalWrite(carRed, LOW);

  digitalWrite(walkLED, LOW);
  digitalWrite(dontWalkLED, HIGH);

  delay(5000);


  // Cars SLOW DOWN
  digitalWrite(carGreen, LOW);
  digitalWrite(carYellow, HIGH);

  delay(2000);


  // Cars STOP + Pedestrians WALK
  digitalWrite(carYellow, LOW);
  digitalWrite(carRed, HIGH);

  digitalWrite(walkLED, HIGH);
  digitalWrite(dontWalkLED, LOW);

  delay(5000);


  // Pedestrians stop
  digitalWrite(walkLED, LOW);
  digitalWrite(dontWalkLED, HIGH);

  delay(1000);
}