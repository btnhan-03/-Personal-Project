#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <Wire.h>
#include<FirebaseESP32.h>
#include<WiFi.h>

#define FIREBASE_HOST "iot2024-d856d-default-rtdb.firebaseio.com"    
#define FIREBASE_AUTH "kxmmPaQHFot4OZaNtcCjjqCeg8xD89bbWQdEfC9U"                             
#define WIFI_SSID "nhancute"
#define WIFI_PASSWORD "nhan123456"

LiquidCrystal_I2C lcd(0x27,20,4);
#define DHTTYPE DHT11
#define DHTPIN 0
FirebaseData   firebase;
DHT dht(DHTPIN, DHTTYPE);
const int cambien=2;
const int led_2=33;
const int led_1=32;

void setup(){
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(5,0);
  lcd.print("SMART HOME");
  lcd.setCursor(13,2);
  lcd.print("FAN:");
  pinMode(led_2,OUTPUT);
  dht.begin();
  pinMode(led_1,OUTPUT);

  Serial.begin(9600);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
   
}
void loop(){
  float h = dht.readHumidity();    //Đọc độ ẩm
  float t = dht.readTemperature(); //Đọc nhiệt độ
  Firebase.setFloat(firebase,"DHT/TemC",t);
  Firebase.setFloat(firebase,"/DHT/TemH",h);
  lcd.setCursor(0,1);
  lcd.print("Nhiet Do:");
  lcd.print(t);
  lcd.print("C");
  lcd.setCursor(0,2);
  lcd.print("Do Am:");
  lcd.print(h);
  if(t>20){
    digitalWrite(led_2, HIGH);
    Firebase.setString(firebase,"/DHT/FAN","ON");
    lcd.setCursor(18,2);
    lcd.print("ON   ");
    delay(500);
  }else{
    digitalWrite(led_2, LOW);
    Firebase.setString(firebase,"/DHT/FAN","OFF");
    lcd.setCursor(18,2);
    lcd.print("OFF");
    delay(500);
  }
  int nut_nhan= digitalRead(cambien);
 if(nut_nhan==1||Firebase.getInt(firebase,"/IoT_2024/LED")==1){
    digitalWrite(led_1, HIGH);
    Firebase.setInt(firebase,"/IoT_2024/LED",1);
    lcd.setCursor(0,3);
    lcd.print("Co Vat Can   ");
  }else{
    digitalWrite(led_1, LOW);
    Firebase.setInt(firebase,"/IoT_2024/LED",0);
    lcd.setCursor(0,3);
    lcd.print("Ko Co Vat Can");
  }

}
