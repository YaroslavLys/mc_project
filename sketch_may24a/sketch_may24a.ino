#include <dht.h>

#include <Keypad.h>
#include <LiquidCrystal.h>

#define outPinDHT 22  // Defines pin number to which the temperature and humidity sensor is connected
dht DHT;  


int rs = 17;
int e = 16;
int d0 = 49;
int d1 = 48;
int d2 = 47;
int d3 = 46;
int d4 = 45;
int d5 = 44;
int d6 = 43;
int d7 = 42;
int buzzer = 21;

float oldTemp = 0;
float oldHum = 0;

int sec = 0;
int minute = 0;
int hour = 0;

int secAlarm = 0;
int minuteAlarm = 0;
int hourAlarm = 0;

int secLastAlarm = 0;
int minuteLastAlarm = 0;
int hourLastAlarm = 0;

boolean timer = true;
boolean alarm = false;
boolean enterTime = false;
boolean enterAlarmTime = false;
boolean showAlarmTime = false;

String inputString = "";

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'F', '0', 'E', 'D'}
};

byte rowPins[ROWS] = {62, 63, 64, 65}; 
byte colPins[COLS] = {66, 67, 68, 69}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal lcd(rs, e, d0, d1, d2, d3, d4, d5, d6, d7); // sets the interfacing pins

void printTime(int hour, int minute, int sec, boolean alarm){
  lcd.clear();
  if(hour < 10){
      lcd.print("0");
    }
    lcd.print(hour);  
    lcd.print(":");
    if(minute < 10){
      lcd.print("0");
    }
    lcd.print(minute);
    lcd.print(":");
    if(sec < 10){
      lcd.print("0");
    }
    lcd.print(sec);
    lcd.print("    ");
    if(alarm){
      lcd.print((char) 42);
    }
}

ISR(TIMER1_COMPA_vect)
{
  if(timer){
    if(sec == 60){
      sec = 0;
      minute++;
    }
    if(minute == 60){
      minute = 0;
      hour++;
    }
    if(hour == 24){
      hour = 0;
    }
    if(!showAlarmTime){
      printTime(hour, minute, sec, alarm);
    } else {
      printTime(hourAlarm, minuteAlarm, secAlarm, false);
    }
    sec++;
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(buzzer, OUTPUT);
  
  noInterrupts();

  TCCR1A = 0x00;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TIMSK1 = (1 << OCIE1A);
  OCR1A = 5812.5;

  interrupts();
}

void inputAlarmTime(char key){
  if (key >= '0' && key <= '9') {
      inputString += key;
      lcd.clear();
      lcd.print(inputString);
  } 
  else if (key == 'E') {
    if (inputString.length() > 5) {      
        delay(110);        
        hourAlarm = inputString.substring(0,2).toInt();
        if(hourAlarm > 24){
          hourAlarm = 23;
        }        
        minuteAlarm = inputString.substring(2,4).toInt();
        if(minuteAlarm > 59){
          minuteAlarm = 59;
        }     
        secAlarm= inputString.substring(4).toInt();
        if(secAlarm > 59){
          secAlarm = 59;
        }        
        inputString = "";
        timer = !timer;
        enterAlarmTime = !enterAlarmTime;  
     }      
  } 
  else if (key == 'F') {     
    inputString = ""; 
    timer = !timer;
    enterAlarmTime = !enterAlarmTime;                  
  }
}

void inputTime(char key){
  if (key >= '0' && key <= '9') {
      inputString += key;
      lcd.clear();
      lcd.print(inputString);
  } 
  else if (key == 'E') {
    if (inputString.length() > 5) {        
        delay(200);        
        hour = inputString.substring(0,2).toInt();
        if(hour > 24){
          hour = 23;
        }        
        minute = inputString.substring(2,4).toInt();
        if(minute > 59){
          minute = 59;
        }     
        sec = inputString.substring(4).toInt();
        if(sec > 59){
          sec = 59;
        }        
        inputString = "";
        timer = !timer;
        enterTime = !enterTime;  
     }      
  } 
  else if (key == 'F') {
    //tone(buzzer, 5500, 500);
    inputString = ""; 
    timer = !timer;
    enterTime = !enterTime;                  
  }  
}

void loop() {
  int readData = DHT.read11(outPinDHT);
  float t = DHT.temperature;  
  float h = DHT.humidity; 
  
  if (t != oldTemp || h != oldHum) { 
    Serial.print(t);
    Serial.print(h);
  
    if (hourAlarm >= 10) {
      Serial.print(hourAlarm);
    } else {
      Serial.print("0");
      Serial.print(hourAlarm);
    }
  
    if (minuteAlarm >= 10) {
      Serial.print(minuteAlarm);
    } else {
      Serial.print("0");
      Serial.print(minuteAlarm);
    }
  
    if (secAlarm >= 10) {
      Serial.print(secAlarm);
    } else {
      Serial.print("0");
      Serial.print(secAlarm);
    }
    
    if (hour >= 10) {
      Serial.print(hour);
    } else {
      Serial.print("0");
      Serial.print(hour);
    }
  
    if (minute >= 10) {
      Serial.print(minute);
    } else {
      Serial.print("0");
      Serial.print(minute);
    }
  
    if (sec >= 10) {
      Serial.print(sec);
    } else {
      Serial.print("0");
      Serial.print(sec);
    }

    if (hourLastAlarm >= 10) {
      Serial.print(hourLastAlarm);
    } else {
      Serial.print("0");
      Serial.print(hourLastAlarm);
    }
  
    if (minuteLastAlarm >= 10) {
      Serial.print(minuteLastAlarm);
    } else {
      Serial.print("0");
      Serial.print(minuteLastAlarm);
    }
  
    if (secLastAlarm >= 10) {
      Serial.print(secLastAlarm);
    } else {
      Serial.print("0");
      Serial.print(secLastAlarm);
    }
  
    Serial.println("");
    oldTemp = t;
    oldHum = h;
  }

   
  char customKey = customKeypad.getKey();
  if (customKey){
    //tone(buzzer, 1500, 100);
  }
  // Enter clock time
  if (customKey == 'A'){  
    timer = !timer;
    enterTime = !enterTime;  
  }
  if (enterTime && customKey){
     inputTime(customKey);
  }

  // Enter alarm time
  if (customKey == 'B'){   
    timer = !timer;
    enterAlarmTime = !enterAlarmTime; 
  }
  if (enterAlarmTime && customKey){
     inputAlarmTime(customKey);
  }

  // Show alarm time
  if (customKey == 'C'){
    showAlarmTime = !showAlarmTime;
  } 

  // Enable/disable alarm
  if (customKey == 'D'){
    alarm = !alarm;
  }   

  // Alarm
  if((hour == hourAlarm && minute == minuteAlarm)&& sec == secAlarm && alarm){
    tone(buzzer, 2500, 4000);
    secLastAlarm = secAlarm;
    minuteLastAlarm = minuteAlarm;
    hourLastAlarm = hourAlarm;
  }
}
