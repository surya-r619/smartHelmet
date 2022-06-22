#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPLGyMt_Q--"
#define BLYNK_DEVICE_NAME "smart helmet"
#define BLYNK_AUTH_TOKEN "3MkEBGHK71xdEgOwMUaKcpKB1xB70f_N"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial sgps(D5, D6);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Young brother";
char pass[] = "11111111";

int Gas_pin = D7;
float temp;
float temp_pin = A0;
int vibration_pin = D4;
const int buttonPin = D1;
int buttonState = 0;

float slat, slon;
int a = 0;
int l = 0;

void setup()
{
  // Debug console
  Serial.begin(9600);
  sgps.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(Gas_pin, INPUT);
  pinMode(vibration_pin, INPUT);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  while (slat == 0 && slon == 0) {
    getGPS();
    Blynk.virtualWrite(V3, slat);
    Blynk.virtualWrite(V4, slon);
  }
  get_temp();
  gasread_Gas();
  vib_status();
  switch_stat();

}

void get_temp() {
  temp = analogRead(temp_pin);
  temp = temp * 0.48828125;
  temp = temp / 2;
  Blynk.virtualWrite(V5, temp);
}

void gasread_Gas() {
  if (!digitalRead(Gas_pin)) {
    Blynk.virtualWrite(V1, "Alcohol Detected");
    a = 1;

  } else {
    Blynk.virtualWrite(V1, "....");
    a = 0;
  }
}


void vib_status() {
  if (!digitalRead(vibration_pin)) {
    Blynk.virtualWrite(V0, "Crash Detected!");

  } else {
    Blynk.virtualWrite(V0, "....");

  }
}

void switch_stat() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == 0) {
    Blynk.virtualWrite(V2, "High");
    if (a == 0) {
      Serial.println("2");
      l = 1;
    }
  } else {
    Blynk.virtualWrite(V2, "....");
    Serial.println("1");
    l = 0;
  }
}

void getGPS() {
  while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&slat, &slon);
    }
  }
  delay(50);
}

BLYNK_WRITE(V6) {
  int val = param.asInt();
  if (val) {
    Serial.println(2);
  }
}

BLYNK_WRITE(V7) {
  int val = param.asInt();
  if (val) {
    Serial.println(1);
  }
}
