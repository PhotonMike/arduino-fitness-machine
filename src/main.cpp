#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);
int us1 = 22;

void setup() {
    //LCD
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Betoltes...");

    //Misc
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(48, OUTPUT);
    digitalWrite(48, HIGH);
    //Serial.begin(9600);

    //Ultrasonic
    pinMode(us1, OUTPUT); 
    pinMode(us1 + 1, INPUT);
    
    //Final
    //lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Start!          ");
}

int bright = 255;
int del = 20;
bool up = true;
int huzasok = 0;
bool tavol = true;
long int startmillis = 0;
unsigned long long int lastMilli = 0;

int ultrasonic(int port) {
    digitalWrite(port, HIGH);
    delayMicroseconds(10);
    digitalWrite(port, LOW);
    return pulseIn(port + 1, HIGH)*0.034/2;
}

double calcSpeed() {
    //return (((double) huzasok / (millis()-startmillis)));
    return ((double) huzasok / (millis()-startmillis)) * 1000;
}

void writeLcd() {
    lcd.setCursor(0,0);
    lcd.print(huzasok);
    lcd.print(" huzas");

    lcd.setCursor(0,2);
    lcd.print(calcSpeed());
    lcd.print(" huzas/mp      ");
}

void waitForAction(){
    while(ultrasonic(us1) > 10){
        if (millis()-lastMilli > 30000) {
            lcd.setBacklight(LOW);
        }
        tavol = true;
        delay(20);
    }
}

void loop() {
    waitForAction();
    if (startmillis == 0) {
        startmillis = millis();
    }
    if (tavol) {
        huzasok++;
        lastMilli = millis();
        lcd.setBacklight(HIGH);
        writeLcd();
        tavol = false;
        delay(250);
    }
}