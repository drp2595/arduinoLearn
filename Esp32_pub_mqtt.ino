//MAY BE YOU NEED TO ADD FILES/FOLDERS TO RUN THIS CODE PROPERLY.THIS CODE IS FOR REFERENCES ONLY.
//I USED WROOM32- ESP32 DEV KIT.
#include "SPIFFS.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>


// Enter your WiFi ssid and password
const char* ssid = "abcd"; //Provide your SSID
const char* password = "Ttpl@y2x!CVJS!@#Workstation3()"; // Provide Password
const char* mqtt_server = "ACCOUNTid.iot.REGION.amazonaws.com"; // Relace with your MQTT END point
const int   mqtt_port = 8883;

String Read_rootca;
String Read_cert;
String Read_privatekey;
#define BUFFER_LEN 256
long lastMsg = 0;
char msg[BUFFER_LEN];
int Value = 0;
byte mac[6];
char mac_Id[18];
int count = 1;

WiFiClientSecure espClient;
PubSubClient client(espClient);



void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("ei_out", "hello world");
      // ... and resubscribe
      client.subscribe("ei_in");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  // dht.begin();
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(2, OUTPUT);
  setup_wifi();
  delay(1000);
  //=============================================================
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  //=======================================
  //Root CA File Reading.
  File file2 = SPIFFS.open("/FILE.pem", "r"); //Put your pem file
  if (!file2) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("Root CA File Content:");
  while (file2.available()) {
    Read_rootca = file2.readString();
    Serial.println(Read_rootca);
  }
  //=============================================
  // Cert file reading
  File file4 = SPIFFS.open("/FILE-certificate.pem.crt", "r"); //Put your cert file
  if (!file4) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("Cert File Content:");
  while (file4.available()) {
    Read_cert = file4.readString();
    Serial.println(Read_cert);
  }
  //=================================================
  //Privatekey file reading
  File file6 = SPIFFS.open("/FILE-private.pem.key", "r"); //Put your key file
  if (!file6) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("privateKey File Content:");
  while (file6.available()) {
    Read_privatekey = file6.readString();
    Serial.println(Read_privatekey);
  }
  //=====================================================

  char* pRead_rootca;
  pRead_rootca = (char *)malloc(sizeof(char) * (Read_rootca.length() + 1));
  strcpy(pRead_rootca, Read_rootca.c_str());

  char* pRead_cert;
  pRead_cert = (char *)malloc(sizeof(char) * (Read_cert.length() + 1));
  strcpy(pRead_cert, Read_cert.c_str());

  char* pRead_privatekey;
  pRead_privatekey = (char *)malloc(sizeof(char) * (Read_privatekey.length() + 1));
  strcpy(pRead_privatekey, Read_privatekey.c_str());

  Serial.println("================================================================================================");
  Serial.println("Certificates that passing to espClient Method");
  Serial.println();
  Serial.println("Root CA:");
  Serial.write(pRead_rootca);
  Serial.println("================================================================================================");
  Serial.println();
  Serial.println("Cert:");
  Serial.write(pRead_cert);
  Serial.println("================================================================================================");
  Serial.println();
  Serial.println("privateKey:");
  Serial.write(pRead_privatekey);
  Serial.println("================================================================================================");

  espClient.setCACert(pRead_rootca);
  espClient.setCertificate(pRead_cert);
  espClient.setPrivateKey(pRead_privatekey);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //====================================================================================================================
  WiFi.macAddress(mac);
  snprintf(mac_Id, sizeof(mac_Id), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print(mac_Id);
  //=====================================================================================================================
  delay(2000);
}


void loop() { //below values are hardcoded for now, cause at this time I don`t have sensor.
  float h = 28;   // Reading Temperature form DHT sensor
  float t = 79;      // Reading Humidity form DHT sensor
  float tF = (t * 1.8) + 32;
  String Temprature = String(t);
  String Humidity = String(h);
}
