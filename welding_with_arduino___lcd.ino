/*
  
  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int analogPin = A0; // potentiometer wiper (middle terminal) connected to analog pin 3
// outside leads to ground and +5V

const int INCREASE = 8;
const int DECREASE = 9;
const int SET = 10;
const int SENSOR = 6;
const int GUN = 7;
const int BUZZER = 13;
const int LCD = A5;

int Increase_State = LOW;
int Decrease_State = LOW;
int Set_State = LOW;

int time1 = 0;
int time2 = 0;
int value;
int i = 1;

int set_time(int code)
{

  while (Set_State != HIGH)
  {
    Increase_State = digitalRead(INCREASE);
    Decrease_State = digitalRead(DECREASE);
    Set_State = digitalRead(SET);

    if (Increase_State == HIGH)
    {
      value++;
      delay(200);
      lcd.clear();
    }

    else if (Decrease_State == HIGH)
    {
      value--;
      delay(200);
      lcd.clear();
    }

    if (value > 255)
      value = 255;
    else if (value < 0)
      value = 0;

    lcd.setCursor(0, 0);
    lcd.print("Setting Time-");
    lcd.print(code);

    lcd.setCursor(0, 1);
    lcd.print("TIME ");
    lcd.print(code);

    lcd.print(" : ");
    lcd.print(value);
  }
  return value;
}

void continue_gun()
{
  if (digitalRead(SENSOR) == HIGH)
  {
    i++;
    Serial.print("Ball No. : ");
    Serial.println(i);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting....");

    delay(time1 );
    for (int x = 0; x < 3; x++)
    {
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      delay(100);
    }
    lcd.noDisplay();
    digitalWrite(LCD, LOW);
    digitalWrite(GUN, HIGH);
    //
    //    lcd.clear();
    //    lcd.setCursor(0, 0);
    //    lcd.print("Rotating....");
    delay(time2);

    digitalWrite(GUN, LOW);
    lcd.display();
    digitalWrite(LCD, HIGH);
    lcd.begin(16, 2);
    lcd.clear();

    while (digitalRead(SENSOR) == HIGH)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Release now.");
      digitalWrite(BUZZER, HIGH);
      delay(500);
    }
    // delay(3000);
    digitalWrite(BUZZER, LOW);
    lcd.clear();
  }
  else
    digitalWrite(GUN, LOW);
}


void pause_gun()
{
  while (digitalRead(SET) != HIGH)
  {
    digitalWrite(LCD, HIGH);
    digitalWrite(GUN, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Paused....");
  }
  return;
}



void setup() {

  Serial.begin(9600);
  pinMode(SENSOR, INPUT);
  pinMode(GUN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LCD, OUTPUT);

  lcd.begin(16, 2);
  digitalWrite(LCD, HIGH);

  lcd.setCursor(0, 0);
  lcd.print("hello, world!");
  delay(2000);
  lcd.clear();

  lcd.print("Enter Time 1.");
  time1 = set_time(1);
  delay(500);
  lcd.clear();

  Set_State = LOW;
  value = 0;
  delay(500);


  lcd.setCursor(0, 0);
  lcd.print("Enter Time 2.");
  time2 = set_time(2);
  delay(500);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T1 : ");
  lcd.print(time1);

  lcd.print("  T2 : ");
  lcd.print(time2);


  delay(2000);

  time1 = time1 * 1000;
  time2 = time2 * 1000;

  Serial.print("Time 1: ");
  Serial.println(time1);
  Serial.print("Time 2: ");

  Serial.println(time2);

  while (digitalRead(SET) == LOW)
  {
    //lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Push OK to Start");
    delay(1500);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting.....");
  delay(2000);
}

void loop() {

  Set_State = digitalRead(SET);
  lcd.setCursor(0, 0);
  lcd.print("Program Started.");

  if (Set_State == HIGH)
  {
    delay(1500);
    lcd.clear();
    Serial.println("Gun is Paused....");
    pause_gun();
    lcd.clear();
    delay(1500);
  }

  if (Set_State == LOW)
  {
    Serial.println("Gun is running Continue......");
    continue_gun();
  }
}
