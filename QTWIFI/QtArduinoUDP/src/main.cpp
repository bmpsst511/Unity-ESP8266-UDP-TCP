#include <Arduino.h>

/** WIFI LIBRARY PART**/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>
/** WIFI LIBRARY PART**/

/** WIFI分享器設定 **/
String receivedSSID, WIFI_Name, WIFI_Password, ipAddress, portNum;
int confirmHead, wifinameIndex, passwordIndex, ipIndex, portIndex;
int port; //For Left Hand
WiFiUDP Client;
/** WIFI分享器設定 **/

#define LED D0 // Led in NodeMCU at pin GPIO16 (D0).

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);    // LED pin as output.
  digitalWrite(LED, HIGH); //HIGH is LED off
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    //receivedSSID = Serial.read(); //只讀第一個Byte
    //receivedSSID = Serial.readString(); //讀取一整個字串 "wifinameLeeiPhonehnwl0618172.20.10.48010"
    receivedSSID = Serial.readString();//"confirmHead;LeeiPhone:hnwl0618@172.20.10.4!8010";
    confirmHead = receivedSSID.indexOf(';');
    wifinameIndex = receivedSSID.indexOf(':');
    passwordIndex = receivedSSID.indexOf('@');
    ipIndex = receivedSSID.indexOf('!');

    Serial.println(receivedSSID.substring(0,confirmHead));
    Serial.println(";");
    Serial.println(receivedSSID.substring(confirmHead+1,wifinameIndex));
    Serial.println(";");
    Serial.println(receivedSSID.substring(wifinameIndex+1,passwordIndex));
    Serial.println(";");
    Serial.println(receivedSSID.substring(passwordIndex+1,ipIndex));
    Serial.println(";");
    Serial.println(receivedSSID.substring(ipIndex+1));

    if (receivedSSID.substring(0,confirmHead) == "confirmHead")
    {
      WIFI_Name = receivedSSID.substring(confirmHead+1,wifinameIndex);
      WIFI_Password = receivedSSID.substring(wifinameIndex+1,passwordIndex);
      ipAddress = receivedSSID.substring(passwordIndex+1,ipIndex);
      portNum = receivedSSID.substring(ipIndex+1); 
      port = portNum.toInt();
      WiFiServer server(port);

      /** 上電後執行WIFI連線與顯示相關資訊**/
      WiFi.mode(WIFI_STA); 
      Serial.println("Orientation Sensor Test"); Serial.println("");
      WiFi.begin(WIFI_Name,WIFI_Password);
      Serial.println("Connecting");
    
      while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        digitalWrite(LED, LOW); //HIGH is LED on
        delay(300);
        digitalWrite(LED, HIGH); //HIGH is LED off
        delay(300);
      }
      if(WiFi.status() == WL_CONNECTED)
      {
       digitalWrite(LED, LOW); //HIGH is LED on 
      }
    
      Serial.print("Connected to "); 
      Serial.println(WIFI_Name);
      Serial.print("IP Address: "); 
      Serial.println(WiFi.localIP());
    
      // Start the UDP client
      Client.begin(port);
      /** 上電後執行WIFI連線與顯示相關資訊**/
    }
  }
}