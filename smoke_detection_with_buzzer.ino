#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

  // initialize the LCD and turn on the blacklight.
  lcd.begin(); 
  lcd.backlight();
  lcd.print("Starting ..."); 
}


void printOnLcd(String text, int line = 1){
  
  // line numbering starts with zero
  line = line -1 ; 
  lcd.setCursor(0,line);

  // print 16 spaces to clear existing text on specified line
  lcd.print("                "); 

  lcd.setCursor(0,line);
  lcd.print( text );

  // for debugging purposes, remove befor final presentation
  Serial.println(text);
}

void loop() {
  
  checkForGasOrSmoke();

  // tone(buzzer, 1000, 200);
  delay(500);
 
  checkForFlames();
}

void checkForGasOrSmoke(){
//  tone(buzzer, 1000, 200);
  int analogSensor = analogRead(smokeA0);

//  Serial.print("Pin A0: ");
//  Serial.println(analogSensor);
  String lcdText = "Gas/Smoke: ";
  printOnLcd(lcdText += analogSensor);
  
  // Checks if it has reached the threshold value
  if (analogSensor > smokeSensorThreshold)
  {
    printOnLcd("SMOKE DECTECTED!", 2);
    
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    tone(buzzer, 1000, 200);
  }
  else
  {
    printOnLcd("                ", 2);
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
