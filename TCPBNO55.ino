
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

int ch1 =3;
int ch2 =4;
int ch3 =2;

const char* ssid =/*"LeeiPhone";*/ "dlink";
const char* password =/*"hnwl0618";*/"468255000";
String PoseX, PoseY, PoseZ, PoseX2, PoseY2, PoseZ2, PoseX3, PoseY3, PoseZ3;
 /* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (1000)

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_BNO055 bno2 = Adafruit_BNO055(55);
Adafruit_BNO055 bno3 = Adafruit_BNO055(55);

  char*         TCPssid;
  char*         TCPpassword;
  IPAddress     TCPServer(192,168,1,113);
  WiFiClient    TCPClient;
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
  Serial.println("Orientation Sensor Test"); Serial.println("");
  Wire.begin();
 
  Serial.println("Connecting");
 
   WiFi.mode(WIFI_STA);            // To Avoid Broadcasting An SSID
    WiFi.begin(ssid,password);          // The SSID That We Want To Connect To

    // Printing Message For User That Connetion Is On Process ---------------
    Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

     // WiFi Connectivity ----------------------------------------------------
    CheckConnectivity();            // Checking For Connection

  // Printing IP Address --------------------------------------------------
    Serial.println("Connected To      : " + String(WiFi.SSID()));
    Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Server IP Address : ");
    Serial.println(TCPServer);
    Serial.print  ("Device IP Address : ");
    Serial.println(WiFi.localIP());
// Conecting The Device As A Client -------------------------------------
 
   /* Initialise the sensor */
    enableMuxPort(ch1); //Tell mux to connect to port X
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
    disableMuxPort(ch1);
    
  //Initialize the sensor2
    enableMuxPort(ch2); //Tell mux to connect to port X
  if(!bno2.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
    disableMuxPort(ch2);

     //Initialize the sensors3
  //  enableMuxPort(ch3); //Tell mux to connect to port X
//  if(!bno3.begin())
 // {
    /* There was a problem detecting the BNO055 ... check your connections */
 //   Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
 //   while(1);
 // }
  //  disableMuxPort(ch3);
    
    /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
   
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
}
 
void loop() {
    if(TCPClient.connect(TCPServer, 27))
    {     
   // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
  // Listen for connecting clients


/* Get a new sensor event */
  enableMuxPort(ch1); //Tell mux to connect to this port, and this port only
  sensors_event_t event;
  bno.getEvent(&event);
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
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

  Serial.print((int)euler.x());Serial.print(";");

  Serial.print((int)euler.y());Serial.print(";");

  Serial.print((int)euler.z());Serial.print("\t");

  PoseX = (int)euler.x();
    PoseY = (int)euler.y();
      PoseZ = (int)euler.z();

  /* Also send calibration data for each sensor. */
  uint8_t sys, gyro, accel, mag = 3;
  bno.getCalibration(&sys, &gyro, &accel, &mag);

  disableMuxPort(ch1); //Tell mux to disconnect from this port
//
   enableMuxPort(ch2); //Tell mux to connect to this port, and this port only
   bno2.getEvent(&event);
   imu::Vector<3> euler2 = bno2.getVector(Adafruit_BNO055::VECTOR_EULER);
   /* The processing sketch expects data as roll, pitch, heading */

  Serial.print((int)euler2.x());Serial.print(";");

  Serial.print((int)euler2.y());Serial.print(";");

  Serial.print((int)euler2.z());Serial.print("\t");
  PoseX2 = (int)euler2.x();
    PoseY2 = (int)euler2.y();
      PoseZ2 = (int)euler2.z();
  /* Also send calibration data for each sensor. */
  bno2.getCalibration(&sys, &gyro, &accel, &mag);

  disableMuxPort(ch2); //Tell mux to disconnect from this port

//enableMuxPort(ch3); //Tell mux to connect to this port, and this port only
    // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
  /* Get a new sensor event */
 // bno3.getEvent(&event);
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
//  Serial.print((float)event.orientation.x);Serial.print(";");
 // Serial.print(F(" "));
//  Serial.print((float)event.orientation.y);Serial.print(";");
 // Serial.print(F(" "));
 // Serial.print((float)event.orientation.z);Serial.print("\t");
 // Serial.println(F(""));
 // PoseX3 = (float)event.orientation.x;
  //  PoseY3 = (float)event.orientation.y;
   //   PoseZ3 = (float)event.orientation.z;
 
  /* Also send calibration data for each sensor. */
//  bno3.getCalibration(&sys, &gyro, &accel, &mag);
 /* Serial.print(F("Calibration: "));
  Serial.print(sys, DEC);
  Serial.print(F(" "));
  Serial.print(gyro, DEC);
  Serial.print(F(" "));
  Serial.print(accel, DEC);
  Serial.print(F(" "));
  Serial.println(mag, DEC);*/
  Serial.print("\r\n");
//   disableMuxPort(ch3); //Tell mux to disconnect from this port

       // Send the distance to the client, along with a break to separate our messages
  //TKDClient.print(PoseX+";"+PoseY+";"+PoseZ+";"+PoseX2+";"+PoseY2+";"+PoseZ2);
  TCPClient.print(PoseX+";"+PoseY+";"+PoseZ);
  delay(BNO055_SAMPLERATE_DELAY_MS);
 } 
}
//====================================================================================

  void CheckConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.println(".");
    }
  }
  
  

//====================================================================================
