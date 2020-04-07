#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "TP-LINK_A7366A";
const char* password = "03487150";

WiFiUDP UDPTestServer;
unsigned int UDPPort = 2807;//定義此伺服機端口

const int packetSize = 3;
byte packetBuffer[packetSize];

void setup() {
  Serial.begin(9600);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  WiFi.config(IPAddress(192, 168, 0, 102), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));//定義此伺服機的IP位址
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  UDPTestServer.begin(UDPPort);//啟動伺服機端口
  
}

int value = 0;

//接收與處理從客戶端發送來的數據
void handleUDPServer() {
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer, packetSize);
    String myData = ""; 
    for(int i = 0; i < packetSize; i++) {
      myData += (char)packetBuffer[i];
    }
    Serial.println(myData);
  }
}
//接收與處理從客戶端發送來的數據

void loop() {
   handleUDPServer();
   delay(1);
}
