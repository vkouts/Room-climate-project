#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define DHTPIN 12
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

#define pwmPin 11
// #define LedPin 13

#define RedLed    9
#define YellowLed 8
#define GreenLed  7
#define BlueLed   6

#define OLED_MOSI  3
#define OLED_CLK   2
#define OLED_DC    5
#define OLED_CS    1
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define GOOD_LVL  800
#define NORMAL_LVL 1000
#define BAD_LVL 1500

int prevVal = LOW;
long th, tl, h, l, ppm;
  
void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  
  display.display();
  delay(2000);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Start");
  display.display();

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
  display.clearDisplay();
  display.setCursor(0,0);
  
  long tt = millis();
  int myVal = digitalRead(pwmPin);

  //Если обнаружили изменение
  if (myVal == HIGH) {
    // digitalWrite(LedPin, HIGH);
    if (myVal != prevVal) {
      h = tt;
      tl = h - l;
      prevVal = myVal;
    }
  }  else {
    // digitalWrite(LedPin, LOW);
    if (myVal != prevVal) {
      l = tt;
      th = l - h;
      prevVal = myVal;
      ppm = 5000 * (th - 2) / (th + tl - 4);

      float hum = dht.readHumidity();
      float temp = dht.readTemperature();
      if (isnan(hum) || isnan(temp)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }
      
      Serial.println("PPM = " + String(ppm));
      display.print("CO2 = ");
      display.println(String(ppm));
      
      display.print("Hum = ");
      display.print(String(hum));
      display.println(" %\t");
    
      display.print("Temp = ");
      display.print(String(temp));
      display.println(" *C"); 
    
      display.display();
       delay(2000);

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
