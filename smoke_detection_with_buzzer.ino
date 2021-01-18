#include <avr/wdt.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

const unsigned long smsEventInterval = 10000;
unsigned long smsPreviousTime = 0;

int redLed = 12;
int greenLed = 11;
int buzzer = 10;
int smokeA0 = A3; 
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
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);
   
  Serial.println("Starting arduino . . .");
  wdt_enable(WDTO_8S);
}


void clearLcdScreen(int line = 1){
  // line numbering starts with zero 
  lcd.setCursor(0,line - 1);

  // print 16 spaces to clear existing text on specified line
  lcd.print("                "); 
}


void printOnLcd(String text, int line = 1){

  clearLcdScreen(line);
  
  // line numbering starts with zero
  line = line -1 ;  
  lcd.setCursor(0,line);
  lcd.print( text );

  // for debugging purposes, remove befor final presentation
//   delay(1500);
//  Serial.println(text);
}


void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}


void sendAlert(String fireOrSmoke){
   
  wdt_reset();
//  delay(5000);
  unsigned long currentTime = millis(); 
  if( (smsPreviousTime == 0) || (currentTime - smsPreviousTime >= smsEventInterval) ){

    String alertMessage = " detected in your house!";
    alertMessage =  fireOrSmoke + alertMessage;
    Serial.println(alertMessage);
  
    // send sms 
    mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
    updateSerial();
  
    mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    updateSerial();
    mySerial.println("AT+CMGS=\"+491638267508\"");//change 49 with country code and xxxxxxxxxxx with phone number to sms
    updateSerial();
    mySerial.print( alertMessage ); //text content
    updateSerial();
    mySerial.write(26);
  
    mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
    updateSerial();

    smsPreviousTime = currentTime;
  }else{
    Serial.println("skipped sending sms");
  }
   
}


void loop() {
  
  checkForGasOrSmoke();
  
  checkForFlames();

  wdt_reset();
}


void checkForGasOrSmoke(){
//  tone(buzzer, 1000, 200);
  int analogSensor = analogRead(smokeA0);
 
  String lcdText = "Gas/Smoke: ";
  printOnLcd(lcdText += analogSensor);
  
  // Checks if it has reached the threshold value
  if (analogSensor > smokeSensorThreshold)
  {
    printOnLcd("SMOKE/GAS ALERT!");
    
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    tone(buzzer, 1000, 500);
//  digitalWrite(buzzer, HIGH);
    sendAlert("SMOKE/GAS");
  }
  else
  { 
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH); 
 // digitalWrite(buzzer, LOW);
    noTone(buzzer);
  }
//  delay(1500);
}


void checkForFlames(){
  int flameSensorReading = analogRead(A0);

  String lcdText = "Fire level: ";
  printOnLcd(lcdText += flameSensorReading ,2);
  
   // FIRE SENSOR 
  Flame = digitalRead(flamePin);
  
  if (Flame== HIGH)
  {
    printOnLcd("FIRE DECTECTED!", 2); 
    
    digitalWrite(buzzer, HIGH);
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    sendAlert("FIRE");
  }
  else
  { 
    digitalWrite(buzzer, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
  }
}
