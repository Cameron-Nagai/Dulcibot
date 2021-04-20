#include "Adafruit_TLC5947.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP_WiFiManager.h>  
#include "AppleMidi.h"
// How many boards do you have chained?
#define NUM_TLC5974 1
#define usingWifi 1
#define data   2
#define clock   22
#define latch   21
#define oe  -1  // set to -1 to not use the enable pin (its optional)\
set
const int resolution = 8;   // 8 bit pwm resolution (0-255)
const int freq = 5000;      // PWM Frequency
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int pwmChannel3 = 2;
const int pwmChannel4 = 3;
const int pwmChannel5 = 4;
const int pwmChannel6 = 5;
const int pwmChannel7 = 6;




int delayAction = 14;
char ssid[] = "Sue Associates"; //  your network SSID (name)
char pass[] = "Do you know the way to mayan warrior?";    // your network password (use for WPA, or use as key for WEP)

bool wifiSaved;
String AP_SSID;
String AP_PASS;
String Router_SSID;
String Router_Pass;


unsigned long t0 = millis();
bool isConnected = false;

APPLEMIDI_CREATE_INSTANCE(WiFiUDP, AppleMIDI); // see definition in AppleMidi_Defs.h
// Forward declaration
void OnAppleMidiConnected(uint32_t ssrc, char* name);
void OnAppleMidiDisconnected(uint32_t ssrc);
void OnAppleMidiNoteOn(byte channel, byte note, byte velocity);
void OnAppleMidiNoteOff(byte channel, byte note, byte velocity);

bool solenoidsOn[] = {false, false, false, false, false, false, false, false};



Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

void setup() {
  Serial.begin(115200);

  ledcSetup(0, freq, resolution);   //Configures PWM Channels for LEDs (1 different pwm channel per Sol)
  ledcSetup(1, freq, resolution);  
  ledcSetup(2, freq, resolution);  
  ledcSetup(3, freq, resolution); 
  ledcSetup(4, freq, resolution);  
  ledcSetup(5, freq, resolution);  
  ledcSetup(6, freq, resolution); 

  ledcAttachPin(32, 0);         //Assigns pins to pwm channels
  ledcAttachPin(33, 1);
  ledcAttachPin(17, 2);
  ledcAttachPin(18, 3);
  ledcAttachPin(4, 4);         
  ledcAttachPin(26, 5);
  ledcAttachPin(23, 6);
  
  tlc.begin();
 
  
  if (oe >= 0) {
    pinMode(oe, OUTPUT);
    digitalWrite(oe, LOW);
  }
if (usingWifi) {
  wifiSetup();
}
  Serial.print(F("Getting IP address..."));


//  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(""));
  Serial.println(F("WiFi connected"));


  Serial.println();
  Serial.print(F("IP address is "));
  Serial.println(WiFi.localIP());

  Serial.println(F("OK, now make sure you an rtpMIDI session that is Enabled"));
  Serial.print(F("Add device named Arduino with Host/Port "));
  Serial.print(WiFi.localIP());
  Serial.println(F(":5004"));
  Serial.println(F("Then press the Connect button"));
  Serial.println(F("Then open a MIDI listener (eg MIDI-OX) and monitor incoming notes"));

  // Create a session and wait for a remote host to connect to us
  AppleMIDI.begin("test");

  AppleMIDI.OnConnected(OnAppleMidiConnected);
  AppleMIDI.OnDisconnected(OnAppleMidiDisconnected);

  AppleMIDI.OnReceiveNoteOn(OnAppleMidiNoteOn);
  AppleMIDI.OnReceiveNoteOff(OnAppleMidiNoteOff);

  
}

void loop() {
  AppleMIDI.read();


//  for (int i=0; i<8; i++) {
//    if (solenoidsOn[i] == true) {
//      tlc.setPWM(i, 4095);
//      tlc.write();
//      Serial.println(i);
//      Serial.println("HIGH");
//      delay(delayAction);
//      tlc.setPWM(i, 0);
//      tlc.write();
//      Serial.println("LOW");
//      solenoidsOn[i] = false;
//  }  
//}

    
  for (int i=0; i<8; i++) {
    if (solenoidsOn[i] == true) {
      ledcWrite (i, 255);
      Serial.println("HIGH");
      delay(delayAction);
      ledcWrite(i, 0);
      Serial.println("LOW");
      solenoidsOn[i] = false;
  }  
}

  
//  if (solenoidsOn[0] == true) {
//      Serial.println("0 is on");
//      tlc.setPWM(0, 4095);
//      tlc.write();
//      Serial.println("HIGH");
//      delay(delayAction);
//      tlc.setPWM(0, 0);
//      tlc.write();
//      Serial.println("LOW");
//      solenoidsOn[0] = false;
//}
//  else if (solenoidsOn[1] == true) {
//      Serial.println("1 is on");
//      tlc.setPWM(1, 4095);
//      tlc.write();
//      Serial.println("HIGH");
//      delay(delayAction);
//      tlc.setPWM(1, 0);
//      tlc.write();
//      Serial.println("LOW");
//      solenoidsOn[1] = false;
//  }
}
void OnAppleMidiConnected(uint32_t ssrc, char* name) {
  isConnected  = true;
  Serial.print(F("Connected to session "));
  Serial.println(name);
}

