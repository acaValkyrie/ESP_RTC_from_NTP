#include <Arduino.h>
#include <esp_sntp.h>
#include <TimeLib.h>
#include <WiFi.h>


const char* ntpServer = "ntp.nict.jp";
// JST: 9hours late. 9h = 32400s
const long  gmtOffset_sec = 32400;
const int   daylightOffset_sec = 0;

void WifiSyncJST()
{
  //---------内蔵時計のJST同期--------
  // WiFi接続
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  int count = 0;
  while (WiFi.status() != WL_CONNECTED){
    if(count % 10 == 0){
      Serial.print(".");
    }
    count++;
  }
  Serial.println("Connected");
  // NTPサーバからJST取得
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(1000);
  // 内蔵時計の時刻がNTP時刻に合うまで待機
  Serial.println("Waiting for NTP time sync");
  while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET)
  {
    delay(1000);
  }
  Serial.println("NTP time synced");
  // WiFi切断
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

struct tm *tm;

void setup()
{
  Serial.begin(9600);
  WifiSyncJST();
}

void PrintTm(struct tm);

void loop()
{
  time_t now = time(NULL);
  tm = localtime(&now);
  PrintTm(tm);
}

void PrintTm(struct tm){
  int month, day, hour, minute, second;
  month = tm->tm_mon + 1;
  day = tm->tm_mday;
  hour = tm->tmHour;
  minute = tm->tm_min;
  second = tm->tm_sec;
  Serial.printf("%d/%d %d:%d:%d\n", month, day, hour, minute, second);
}