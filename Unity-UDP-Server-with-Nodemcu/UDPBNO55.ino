
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>
 #include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>

Servo myservo;  // 建立一個 servo 物件，最多可建立 12個 servo
int pos = 0;    // 設定 Servo 位置的變數
int TouchSensor = D0; //connected to Digital pin D0
const char* ssid =/*"dlink";*//*"iPhone"*/"My ASUS";
const char* password = /*"468255000";*//*"19940625"*/"9361adb34b14";
String PoseX, PoseY, PoseZ, On;
 /* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55);
WiFiServer server(27);
WiFiUDP Client;

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx"); 
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); 
  Serial.println("Orientation Sensor Test"); Serial.println("");
  pinMode(TouchSensor, INPUT);
  myservo.attach(D6);  // 將 servo 物件連接到 pin 7
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
 
  // Start the UDP client
  Client.begin(27);
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

/* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);
  imu::Vector<3> linAcc = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
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
  PoseX = (float)event.orientation.x;
    PoseY = (float)event.orientation.y;
      PoseZ = (float)event.orientation.z;
  /* Also send calibration data for each sensor. */
  uint8_t sys, gyro, accel, mag = 3;
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
  if(digitalRead(TouchSensor)==HIGH)       //Read Touch sensor signal
   { 
    On = "1";
   for(int i = 0; i <= 180; i+=1){
    myservo.write(i); // 使用write，傳入角度，從0度轉到180度
  }
   }
  else
   {
    On = "0";
     myservo.write(0); // 使用write，傳入角度，從0度轉到180度
   }
         // Send the distance to the client, along with a break to separate our messages
          const char ip[]="192.168.43.157";
        Client.beginPacket(ip,27);
        Client.println(PoseX+";"+PoseY+";"+PoseZ+";"+On);
        Client.endPacket();
          delay(BNO055_SAMPLERATE_DELAY_MS);
  }
