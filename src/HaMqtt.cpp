#include <HaMqtt.h>
#include <cstring> 
#include <string>


HaMqtt::HaMqtt() {
    // Standardkonstruktor
}
HaMqtt::HaMqtt(Client& networkClient) {
    this->_mqttClient.setClient(networkClient);
}
HaMqtt::HaMqtt(Client& networkClient,String server, uint16_t port) {
    this->_mqttClient.setClient(networkClient);
    this->setMQTTServer(server,port,"","");
}
HaMqtt::HaMqtt(Client& networkClient,String server, uint16_t port,String user, String pass) {
    _mqttClient.setClient(networkClient);
    this->setMQTTServer(server,port,user,pass);
}
void HaMqtt::setNetworkClient(Client& newNetworkClient) {
    this->_mqttClient.setClient(newNetworkClient);
}
void HaMqtt::setMQTTServer(String server, uint16_t port, String user, String pass) {
    this->_mqtt_server = server;
    this->_mqtt_port   = port;
    this->_mqtt_user   = user;
    this->_mqtt_pass   = pass;
}
bool HaMqtt::setDeviceAvailability(bool available) {
    String topic=this->_deviceInfos.ids + "/status";
    const char* status = available ? "online" : "offline";
    #ifdef DEBUG
        Serial.print(F("set device avaibable to: "));
        Serial.println(status);
        Serial.print(F("topic: "));
        Serial.println(topic);
        Serial.print(F("rewquest result:"));
    #endif
    if(this->_mqttClient.publish(topic.c_str(), status, true)){
        #ifdef DEBUG
            Serial.println(F(" OK"));
        #endif
        return true;
    }else{
        #ifdef DEBUG
            Serial.println(F(" faild"));
        #endif
        return false;
    }
}
int HaMqtt::getSensorID(const char* unique_id){
    for (int sensorID=0;sensorID<MAX_SENSORS;sensorID++){
        if (this->_sensors[sensorID].unique_id != nullptr) {
            if (strcmp(this->_sensors[sensorID].unique_id, unique_id) == 0) {return sensorID;}
        }
    }
    #ifdef DEBUG
        Serial.print(F("HA sendor not found:"));
        Serial.println(unique_id);
    #endif
    return -1;
}
bool HaMqtt::setSensorAvailability(int sensorID,bool available){
    if (sensorID < 0 || sensorID > MAX_SENSORS) {return false;}
    String topic=this->_deviceInfos.ids+ "/"+this->_sensors[sensorID].unique_id+"/status";
    const char* status = available ? "online" : "offline";
    #ifdef DEBUG
        Serial.print(F("set sensor "));
        Serial.print(this->_sensors[sensorID].unique_id);
        Serial.print(F(" avaibable to: "));
        Serial.println(status);
        Serial.print(F("topic: "));
        Serial.println(topic);
        Serial.print(F("rewquest result:"));
    #endif
    if(this->_mqttClient.publish(topic.c_str(), status, true)){
        #ifdef DEBUG
            Serial.println(F(" OK"));
        #endif
        return true;
    }else{
        #ifdef DEBUG
            Serial.println(F(" faild"));
        #endif
        return false;
    }
}
bool HaMqtt::setSensorAvailability(const char* unique_id,bool available){
    int sensorID=this->getSensorID(unique_id);
    if (sensorID<0){return false;}
    return this->setSensorAvailability(sensorID,available);
}
bool HaMqtt::mqttConnect() {    
    if (this->_mqtt_server == "" || this->_mqtt_port < 1) return false;
    if (this->_mqttClient.state()==MQTT_CONNECTED) return true;
    // Aktuelle Werte von den Referenz-Adressen holen
    this->_mqttClient.setBufferSize(this->_mqttBuffer);
    this->_mqttClient.setServer(this->_mqtt_server.c_str(), this->_mqtt_port);
    String deviceStatusTopic= this->_deviceInfos.ids + "/status";
    #ifdef DEBUG
        Serial.print(F("try to connect mqtt to: "));
    #endif
    if (this->_mqtt_user != "" && this->_mqtt_pass != "") {
        #ifdef DEBUG
            Serial.print(F("user: "));
            Serial.print(this->_mqtt_user);
            Serial.print(F(" password: "));
            Serial.println(this->_mqtt_pass);
        #endif
        return this->_mqttClient.connect(this->_deviceInfos.ids.c_str(), this->_mqtt_user.c_str(), this->_mqtt_pass.c_str(),deviceStatusTopic.c_str(), 1, true, "offline");
    } else {
        #ifdef DEBUG
            Serial.println();
        #endif
        return this->_mqttClient.connect(this->_deviceInfos.ids.c_str(),deviceStatusTopic.c_str(), 1, true, "offline");
    }
}
void HaMqtt::mqttDisconnect(){
  if (this->_mqttClient.state()!=MQTT_CONNECTED) return;
    #ifdef DEBUG
      Serial.println(F("stop mqtt server"));
    #endif
    this->_mqttClient.disconnect();
};
void HaMqtt::deleteHASensor(int sensor_id){
    if (sensor_id <0 || sensor_id>MAX_SENSORS){return;}
    #ifdef DEBUG
        Serial.print(F("delete HA sensor"));
        Serial.print(this->_sensors[sensor_id].unique_id);
    #endif
    this->_sensors[sensor_id].unique_id = nullptr;
    return;
}
void HaMqtt::deleteHASensor(const char* unique_id){
    int sensor_id=this->getSensorID(unique_id);
    if (sensor_id<0){return;}// not found
    this->deleteHASensor(sensor_id);
}
int HaMqtt::addHASensor(HASensor sensor){
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (this->_sensors[i].unique_id == nullptr || this->_sensors[i].unique_id[0] == '\0'){ // free array found
            if (sensor.components == nullptr) {sensor.components=HA_COMPONENT_SENSOR;}
            this->_sensors[i] = sensor;
            #ifdef DEBUG
                Serial.print(F("add HA sensor: "));
                Serial.print(this->_sensors[i].unique_id);
                Serial.print(F(" name: "));
                Serial.print(this->_sensors[i].name);
                Serial.print(F(" HASensorID: "));
                Serial.println(i);
            #endif
            return i; // succes
        }
    }
    return -1; // Array full
}
void HaMqtt::addHADevice(const HADevice& device){
    if (device.ids.length() > 0)  this->_deviceInfos.ids  = device.ids;
    if (device.name.length() > 0) this->_deviceInfos.name = device.name;
    if (device.mdl.length() > 0)  this->_deviceInfos.mdl  = device.mdl;
    if (device.mf.length() > 0)   this->_deviceInfos.mf   = device.mf;
    if (device.sw.length() > 0)   this->_deviceInfos.sw   = device.sw;
    if (device.hw.length() > 0)   this->_deviceInfos.hw   = device.hw;
    if (device.sa.length() > 0)   this->_deviceInfos.sa   = device.sa;
    if (device.cu.length() > 0)   this->_deviceInfos.cu   = device.cu;
    if (device.via_device.length() > 0) this->_deviceInfos.via_device = device.via_device;

    // Connections kopieren
    this->_deviceInfos.connectionCount = device.connectionCount;
    for (uint8_t i = 0; i < device.connectionCount; i++) {
        // Hier findet eine echte Kopie der Texte statt
        this->_deviceInfos.connections[i].type  = device.connections[i].type;
        this->_deviceInfos.connections[i].value = device.connections[i].value;
    }
    #ifdef DEBUG
        Serial.print(F("add ha device: "));
        Serial.print(this->_deviceInfos.ids);
        Serial.print(F(" : "));
        Serial.println(this->_deviceInfos.name);
    #endif
}
bool HaMqtt::updateSensorValue(const char* unique_id,const char* value){
    int sensorID=this->getSensorID(unique_id);
    if (sensorID<0){return false;}
    return this->updateSensorValue(sensorID,value);
}
bool HaMqtt::updateSensorValue(const char* unique_id,float value){
    int sensorID=this->getSensorID(unique_id);
    if (sensorID<0){return false;}
    return this->updateSensorValue(sensorID,value);
}
bool HaMqtt::updateSensorValue(const char* unique_id,int value){
    int sensorID=this->getSensorID(unique_id);
    if (sensorID<0){return false;}
    return this->updateSensorValue(sensorID,value);
}
bool HaMqtt::updateSensorValue(int sensorID,const char* value){
    if (sensorID < 0 || sensorID > MAX_SENSORS) {return false;}
    String topic=this->_deviceInfos.ids+ "/"+this->_sensors[sensorID].unique_id+"/data";
    #ifdef DEBUG
        Serial.print(F("set value for sensor "));
        Serial.println(this->_sensors[sensorID].unique_id);
        Serial.print(F("Topic: "));
        Serial.println(topic);
        Serial.print(" paylod: ");
        Serial.println(value);
        Serial.print("result: ");
    #endif
    if(this->_mqttClient.publish(topic.c_str(),value, true)){
        #ifdef DEBUG
            Serial.println(F(" OK"));
        #endif
        return true;
    }else{
        #ifdef DEBUG
            Serial.println(F(" faild"));
        #endif
        return false;
    }
}
bool HaMqtt::updateSensorValue(int sensorID,int value){
    std::string svalue = std::to_string(value);
    return this->updateSensorValue(sensorID,svalue.c_str());
}
bool HaMqtt::updateSensorValue(int sensorID,float value){
    std::string svalue = std::to_string(value);
    return this->updateSensorValue(sensorID,svalue.c_str());
}
void HaMqtt::begin(){
    String randomSuffix = String(random(1000000, 99999999));
    String deviceIdentifier;
    String deviceName;
    (this->_deviceInfos.ids==nullptr) ? deviceIdentifier="ESP_"+randomSuffix:deviceIdentifier=this->_deviceInfos.ids;
    (this->_deviceInfos.name==nullptr) ? deviceName="ESP_"+randomSuffix:deviceName=this->_deviceInfos.name;
    this->begin(deviceIdentifier,deviceName);
}
void HaMqtt::begin(String deviceIdentifier,String deviceName){
    this->_deviceInfos.ids=deviceIdentifier;
    this->_deviceInfos.name=deviceName;
    #ifdef DEBUG
        Serial.println(F("HaMQTT begin"));
        Serial.print(F("device ID: "));
        Serial.print(this->_deviceInfos.ids);
        Serial.print(F(" name: "));
        Serial.println(this->_deviceInfos.name);  
    #endif
    this->startup=true;
}
void HaMqtt::loop(){
    #ifdef DEBUG
        if (strcmp(this->_lastMQTTStatus,this->MQTTStateString(this->_mqttClient.state()))){  
            Serial.print(F("mqtt status change from: "));
            Serial.print(this->_lastMQTTStatus);
            Serial.print(F(" to: "));
            this->_lastMQTTStatus=this->MQTTStateString(this->_mqttClient.state());
            Serial.println(this->_lastMQTTStatus);
        }
    #endif
    if (this->startup){
        if (this->_mqttClient.state() != MQTT_CONNECTED) {
            if (millis() - this->_blockMQTTConnectTimer > MQTT_ReCONNECT_INTERVAL) {
                this->_blockMQTTConnectTimer = millis();
                if (this->mqttConnect()) {
                    #ifdef DEBUG
                        Serial.println(F("MQTT connected successfully"));
                    #endif
                    this->setDeviceAvailability(true);
                    for (int sensorID=0;sensorID<MAX_SENSORS;sensorID++){
                        if (this->_sensors[sensorID].unique_id != nullptr) {
                            #ifdef DEBUG
                                Serial.print(F("publish sensor: "));
                                Serial.print(this->_sensors[sensorID].unique_id);
                                Serial.print(F(" : "));
                                Serial.println(this->_sensors[sensorID].name);
                            #endif
                            
                            JsonDocument doc;
                            this->getSensorConfig(doc,sensorID);
                            String topic=String(HA_DISCOVERY_PREFIX)+"/"+String(this->_sensors[sensorID].components)+"/"+this->_deviceInfos.ids+"/"+this->_sensors[sensorID].unique_id+"/config";
                            String paylod;
                            #ifdef DEBUG
                                String mqttString=topic+paylod;
                                
                            #endif
                            serializeJson(doc, paylod);
                            #ifdef DEBUG
                                Serial.print(F("topic:"));
                                Serial.println(topic);
                                Serial.print(F("payload:"));
                                Serial.println(paylod);
                                Serial.print(F("result: "));
                            #endif
                            if (this->_mqttClient.publish(topic.c_str(), paylod.c_str(), true)){
                                #ifdef DEBUG
                                    Serial.println(F("OK"));
                                #endif
                            }else{
                                #ifdef DEBUG
                                    Serial.println(F("FAILD"));
                                #endif
                            }
                        }
                    }
                } else {
                    #ifdef DEBUG
                        Serial.println(F("MQTT connection failed"));
                    #endif
                }
            }
        }
    }   
}
void HaMqtt::getSensorConfig(JsonDocument& doc, int sensorID) {
    // Das Document initialisieren und als Objekt ansprechen
    JsonObject root = doc.to<JsonObject>();

    // 1. Identifikation & Pflichtfelder
    root["name"] = this->_sensors[sensorID].name;
    root["unique_id"] = this->_sensors[sensorID].unique_id;
    root["stat_t"] = String(this->_deviceInfos.ids)+"/"+(this->_sensors[sensorID].unique_id)+"/data";

    // 2. Optionale Sensor Details
    if (this->_sensors[sensorID].dev_cla != nullptr && this->_sensors[sensorID].dev_cla[0] != '\0') 
    root["dev_cla"] = this->_sensors[sensorID].dev_cla;
    if (this->_sensors[sensorID].unit_of_meas != nullptr && this->_sensors[sensorID].unit_of_meas[0] != '\0') 
        root["unit_of_meas"] = this->_sensors[sensorID].unit_of_meas;
    if (this->_sensors[sensorID].ic != nullptr && this->_sensors[sensorID].ic[0] != '\0') 
        root["ic"] = this->_sensors[sensorID].ic;
    if (this->_sensors[sensorID].stat_cla != nullptr && this->_sensors[sensorID].stat_cla[0] != '\0') 
        root["stat_cla"] = this->_sensors[sensorID].stat_cla;
    // 3. Payload & Logik
    if (this->_sensors[sensorID].val_tpl != nullptr && this->_sensors[sensorID].val_tpl[0] != '\0') 
        root["val_tpl"] = this->_sensors[sensorID].val_tpl;
    if (this->_sensors[sensorID].exp_aft != -1) 
        root["exp_aft"] = this->_sensors[sensorID].exp_aft;
    if (this->_sensors[sensorID].sugg_disp_prec != -1) 
        root["sugg_disp_prec"] = this->_sensors[sensorID].sugg_disp_prec;
    // Verfügbarkeit (Ternary Operator korrigiert)
    root["avty_mode"] ="all";
    JsonArray avtyArray = root["avty"].to<JsonArray>();
    JsonObject obj1 = avtyArray.add<JsonObject>();
    obj1["t"]=this->_deviceInfos.ids + "/status";
    // sensor
    JsonObject obj2 = avtyArray.add<JsonObject>();
    obj2["t"]=this->_deviceInfos.ids+ "/"+this->_sensors[sensorID].unique_id+"/status";

    this->appendDeviceToConfig(doc);
}
void HaMqtt::appendDeviceToConfig(JsonDocument& doc) {
   JsonObject deviceObj = doc["dev"].to<JsonObject>();  // "dev" ist die Kurzform für "device"

  // Pflichtfelder & Identifikation
  deviceObj["ids"] = this->_deviceInfos.ids;
  deviceObj["name"] = this->_deviceInfos.name;

  // Optionale Metadaten (nur hinzufügen, wenn nicht leer)
  if (this->_deviceInfos.mdl.length() > 0) deviceObj["mdl"] = this->_deviceInfos.mdl;
  if (this->_deviceInfos.mf.length() > 0)  deviceObj["mf"]  = this->_deviceInfos.mf;
  if (this->_deviceInfos.sw.length() > 0)  deviceObj["sw"]  = this->_deviceInfos.sw;
  if (this->_deviceInfos.hw.length() > 0)  deviceObj["hw"]  = this->_deviceInfos.hw;
  if (this->_deviceInfos.sa.length() > 0)  deviceObj["sa"]  = this->_deviceInfos.sa;
  if (this->_deviceInfos.cu.length() > 0)  deviceObj["cu"]  = this->_deviceInfos.cu;
  if (this->_deviceInfos.via_device.length() > 0) deviceObj["via_device"] = this->_deviceInfos.via_device;

  // Verbindungen (Connections) als Array von Arrays: [["mac", "01:02..."], ["ip", "192..."]]
    if (this->_deviceInfos.connectionCount > 0) {
        // 1. Array in einem Objekt erstellen (v7 Syntax)
        JsonArray connArray = deviceObj["cns"].to<JsonArray>(); 
    
        for (uint8_t i = 0; i < this->_deviceInfos.connectionCount; i++) {
            // 2. Ein verschachteltes Array in ein Array einfügen
            JsonArray pair = connArray.add<JsonArray>(); 
            
            pair.add(this->_deviceInfos.connections[i].type);
            pair.add(this->_deviceInfos.connections[i].value);
        }
    }
}
const char* HaMqtt::MQTTStateString(int state) {
    switch (state) {
        case MQTT_CONNECTION_TIMEOUT: return "MQTT_CONNECTION_TIMEOUT";
        case MQTT_CONNECTION_LOST: return "MQTT_CONNECTION_LOST";
        case MQTT_CONNECT_FAILED: return "MQTT_CONNECT_FAILED";
        case MQTT_DISCONNECTED: return "MQTT_DISCONNECTED";
        case MQTT_CONNECTED: return "MQTT_CONNECTED";
        case MQTT_CONNECT_BAD_PROTOCOL: return "MQTT_CONNECT_BAD_PROTOCOL";
        case MQTT_CONNECT_BAD_CLIENT_ID: return "MQTT_CONNECT_BAD_CLIENT_ID";
        case MQTT_CONNECT_UNAVAILABLE: return "MQTT_CONNECT_UNAVAILABLE";
        case MQTT_CONNECT_BAD_CREDENTIALS: return "MQTT_CONNECT_BAD_CREDENTIALS";
        case MQTT_CONNECT_UNAUTHORIZED: return "MQTT_CONNECT_UNAUTHORIZED";
    }
    return "UNKOWN MQTT StATUS";
}