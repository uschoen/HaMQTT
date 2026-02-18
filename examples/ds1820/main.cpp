/* This example demonstrates how to use the HaMqtt class. 
   This example is not intended for production use! 
   It is only meant to illustrate how the class works.
*/
#include <Arduino.h>
/* Serial debug in HaMqtt and start the Serial interface "Serial.begin(115200);""
   for platformio use :
        build_flags = '-D DEBUG'
*/
  #ifndef DEBUG
    #define DEBUG
  #endif
/* The maximum number of sensors required in HaMqtt.
   Please use sparingly due to memory constraints. If too many sensors are created or 
   not properly deleted, addHASensor() will return -1. 
   for platformio use :
        build_flags = '-D MAX_SENSORS=2'
*/
  #ifndef MAX_SENSORS
    #define MAX_SENSORS 1
  #endif

/* Wi-Fi Connection or some other Network Connection. */
  #ifdef ESP32
    #include <WiFi.h>         // Lib for ESP32
  #else
    #include <ESP8266WiFi.h>  // Lib for ESP8266
  #endif
/* Wi-Fi credentials. */
  #ifndef STASSID
    #define STASSID "FS-INT"
    #define STAPSK "FamSch03n"
  #endif

/* Wi-Fi client, or the Ethernet connection if you are using one. */
  WiFiClient client;
  const char* ssid = STASSID;
  const char* password = STAPSK;

/* Home Assistant Lib*/
  #include <HaMqtt.h>
  HaMqtt HAClass;

/* Include the libraries we need for ds18b20 temperatur sensor.*/
  #include <OneWire.h>
  #include <DallasTemperature.h>
/* Data wire is plugged into port 2 on the Arduino. */
  #define ONE_WIRE_BUS 2
/* Setup a OneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs.)*/
  OneWire oneWire(ONE_WIRE_BUS);
/* Pass our oneWire reference to Dallas Temperature.*/
  DallasTemperature sensors(&oneWire);

/* Helper to convert a ds18b20 Device Address to a String*/
  String addressToString(DeviceAddress deviceAddress) {
  String str = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) str += "0"; // Leading zero for values less than 0x10
    str += String(deviceAddress[i], HEX);
  }
  return str;
}

void setup() {
  // Start serial for debug.
    Serial.begin(115200);
    while (!Serial) {};
    Serial.println("Debugging is enabled"); 
 
  // Start Wifi connection.
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  
  /* Define a unique node_id for the esp device and name.
     if no node_id or name set, the HA Class generate a random node_id and name.
  */
    // 
    String node_id=WiFi.macAddress();         // 43:33:45:33:21:ff , unique name are the MAC address 
    node_id.replace(":","_");                 // 43_33_45_33_21_ff, Special character ':' is not allowed; replaced by '_'.
    // define a human name like "Temperatur Heading"
    String deviceName="ESP_"+node_id;         // ESP_43_33_45_33_21_ff

  
  // Start the HA Class
    HAClass.begin(node_id,deviceName);
    // HAClass.begin();                         // if no node id or device name set its generade a random node_id and name
    // HAClass.setDeviceIdentifier(node_id);    // you can change the nodeID later
    // HAClass.setDeviceName(deviceName);       // you can change the device name later
    // HAClass.addHADevice(HADevice);           // or overwirte it with the HADevice struct (device data)
  

  // Add Network Connection Wifi or network  client;
    HAClass.setNetworkClient(client);

  // Setup he mqtt Server connection
    String   server="10.90.31.110";
    uint16_t port=1883;
    String   user="somemqttuser";
    String   pass="secret";
    HAClass.setMQTTServer(server,port,user,pass);
    // HAClass.setMQTTServer(server,port); // alternativ with out user and password
  
  /* Add your devcie data
     if you qwant add mor device/node information, you can add a struct (optional)
  */
    HADevice myESP;
  /*
  struct HADevice {
      // Identifikation (mandatory)
        String ids;           // unique id, if notset its use node_id from begin or setDeviceIdentifier()
        String name;          // humen name, if notset its use deviceName from begin or setDeviceName()
      // Optionale Metadaten
        String mdl;           // model: Modell name(z.B. "ESP32-WROOM-32")
        String mf;            // manufacturer: Hersteller (z.B. "Espressif" oder "Eigenbau")
        String sw;            // sw_version: Firmware-Version (z.B. "1.2.0")
        String hw;            // hw_version: Hardware-Revision (z.B. "v2.1")
        String sa;            // suggested_area: Romm (z.B. "Wohnzimmer")
        String cu;            // configuration_url: Web-Link zur Config-Seite for ESP
        String via_device;    // If the device is connected via a bridge
      // Method for easily adding connections
        void addConnection(const char* type, const char* value);
      // not for use
        ConnectionPair connections[8];  // Array for max 8 connections.
        uint8_t connectionCount = 0;       
    };
  */
  myESP.mdl="ESP32/8266";
  myESP.cu="http://"+WiFi.localIP().toString();

  // Create coennction data (max 8) and add to the device (optional)
    myESP.addConnection("ip",String(WiFi.localIP().toString()).c_str());
    myESP.addConnection("mac",WiFi.macAddress().c_str());
  // Add your data
    HAClass.addHADevice(myESP);

  // Set device as available
  // If the device unavailable, all sensor set to unavailable
  HAClass.setDeviceAvailability(true); 

  // Start up the library
  sensors.begin();

  Serial.println("setup ds1820HA");
}

