#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/***** Setting Wi-Fi account and password and internet port *****/
        const char* ssid ="TP-LINK_A7366A";//"dlink";
        const char* password ="03487150";//"468255000";
        const char ip[]="192.168.0.101";//"192.168.1.111";
        const int port =8001;//客戶端端口
/***** Setting Wi-Fi account and password and internet port *****/
        WiFiServer server(port);
        WiFiUDP Client;


const int packetSize = 3;
byte packetBuffer[packetSize];


void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);//WIFI_STA : Client 模式
  Serial.println("Orientation Sensor Test"); Serial.println("");
  WiFi.begin(ssid,password);
  Serial.println("Connecting");
 
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.print("Connected to "); 
  Serial.println(ssid);
  Serial.print("IP Address: "); 
  Serial.println(WiFi.localIP());
 
 /*Start the UDP client*/
  Client.begin(port);//啟始客戶端端口
}

//處理從伺服端發送來的數據
void handleUDPServer() {
  int cb = Client.parsePacket();
  if (cb) {
    Client.read(packetBuffer, packetSize);
    String myData = ""; 
    for(int i = 0; i < packetSize; i++) {
      myData += (char)packetBuffer[i];
    }
    Serial.println(myData);
  }
}
//處理從伺服端發送來的數據

void loop()
{
  handleUDPServer();
   delay(1);
  }
