#ifndef HAMQTT_H
    #define HA_MQTT_H

    #include <Arduino.h>
    #include <Client.h>
    #include <PubSubClient.h>
    #include <HaKeyValue.h>
    #include <ArduinoJson.h>

    

    struct ConnectionPair {
        String type;  // z.B. "mac", "ip", "zigbee"
        String value; // ip address or mac 
    };

    struct HADevice {
        // Identifikation (Pflicht)
        String ids;
        String name;
        // Optionale Metadaten
        String mdl;           // model: Modellbezeichnung (z.B. "ESP32-WROOM-32")
        String mf;            // manufacturer: Hersteller (z.B. "Espressif" oder "Eigenbau")
        String sw;            // sw_version: Firmware-Version (z.B. "1.2.0")
        String hw;            // hw_version: Hardware-Revision (z.B. "v2.1")
        String sa;            // suggested_area: Raumvorschlag (z.B. "Wohnzimmer")
        String cu;            // configuration_url: Web-Link zur Config-Seite des ESP
        String via_device;    // Falls das Gerät über eine Bridge verbunden ist
        
        // Array for max 8 connections
        ConnectionPair connections[8];
        uint8_t connectionCount = 0;

        // Method for easily adding connections
        void addConnection(const char* type, const char* value) {
            if (connectionCount < 8) {
                connections[connectionCount] = {type, value};
                connectionCount++;
            }
        }
    };
    
    struct HASensor {
        // Identifikation (Pflicht)
       const char* unique_id;                           // unique ID (z.B. "sensor_temp_01","mac")
       const char* name;                                // name in HA
       const char* components=HA_COMPONENT_SENSOR;      // paylod Components

        // Sensor Details (Optional, leer lassen wenn nicht benötigt)
        const char* dev_cla=nullptr;                    // device_class: z.B. "temperature", "humidity", "battery", "energy"
        const char* unit_of_meas=nullptr;               // Maßeinheit: z.B. "°C", "%", "kWh"
        const char* ic=nullptr;                         // icon: z.B. "mdi:thermometer"
        const char* stat_cla=nullptr;                   // state_class: z.B. "measurement", "total_increasing"
        
        // Payload Konfiguration
        const char* val_tpl=nullptr;                    // value_template: z.B. "{{ value_json.temperature }}"
        int exp_aft=600;                                // expire_after: Zeit in Sek. bis Sensor "unavailable" wird
        const char* availability_mode="all";            // default auf all, es müssen alle status felder unter availability online sein
        //availability_string availability[2];            // topics for availability "topic": "mein_geraet_01/status",
        
        // Nummerische Einstellungen
        int sugg_disp_prec = -1;                        // suggested_display_precision: Nachkommastellen (-1 = ignorieren)
    };



    class HaMqtt
    {
        public:
            HaMqtt();
            HaMqtt(Client& networkClient);
            HaMqtt(Client& networkClient,String server, uint16_t port);
            HaMqtt(Client& networkClient,String server, uint16_t port, String user , String pass);

            virtual void begin();
            virtual void begin(String device_identifier,String deviceName); 
            
            virtual void loop();

            void addHADevice(const HADevice& device);

            int addHASensor(HASensor sensor);

            void deleteHASensor(int sensor_id);
            void deleteHASensor(const char* unique_id);

            void setNetworkClient(Client& newNetworkClient);
            void setMQTTServer(String server, uint16_t port, String user , String pass);
            
            bool setDeviceAvailability(bool available);
            bool setSensorAvailability(const char* unique_id,bool available);
            bool setSensorAvailability(int sensorID,bool available);

            /* get  sensor id for senor uniqued name back
                Arguments:
                    const char* uniqued_id : uniqued name of the sendor [3d_dd_ee_3e_21_00_ff]
                Return:
                    int: sensor ID, if sensor id not found -1
            */
            int getSensorID(const char* unique_id);

            bool updateSensorValue(const char* unique_id,const char* value);
            bool updateSensorValue(const char* unique_id,int value);
            bool updateSensorValue(const char* unique_id,float value);
            bool updateSensorValue(int sensorID,const char* value);
            bool updateSensorValue(int sensorID,int value);
            bool updateSensorValue(int sensorID,float value);
            /* Publish the sensor data to the MQTT Server
                Arguments
            */
            bool publishSensor(int sensorID);

            //Device
            void setDeviceName(String device_name){
                this->_deviceInfos.name=device_name;
            };
            void setDeviceIdentifier(String device_identifier) {
                this->_deviceInfos.ids = device_identifier;
            }
        protected:
           
        
        private:
            PubSubClient _mqttClient;                   // Mqtt Class
            String       _mqtt_server   = "";           // Mqtt Server IP
            uint16_t     _mqtt_port     = 1883;         // Mqtt Port
            String       _mqtt_user     = "";           // Mqtt User
            String       _mqtt_pass     = "";           // Mqtt Password
            uint16_t     _mqttBuffer    = 1024;         // Mqtt Buffer for topic+payload
            bool         _mqttConnected = false;
            String _currentMac;
            // Device Vars
            HADevice _deviceInfos;          // Device Data
            // Sensor Vars
            HASensor _sensors[MAX_SENSORS]; // Das Array for x Sensoren
            
            bool startup=false;                                   // if begin called, even loop ist block
            
            ulong       _blockMQTTConnectTimer=0;                 // time to block mqtt server re-connect
            const char* _lastMQTTStatus="ESP Start";              // last mqtt status

            const char* MQTTStateString(int state);
            
            // add device config to jason dokument;
            void appendDeviceToConfig(JsonDocument& doc);

            // get sensor config paylod
            void getSensorConfig(JsonDocument& doc,int sensorID);

            // Device information for Home Assistant Discovery
            //String getDeviceJson(HADevice& dev); // Return the Device json
            /*connect to mqtt server using current values, 
            
            return true if connected, false if failed
            */
            bool mqttConnect();
            void mqttDisconnect();

            void publishAllSensor();
            bool mqttPublish(String topic, String paylod);
    
    };
#endif