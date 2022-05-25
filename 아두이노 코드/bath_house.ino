#include <DFRobot_DHT11.h>

#include <DallasTemperature.h>

#include <OneWire.h>

#include <time.h>
#include <Servo.h>
#include <Ticker.h>


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define ONE_WIRE_BUS 5
#define DHT11_PIN 4

ESP8266WiFiMulti WiFiMulti;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DFRobot_DHT11 DHT;
Servo myServo;
Ticker tickerChange;
Ticker tickerChange1;

WiFiServer server(80);
WiFiClient client;
HTTPClient http;

boolean sensor_read = 0;
char send_Data[30];
char webdate[10];
char webtime[10];
String host = "http://10.10.141.72:81";
//---------------------------------
const char* ntpServer = "pool.ntp.org";
uint8_t timeZone = 9;
uint8_t summerTime = 0; // 3600

int s_hh = 12;      // 시간 설정 변수 < 0 조건 위해 자료형 int
int s_mm = 59;
uint8_t s_ss = 45;
uint16_t s_yy = 2022;
uint8_t s_MM = 05;
uint8_t s_dd = 23;

time_t now;
time_t prevEpoch;
struct tm * timeinfo;

void get_NTP() {
  configTime(3600 * timeZone, 3600 * summerTime, ntpServer);
  timeinfo = localtime(&now);
  while (timeinfo->tm_year + 1900 == 1970) {
    Serial.println("Failed to obtain time");
    set_time();
    localtime(&now);
    return;
  }
}

void set_time() {
  struct tm tm_in;
  tm_in.tm_year = s_yy - 1900;
  tm_in.tm_mon = s_MM - 1;
  tm_in.tm_mday = s_dd;
  tm_in.tm_hour = s_hh;
  tm_in.tm_min = s_mm;
  tm_in.tm_sec = s_ss;
  time_t ts = mktime(&tm_in);
  printf("Setting time: %s", asctime(&tm_in));
  struct timeval now = { .tv_sec = ts };
  settimeofday(&now, NULL);
}

void printLocalTime() {
  if (time(&now) != prevEpoch) {
    //Serial.println(time(&now));  // 현재 UTC 시간 값 출력
    timeinfo = localtime(&now);
    int dd = timeinfo->tm_mday;
    int MM = timeinfo->tm_mon + 1;
    int yy = timeinfo->tm_year + 1900;
    int ss = timeinfo->tm_sec;
    int mm = timeinfo->tm_min;
    int hh = timeinfo->tm_hour;
    int week = timeinfo->tm_wday;

    sprintf(webdate, "%d0%d%d", yy, MM, dd);
    if (ss < 10)
    {
      sprintf(webtime, "%d%d0%d", hh, mm, ss);
    }
    else if (ss == 0)
    {
      sprintf(webtime, "%d%d00", hh, mm);
    }
    else
    {
      sprintf(webtime, "%d%d%d", hh, mm, ss);
    }

    prevEpoch = time(&now);
  }
}
//--------------------------------------

void setup() {

  Serial.begin(9600);
  // Serial.setDebugOutput(true);

  sensors.begin();
  myServo.attach(13);
  tickerChange.attach(2, sensor);
  tickerChange1.attach(10, printLocalTime);

  Serial.println();


  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    //delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("kcci4321", "kcci4321");
  get_NTP();

}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    if (sensor_read) {
      int temp, humi;
      int Water_Temp;
      DHT.read(DHT11_PIN);
      temp = DHT.temperature;
      humi = DHT.humidity;
      int water_Level;
      water_Level = analogRead(A0);
      sensors.requestTemperatures();
      Water_Temp = sensors.getTempCByIndex(0);
      if (water_Level >= 250)
        myServo.write(180);
      else myServo.write(0);
      sprintf(send_Data, "%s:%s:%d:%d:%d:%dL",
              webdate, webtime, temp, humi, water_Level, Water_Temp);
      Serial.println(send_Data);
    }
    sensor_read = 0;
  }
}
void sensor()
{
  sensor_read = !sensor_read;
}
