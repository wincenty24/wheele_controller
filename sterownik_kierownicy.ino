#include <LiquidCrystal_I2C.h> // Library for LCD
#include <Wire.h>
#include <Math.h>
#include <JC_Button.h>
#include "MegunoLink.h"
#include "Filter.h"


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
float val = 0;
int prepared_val =0;
unsigned long aktualnyCzas = 0;
unsigned long zapamietanyCzasLED1 = 0;


long FilterWeight = 90;
ExponentialFilter<float> ADCFilter(50, 0);

void setup() {
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  lcd.begin(16, 2); // LCD 2x16 initialization
  lcd.backlight();
  Serial.begin(9600);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH); 
  Serial.setTimeout(10);
}

void lcd_print(float values, float val2, String info) {
  //Jeśli różnica wynosi ponad 1 sekundę
  if (aktualnyCzas - zapamietanyCzasLED1 >= 100UL) {
    zapamietanyCzasLED1 = aktualnyCzas;
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("spd:");
    lcd.setCursor(4, 0);
    lcd.print(values);
    
    lcd.setCursor(0, 1);
    lcd.print("spd:");
    lcd.setCursor(4, 1);
    lcd.print(val2);

//0.894276113100107 0.303268358466657 0.704195140454861 
  }
}
void checker(float ref, float act){
  
  if(ref > act){

    digitalWrite(4, LOW);
    digitalWrite(5, HIGH); 
  }else if(ref < act ){
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);   
  }
  else{
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH); 
  }
}
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void process_info(String info) {
  char data[200];
  info.toCharArray(data, info.length());
  char* splitted_data = strtok(data, " ");
  int index = 0;
  while (splitted_data != NULL) {
    if (index == 0) {
      Serial.println(splitted_data);
      val = atof(splitted_data);
    }
    splitted_data = strtok(NULL, " ");
    index++;
  }
}
void loop() {
  aktualnyCzas = millis();
  int x = mapf(analogRead(A0), 0, 1023,-50,50);
  int y = mapf(analogRead(A1), 0, 1023,-50,50);
   ADCFilter.Filter(y);
   //Serial.println(y);
   ADCFilter.SetWeight(x);
  checker(val, y);
String info;
  if (Serial.available()) {
    info = Serial.readString();
    process_info(info);
    prepared_val = val;
    digitalWrite(13, HIGH);
  }else{
  //digitalWrite(13, LOW);
  }
    lcd_print(val, y, info);
  
  // put your main code here, to run repeatedly:

}
