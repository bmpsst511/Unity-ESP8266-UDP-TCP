#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55);
String PoseX, PoseY, PoseZ;
const uint16_t port = 8000;
const char * host = "192.168.1.101"; // ip or dns
const char* ssid = "dlink";
const char* password = "468255000";

ESP8266WiFiMulti WiFiMulti;
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
}

void setup() {
  Serial.begin(115200);
   Serial.println("Orientation Sensor Test"); Serial.println("");
  // put your setup code here, to run once:
WiFiMulti.addAP(ssid, password);
Serial.println();
Serial.print("Wait for WiFi...");

while(WiFiMulti.run() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
Serial.println("");
Serial.println("WiFi Connected");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
delay(500);

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
   
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop() {
  //這邊192.168.1.101是我後端要記錄的server
// Use WiFiClient class to create TCP connections
WiFiClient client;
if (client.connect(host, port)) {
      /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  /* Board layout:
         +----------+
         |         *| RST   PITCH  ROLL  HEADING
     ADR |*        *| SCL
     INT |*        *| SDA     ^            /->
     PS1 |*        *| GND     |            |
     PS0 |*        *| 3VO     Y    Z-->    \-X
         |         *| VIN
         +----------+
  */

  /* The processing sketch expects data as roll, pitch, heading */
  //Serial.print(F("Orientation: "));
  Serial.print((float)event.orientation.x);Serial.print(";");
 // Serial.print(F(" "));
  Serial.print((float)event.orientation.y);Serial.print(";");
 // Serial.print(F(" "));
  Serial.print((float)event.orientation.z);Serial.print("\t");
 // Serial.println(F(""));
  PoseX = (String)(float)event.orientation.x;
    PoseY = (String)(float)event.orientation.y;
      PoseZ =(String) (float)event.orientation.z;
  /* Also send calibration data for each sensor. */
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
 /* Serial.print(F("Calibration: "));
  Serial.print(sys, DEC);
  Serial.print(F(" "));
  Serial.print(gyro, DEC);
  Serial.print(F(" "));
  Serial.print(accel, DEC);
  Serial.print(F(" "));
  Serial.println(mag, DEC);*/
  Serial.print("\r\n");
  client.println(PoseX);
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
 


/*if (!client.connect(host, port)) {
Serial.println("connection failed");
Serial.println("wait 5 sec...");
delay(5000);
return;
}*/
//client.println(PoseX);
//Serial.println("Close");
//client.stop();
}
