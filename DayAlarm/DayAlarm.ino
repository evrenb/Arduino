#include <TimeLib.h>

const int buttonPin = 2;
int buttonState = 0;
unsigned long pressTime = 0;
int longButtonTime = 2000;
byte buttonPressed = 0;

int inputDate = 0;
int inputHour = 0;
int alarmDay=0;
int alarmHour=0;

byte state = 0; 
int tempVar = 0;

int alarmActive = 0;
int lastAlarmDay = 0;
/*
 * 0 = regular
 * 1 = set date
 * 2 = set hour
 * 3 = set alarm date
 * 4 = set alarm hour
 * 5 = show info
 * 6 = alarm
 */

int led = 13;
unsigned long previousLightLed = 0;
unsigned long previousAlarmCheck = 0;

void setup() 
{
  //Serial.begin(9600);
  setTime(00,00,01,01,01,2006);
  pinMode(buttonPin, INPUT);
  pinMode(led, OUTPUT); 

  //PrintTime();
}

void loop() {
  CheckButton();
  if(state == 6) LightLed(1);
  if(state == 0) CheckAlarm();
}

void CheckAlarm()
{
  if(millis()-previousAlarmCheck>300000)
  {
    if(weekday() == alarmDay && hour() == alarmHour && lastAlarmDay != day())
    {
      lastAlarmDay = day();
      state = 6;
    }
    previousAlarmCheck = millis();
  }
}

void CheckButton()
{
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)
  {
    if(buttonPressed == 0)
    {
      buttonPressed = 1;
      pressTime = millis();
    }
    if(buttonPressed == 1)
    {
      if(millis()-pressTime>longButtonTime) 
      {     
        LongButton();
        buttonPressed = 2;
      }   
    }
  }
  else
  {
    if(buttonPressed == 1)
    {
      ShortButton(); 
      buttonPressed = 0;
    }
    if(buttonPressed == 2)
    {
      buttonPressed = 0;
    } 
  }
}

void LongButton()
{
  Serial.println("Long Button");
  switch(state)
  {
    case 0:
      tempVar = 0;
      state++;
      break;
    case 1:
      inputDate = tempVar;
      tempVar = 0;
      state++;
      break;
    case 2:
      inputHour = tempVar;
      setTime(inputHour,00,01,inputDate,01,2006);
      tempVar = 0;
      state++;
      break;
    case 3:
      alarmDay = tempVar;
      tempVar = 0;
      state++;
      break;
    case 4:
      alarmHour = tempVar;
      tempVar = 0;
      state = 0;
      SignLongButton();
      break;
  }
  SignLongButton();
}
void ShortButton()
{
  Serial.println("Short Button");
  switch(state)
  {
    case 0:
      state = 5;
      ShowTime();
      break;
    case 6:
      LightLed(0);
      state = 0;
      break;
    default:
      tempVar++;
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      break;
  }
}

void SignLongButton()
{
  for(int i=0; i<4 ; i++)
  {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
}

void LightLed(int isOn)
{
  if(isOn == 1 && millis()-previousLightLed>1000)
  {
    int ledState = digitalRead(led);
    digitalWrite(led, !ledState);
    previousLightLed = millis();
  }
  if(isOn == 0)
  {
    digitalWrite(led, LOW);
  }
}

void ShowTime()
{
  //PrintTime();
  SignLongButton();
  delay(1000);
  for(int i=0; i<weekday() ; i++)
  {
    digitalWrite(led, HIGH);
    delay(800);
    digitalWrite(led, LOW);
    delay(800);
  }  
  SignLongButton();
  delay(1000);
  for(int i=0; i<hour() ; i++)
  {
    digitalWrite(led, HIGH);
    delay(800);
    digitalWrite(led, LOW);
    delay(800);
  }  
  SignLongButton();
  delay(1000);
  for(int i=0; i<alarmDay ; i++)
  {
    digitalWrite(led, HIGH);
    delay(800);
    digitalWrite(led, LOW);
    delay(800);
  }  
  SignLongButton();
  delay(1000);
  for(int i=0; i<alarmHour ; i++)
  {
    digitalWrite(led, HIGH);
    delay(800);
    digitalWrite(led, LOW);
    delay(800);
  }  
  SignLongButton();
  state = 0;
}

//void PrintTime()
//{
//  Serial.print("Year: ");
//  Serial.println(year());
//  Serial.print("Month: ");
//  Serial.println(month());
//  Serial.print("Day: ");
//  Serial.println(day());
//  Serial.print("Weekday: ");
//  Serial.println(weekday());
//  Serial.print("Hour: ");
//  Serial.println(hour());
//  Serial.print("Min: ");
//  Serial.println(minute());
//  Serial.print("Sec: ");
//  Serial.println(second());
//  Serial.print("AlarmDay: ");
//  Serial.println(alarmDay);
//  Serial.print("AlarmHour: ");
//  Serial.println(alarmHour);
//  Serial.print("State: ");
//  Serial.println(state);
//  Serial.println("************************");
//}

