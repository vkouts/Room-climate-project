#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define DHTPIN 12
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

#define pwmPin 11
// #define LedPin 13

#define RedLed    9
#define YellowLed 8
#define GreenLed  7
#define BlueLed   6

#define GOOD_LVL  800
#define NORMAL_LVL 1000
#define BAD_LVL 1500

#define MAX_TAKT 5

int prevVal = LOW;
long th, tl, h, l, ppm;
float hum, temp;
int myscr = 1;
int takt = 0;
  
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.init();
  lcd.backlight();
  delay(1000);

  pinMode(pwmPin, INPUT);
  //pinMode(LedPin, OUTPUT);
  pinMode(RedLed, OUTPUT);
  pinMode(YellowLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(BlueLed, OUTPUT);
  digitalWrite(BlueLed, LOW);
  digitalWrite(GreenLed, LOW);
  digitalWrite(YellowLed, LOW);
  digitalWrite(RedLed, LOW); 

  dht.begin();
}
  
void loop() {
  long tt = millis();
  int myVal = digitalRead(pwmPin);

// Serial.println("tt = " + String(tt));
// Serial.println("myVal = " + String(myVal));

  if (myVal == HIGH) {
      if (myVal != prevVal) {
          h = tt;
          tl = h - l;
          prevVal = myVal;
      }
  }  else {
    if (myVal != prevVal) {
      l = tt;
      th = l - h;
      prevVal = myVal;
      ppm = 5000 * (th - 2) / (th + tl - 4);
      hum = dht.readHumidity();
      temp = dht.readTemperature();
      
      if (isnan(hum) || isnan(temp)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }
      
      Serial.println("CO2 = " + String(ppm));
      Serial.println("HUM = " + String(hum));
      Serial.println("TEMP = " + String(temp));

      //lcd.init();
      lcd.setCursor(0,0);

      takt += 1;
      
      if (myscr == 1) {
          lcd.print("CO2 = " + String(ppm) + "    ");
          // myscr = 0;
          lcd.setCursor(0,1);
          lcd.print("                ");
      } else {
          lcd.print("Hum = " + String(hum) + " %");
          lcd.setCursor(0,1);
          lcd.print("Temp = " + String(temp) + " *C");
          // myscr = 1;
      };

      if (takt >= MAX_TAKT) {
          takt = 0;
          if (myscr == 0) {
            myscr = 1;
          } else {
            myscr = 0;  
          }
      };

      if (ppm < GOOD_LVL) {
        digitalWrite(BlueLed, HIGH);
        digitalWrite(GreenLed, LOW);
        digitalWrite(YellowLed, LOW);
        digitalWrite(RedLed, LOW);
      };
      
      if ((ppm > GOOD_LVL) && (ppm < NORMAL_LVL)) {
         digitalWrite(BlueLed, LOW);
         digitalWrite(GreenLed, HIGH);
         digitalWrite(YellowLed, LOW);
         digitalWrite(RedLed, LOW);       
      };

       if ((ppm > NORMAL_LVL) && (ppm < BAD_LVL)) {
         digitalWrite(BlueLed, LOW);
         digitalWrite(GreenLed, LOW);
         digitalWrite(YellowLed, HIGH);
         digitalWrite(RedLed, LOW);       
      };

       if (ppm > BAD_LVL) {
         digitalWrite(BlueLed, LOW);
         digitalWrite(GreenLed, LOW);
         digitalWrite(YellowLed, LOW);
         digitalWrite(RedLed, HIGH);       
      };           
      
    }
  }

}