void OnAppleMidiDisconnected(uint32_t ssrc) {
  isConnected  = false;
  Serial.println(F("Disconnected"));
}

void OnAppleMidiNoteOn(byte channel, byte note, byte velocity) {
  Serial.print(F("Incoming NoteOn from channel:"));
  Serial.print(channel);
  Serial.print(F(" note:"));
  Serial.print(note);
  Serial.print(F(" velocity:"));
  Serial.print(velocity);
  Serial.println();
  //Not sure why this doesn't work, need to ask HIKE
//  for (byte i=0; i<8; i++) {
//    byte noteCheck = 60+i;
//    Serial.println("notecheck is " + noteCheck);
//    if(noteCheck == note) 
//      solenoidsOn[i] = true;
//  }
switch (note) {
  case 60: solenoidsOn[0] = true;
  break;
  case 61: solenoidsOn[1] = true;
  break;
  case 62: solenoidsOn[2] = true;
  break;
  case 63: solenoidsOn[3] = true;
  break;
  case 64: solenoidsOn[4] = true;
  break;
  case 65: solenoidsOn[5] = true;
  break; 
  case 66: solenoidsOn[6] = true;
  break; 
  case 67: solenoidsOn[7] = true;
  break; 
  }
}

void OnAppleMidiNoteOff(byte channel, byte note, byte velocity) {
  Serial.print(F("Incoming NoteOff from channel:"));
  Serial.print(channel);
  Serial.print(F(" note:"));
  Serial.print(note);
  Serial.print(F(" velocity:"));
  Serial.print(velocity);
  Serial.println();
}

void configModeCallback (ESP_WiFiManager *myESP_WiFiManager) {
 
  Serial.print("Entered config mode with ");
  Serial.println("AP_SSID : " + myESP_WiFiManager->getConfigPortalSSID() + " and AP_PASS = " + myESP_WiFiManager->getConfigPortalPW());
 
  Serial.println(WiFi.softAPIP());
}
 
void saveConfigCallback (void) {
//  Serial.println("Should save config");
  wifiSaved = true;
  Serial.println ("Configuration saved");
  Serial.println ("local ip" + WiFi.localIP());
  Serial.println ("ap ip " + WiFi.softAPIP ()); // print the IP of the AP
 
}

void wifiSetup() {
    Serial.println("\nStarting AutoConnectWithFeedBack");
 
  // Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
  //ESP_WiFiManager ESP_wifiManager;
  // Use this to personalize DHCP hostname (RFC952 conformed)
  ESP_WiFiManager ESP_wifiManager("Dulcimer");
 
  //reset settings - for testing
//  ESP_wifiManager.resetSettings();
 
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  ESP_wifiManager.setAPCallback(configModeCallback);
  ESP_wifiManager.setSaveConfigCallback(saveConfigCallback);
 
  ESP_wifiManager.setDebugOutput(true);
 
  //set custom ip for portal
  ESP_wifiManager.setAPStaticIPConfig(IPAddress(42, 42, 42, 42), IPAddress(42, 42, 42, 42), IPAddress(255, 255, 255, 0));
 
  ESP_wifiManager.setMinimumSignalQuality(-1);
  // Set static IP, Gateway, Subnetmask, DNS1 and DNS2. New in v1.0.5+
//  ESP_wifiManager.setSTAStaticIPConfig(IPAddress(192, 168, 86, 114), IPAddress(192, 168, 86, 1), IPAddress(255, 255, 255, 0));
 
  // We can't use WiFi.SSID() in ESP32 as it's only valid after connected.
  // SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
  // Have to create a new function to store in EEPROM/SPIFFS for this purpose
  Router_SSID = ESP_wifiManager.WiFi_SSID();
  Router_Pass = ESP_wifiManager.WiFi_Pass();
 
  //Remove this line if you do not want to see WiFi password printed
  Serial.println("Stored: SSID = " + Router_SSID + ", Pass = " + Router_Pass);
 
  if (Router_SSID != "")
  {
    ESP_wifiManager.setConfigPortalTimeout(1); //If no access point name has been previously entered disable timeout.
    Serial.println("Got stored Credentials. Timeout 1s");
  }
  else
  {
    Serial.println("No stored Credentials. No timeout");
  }
 
  String chipID = String(ESP_getChipId(), HEX);
  chipID.toUpperCase();
 
  // SSID and PW for Config Portal
  AP_SSID = "PWMDriver" + chipID;
  AP_PASS = "";
 
  // Get Router SSID and PASS from EEPROM, then open Config portal AP named "ESP_XXXXXX_AutoConnectAP" and PW "MyESP_XXXXXX"
  // 1) If got stored Credentials, Config portal timeout is 60s
  // 2) If no stored Credentials, stay in Config portal until get WiFi Credentials
  ESP_wifiManager.autoConnect(AP_SSID.c_str(), AP_PASS.c_str());
  //or use this for Config portal AP named "ESP_XXXXXX" and NULL password
  //ESP_wifiManager.autoConnect();
 
  //if you get here you have connected to the WiFi
  Serial.println("WiFi connected");
 
}
