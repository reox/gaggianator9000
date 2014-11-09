// include the library code:
#include <LiquidCrystal.h>
#include <stdio.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
    Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  
  cli();//stop interrupts
  
  // timer1 is set to 200Hz (LCD refresh)
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 6250;            // compare match register 16MHz/256/100Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
sei();//allow interrupts

}

int v[8] = {0,0,0,0,0,0,0,0};
uint8_t c = 0;
int ana_s = 0;

boolean pump_on = false;
boolean magn_on = false;
int timer = 0;

int timermax = 25*1000;

char line0[16];
char line1[16];

int watts = 1200;

int hours = 0;
int minutes = 0;
int seconds = 0;

// second row:
// 5 chars 

void loop() {
  // read the next analog value
  v[(c++)&7] = analogRead(4);
  
  if(c == 0){
    int x = (v[0]+v[1]+v[2]+v[3]+v[4]+v[5]+v[6]+v[7]) >> 3;

    hours = (millis() / 1000) / 60 / 60;
    minutes = (millis() / 1000 / 60) % 60;
    seconds = (millis() / 1000) % 60;  
    sprintf(line0, "%02u:%02u:%02u   %04uW", hours, minutes, seconds, watts);
    sprintf(line1, "%04u", x);

  
    if(x > 330){
      pump_on = true;
    }
    else{
      pump_on = false;
    }
  }
 
}

ISR(TIMER1_COMPA_vect){
  // refresh the lcd text
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(line0);
  lcd.setCursor(0,1);
  lcd.write(line1);

}