void loop() {

  int HaSensorID;
  DeviceAddress thermometerADR;
  String uniquedName;
  float tempC;

  HAClass.loop(); // need in main loop()

  sensors.requestTemperatures(); // Send the command to get temperatures
  uint8_t sensorCount =sensors.getDeviceCount();
  for (int sensorID=0;sensorID<sensorCount;sensorID++){
    
    //read sensor device address
    sensors.getAddress(thermometerADR, sensorID);
    uniquedName=addressToString(thermometerADR);
    
    //create HA Sensor if not exist.
    if (HAClass.getSensorID(uniquedName.c_str())==-1){
      /*
      struct HASensor {
          // Identifikation (Mandatory)
            const char* unique_id;                           // unique ID (z.B. "sensor_temp_01","mac")
            const char* name;                                // name in HA
            const char* components=HA_COMPONENT_SENSOR;      // paylod Components
          // Sensor Details (Optional, leave blank/or not set if not required.)
            const char* dev_cla=nullptr;                     // device_class: z.B. "temperature", "humidity", "battery", "energy"
            const char* unit_of_meas=nullptr;                // Maßeinheit: z.B. "°C", "%", "kWh"
            const char* ic=nullptr;                          // icon: z.B. "mdi:thermometer"
            const char* stat_cla=nullptr;                    // state_class: z.B. "measurement", "total_increasing"
          // Payload Configuration (Optional, leave blank/or not set if not required.)
            const char* val_tpl=nullptr;                     // value_template: z.B. "{{ value_json.temperature }}"
            int exp_aft=600;                                 // expire_after: Zeit in Sek. bis Sensor "unavailable" wird
          // Numric settings
            int sugg_disp_prec = -1;                         // suggested_display_precision: Nachkommastellen (-1 = ignorieren)
        };
        */
      HASensor sensorData;
    // mandatory
      // unique name
      sensorData.unique_id=uniquedName.c_str();
      // humen name 
      sensorData.name="Some name";
      // see in HAKeyValue for the right sensor components, see HA documentaion
      sensorData.components=HA_COMPONENT_SENSOR;

    // optional
      // see in HAKeyValue for the right device class, see HA dokumentaion
      sensorData.dev_cla=HA_SENS_TEMPERATURE;

      HaSensorID=HAClass.addHASensor(sensorData);       // get the internal ID for the sensor back. -1 is a error.
      HAClass.setSensorAvailability(HaSensorID,true);   // set the sensor in HA available, with HAsensor ID,see below, its work with uniquedName
    }

    //read temperatur senor X
    tempC = sensors.getTempCByIndex(sensorID);
    if (tempC ==-127){
      // -127 means soming going wrong
      HAClass.setSensorAvailability(uniquedName.c_str(),false); // set the sensor in HA unavailable, use with uniquedName
    }else{
      HAClass.setSensorAvailability(uniquedName.c_str(),true); // set the sensor in HA available
    
      HAClass.updateSensorValue(uniquedName.c_str(),tempC); // set the Value for the temperatur in Home Assistant;
    }
    

  }
  delay(5000);
}

