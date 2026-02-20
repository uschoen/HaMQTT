#ifndef HAKEYVALUES_H
    #define HAKEYVALUES_H

    
// Home Assistant MQTT name
    #define HA_DISCOVERY_PREFIX        "homeassistant"
    
// default expire time for sensors
    #define DEFAULT_EXPIRE_AFTER        600   // 10 Minuten
    
// reconnect interval, if mqtt connect faild
    #ifndef MQTT_ReCONNECT_INTERVAL
        #define MQTT_ReCONNECT_INTERVAL 10000 // 10 Sekunden
    #endif

/*  Home Assistant MQTT Discovery Componenten
    Use: "homeassistant/<HA_COMPONENT_TYPE>/<node_id>/<object_id>/config"
    node_id:            most the mac of the device or other unique id
    object_id:          a unique id from the sensor
    HA_COMPONENT_TYPE:  typ of the sensor (se blow)
*/
    #define HA_COMPONENT_ALARM_CONTROL_PANEL "alarm_control_panel"
    #define HA_COMPONENT_BINARY_SENSOR       "binary_sensor"
    #define HA_COMPONENT_BUTTON              "button"
    #define HA_COMPONENT_CAMERA              "camera"
    #define HA_COMPONENT_CLIMATE             "climate"
    #define HA_COMPONENT_COVER               "cover"
    #define HA_COMPONENT_DEVICE_TRACKER      "device_tracker"
    #define HA_COMPONENT_DEVICE_TRIGGER      "device_trigger"
    #define HA_COMPONENT_FAN                 "fan"
    #define HA_COMPONENT_HUMIDIFIER          "humidifier"
    #define HA_COMPONENT_LIGHT               "light"
    #define HA_COMPONENT_LOCK                "lock"
    #define HA_COMPONENT_NUMBER              "number"
    #define HA_COMPONENT_SCENE               "scene"
    #define HA_COMPONENT_SELECT              "select"
    #define HA_COMPONENT_SENSOR              "sensor"
    #define HA_COMPONENT_SIREN               "siren"
    #define HA_COMPONENT_SWITCH              "switch"
    #define HA_COMPONENT_TAG_SCANNER         "tag_scanner"
    #define HA_COMPONENT_TEXT                "text"
    #define HA_COMPONENT_UPDATE              "update"
    #define HA_COMPONENT_VACUUM              "vacuum"
    #define HA_COMPONENT_WATER_HEATER        "water_heater"

    
// Home Assistant Sensor CLASSES 
    #define HA_DEVICE_CLASS_NONE  (char*)NULL // Nutzt den HA Standard
    #define HA_SENS_APPARENT_POWER   "apparent_power"  // VA
    #define HA_SENS_AQI              "aqi"             // Index
    #define HA_SENS_ATMOSPHERIC_PRES "atmospheric_pressure" // hPa, mbar
    #define HA_SENS_BATTERY          "battery"         // %
    #define HA_SENS_CO               "carbon_monoxide" // ppm
    #define HA_SENS_CO2              "carbon_dioxide"  // ppm
    #define HA_SENS_CURRENT          "current"         // A
    #define HA_SENS_DATA_RATE        "data_rate"       // Mbit/s, KiB/s
    #define HA_SENS_DATA_SIZE        "data_size"       // MiB, GB
    #define HA_SENS_DISTANCE         "distance"        // m, cm, km
    #define HA_SENS_DURATION         "duration"        // s, min, h, d
    #define HA_SENS_ENERGY           "energy"          // Wh, kWh, MWh
    #define HA_SENS_ENUM             "enum"            // Status-Liste
    #define HA_SENS_FREQUENCY        "frequency"       // Hz, kHz, MHz
    #define HA_SENS_GAS              "gas"             // m³, ft³
    #define HA_SENS_HUMIDITY         "humidity"        // %
    #define HA_SENS_ILLUMINANCE      "illuminance"     // lx
    #define HA_SENS_IRRADIANCE       "irradiance"      // W/m²
    #define HA_SENS_MOISTURE         "moisture"        // %
    #define HA_SENS_MONETARY         "monetary"        // EUR, USD
    #define HA_SENS_NITROGEN_DIOXIDE "nitrogen_dioxide" // µg/m³
    #define HA_SENS_NITROGEN_MONOXIDE "nitrogen_monoxide" // µg/m³
    #define HA_SENS_NITROUS_OXIDE    "nitrous_oxide"   // µg/m³
    #define HA_SENS_OZONE            "ozone"           // µg/m³
    #define HA_SENS_PH               "ph"              // pH
    #define HA_SENS_PM1              "pm1"             // µg/m³
    #define HA_SENS_PM10             "pm10"            // µg/m³
    #define HA_SENS_PM25             "pm25"            // µg/m³
    #define HA_SENS_POWER_FACTOR     "power_factor"    // %
    #define HA_SENS_POWER            "power"           // W, kW
    #define HA_SENS_PRECIPITATION    "precipitation"   // mm, in
    #define HA_SENS_PRECIPITATION_INT "precipitation_intensity" // mm/h
    #define HA_SENS_PRESSURE         "pressure"        // Pa, hPa, bar
    #define HA_SENS_REACTIVE_POWER   "reactive_power"  // var
    #define HA_SENS_SIGNAL_STRENGTH  "signal_strength" // dB, dBm
    #define HA_SENS_SPEED            "speed"           // m/s, km/h
    #define HA_SENS_SULPHUR_DIOXIDE  "sulphur_dioxide" // µg/m³
    #define HA_SENS_TEMPERATURE      "temperature"     // °C, °F
    #define HA_SENS_TIMESTAMP        "timestamp"       // ISO 8601
    #define HA_SENS_VOLATILE_ORG_COM "volatile_organic_compounds" // µg/m³
    #define HA_SENS_VOLTAGE          "voltage"         // V
    #define HA_SENS_VOLUME           "volume"          // L, m³
    #define HA_SENS_WATER            "water"           // L, m³
    #define HA_SENS_WEIGHT           "weight"          // kg, g, lb
    #define HA_SENS_WIND_SPEED       "wind_speed"      // km/h, m/s

