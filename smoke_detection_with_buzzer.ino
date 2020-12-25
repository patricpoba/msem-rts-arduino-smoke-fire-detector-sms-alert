
int redLed = 12;
int greenLed = 11;
int buzzer = 10;
int smokeA0 = A3;
// Your threshold value
int smokeSensorThreshold = 400;

const int flamePin = 9;
int Flame = LOW;

void setup() {
  
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
 
  pinMode(flamePin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  
  checkForGasOrSmoke();

//  delay(100);
 
  checkForFlames();
}

void checkForGasOrSmoke(){
  
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > smokeSensorThreshold)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    tone(buzzer, 1000, 200);
  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    noTone(buzzer);
  }
}

void checkForFlames(){
   // FIRE SENSOR 
  Flame = digitalRead(flamePin);
  
  if (Flame== HIGH)
  {
    digitalWrite(buzzer, HIGH);
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
  }
  else
  {
    digitalWrite(buzzer, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
  }
}