// --- BINARY SENSOR DEVICE CLASSES ---
    #define HA_BIN_BATTERY           "battery"         // Low/Normal
    #define HA_BIN_BATTERY_CHARGING  "battery_charging" // Charging/Not
    #define HA_BIN_CARBON_MONOXIDE   "carbon_monoxide" // Danger/Safe
    #define HA_BIN_COLD              "cold"            // Cold/Normal
    #define HA_BIN_CONNECTIVITY      "connectivity"    // Connected/Disconnected
    #define HA_BIN_DOOR              "door"            // Open/Closed
    #define HA_BIN_GARAGE_DOOR       "garage_door"     // Open/Closed
    #define HA_BIN_GAS               "gas"             // Detected/Clear
    #define HA_BIN_HEAT              "heat"            // Hot/Normal
    #define HA_BIN_LIGHT             "light"           // Detected/No Light
    #define HA_BIN_LOCK              "lock"            // Unlocked/Locked
    #define HA_BIN_MOISTURE          "moisture"        // Wet/Dry
    #define HA_BIN_MOTION            "motion"          // Detected/Clear
    #define HA_BIN_MOVING            "moving"          // Moving/Stopped
    #define HA_BIN_OCCUPANCY         "occupancy"       // Detected/Clear
    #define HA_BIN_OPENING           "opening"         // Open/Closed
    #define HA_BIN_PLUG              "plug"            // Plugged/Unplugged
    #define HA_BIN_POWER             "power"           // Powered/No Power
    #define HA_BIN_PRESENCE          "presence"        // Home/Away
    #define HA_BIN_PROBLEM           "problem"         // Problem/OK
    #define HA_BIN_RUNNING           "running"         // Running/Not Running
    #define HA_BIN_SAFETY            "safety"          // Unsafe/Safe
    #define HA_BIN_SMOKE             "smoke"           // Detected/Clear
    #define HA_BIN_SOUND             "sound"           // Detected/Clear
    #define HA_BIN_TAMPER            "tamper"          // Detected/Clear
    #define HA_BIN_UPDATE            "update"          // Update Available/No
    #define HA_BIN_VIBRATION         "vibration"       // Detected/Clear
    #define HA_BIN_WINDOW            "window"          // Open/Closed

// --- SWITCH, COVER & OTHER CLASSES ---
    #define HA_SW_OUTLET             "outlet"
    #define HA_SW_SWITCH             "switch"
    #define HA_COV_AWNING            "awning"
    #define HA_COV_BLIND             "blind"
    #define HA_COV_CURTAIN           "curtain"
    #define HA_COV_DAMPER            "damper"
    #define HA_COV_DOOR              "door"
    #define HA_COV_GARAGE            "garage"
    #define HA_COV_GATE              "gate"
    #define HA_COV_SHADE             "shade"
    #define HA_COV_SHUTTER           "shutter"
    #define HA_COV_WINDOW            "window"
    
// Connection Pair contains the mac or ip for the HA Device
    struct ConnectionPair {
        String type;  // z.B. "mac", "ip", "zigbee"
        String value; // ip address or mac 
    };
// HA Device struct
    struct HADevice {
        // Identifikation (mandatory)
        String ids;
        String name;
        // Metadaten (Optional, leave blank if not required)
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
// HA Sensor    
    struct HASensor {
        // Identifikation (mandatory)
       const char* unique_id;                           // unique ID (z.B. "sensor_temp_01","mac")
       const char* name;                                // name in HA
       const char* components=HA_COMPONENT_SENSOR;      // paylod Components

        // Sensor Details (Optional, leave blank if not required)
        const char* dev_cla=nullptr;                    // device_class: z.B. "temperature", "humidity", "battery", "energy"
        const char* unit_of_meas=nullptr;               // Maßeinheit: z.B. "°C", "%", "kWh"
        const char* ic=nullptr;                         // icon: z.B. "mdi:thermometer"
        const char* stat_cla=nullptr;                   // state_class: z.B. "measurement", "total_increasing"
        
        // Payload configuration
        const char* val_tpl=nullptr;                    // value_template: z.B. "{{ value_json.temperature }}"
        int exp_aft=600;                                // expire_after: Zeit in Sek. bis Sensor "unavailable" wird
        const char* availability_mode="all";            // default auf all, es müssen alle status felder unter availability online sein
                
        // numeric settings
        int sugg_disp_prec = -1;                        // suggested_display_precision: Nachkommastellen (-1 = ignorieren)
    };
    
    

#endif