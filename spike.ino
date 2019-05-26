#if defined(ESP8266)
#include <FS.h> // must be first
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#else // ESP32
#include <SPIFFS.h>
#include <WiFi.h>          //https://github.com/esp8266/Arduino
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#endif

#include <Arduino.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <time.h>

#define TEMP_SENSOR_DHT11
#undef  TEMP_SENSOR_DHT12

#ifdef TEMP_SENSOR_DHT11
#include <DHTesp.h>
#elif defined(TEMP_SENSOR_DHT12)
#include <DHT12.h>
#endif

#define SYSLOG_flag wifiConnected
#define SYSLOG_host syslog_server

#include "accelerando_trace.h"

//@******************************* constants *********************************

// change the name below to something unique (eg your name)
#define HOTSPOT_NAME "myspike"

#define SETTINGS_RESET false
#define WIFI_DEBUG false


#define PIN_HELLO 22
/*
#define PIN_BATTERY 2
#define PIN_SOIL 36
#define PIN_RAIN 14
*/
#define PIN_BATTERY 34
#define PIN_SOIL 33
#define PIN_RAIN 35
#define PIN_SOLR 36

#define PIN_TEMP 16
#define PIN_CLEAR 5

#define BLYNK_BATTERY 0
#define BLYNK_SOIL 1
#define BLYNK_RAIN 2
#define BLYNK_TEMP 3
#define BLYNK_HUM  4
#define BLYNK_SOLR  5
#define BLYNK_SLEEP 6

#ifdef ESP8266
#define CONFIG_FILE "config.json"
#else
#define CONFIG_FILE "/config.json"
#endif

#ifndef NETWORK_RECONNECT_SECONDS
#define NETWORK_RECONNECT_SECONDS 5
#endif

const char *wake_reason = NULL; // will be filled in during startup

//@******************************* variables *********************************

//
// Configuration values
//
char blynk_auth[80] = "";
char device_id[20] = "";
char sample_interval[10] = "60";
char syslog_server[40] = "";
char ota_password[20] = "changeme";

//
// Network resources
//
bool wifiConnected = false;
bool blynkConnected = false;

WiFiClient espClient;

bool _shouldSaveConfig = false;
#ifdef ESP8266
WiFiEventHandler _gotIpEventHandler, _disconnectedEventHandler;
#endif
Ticker wifiReconnectTimer;


int blink_rate = 100;
int blink_duty = 50;
char ip_addr_str[20] = "unset";
char reformat[8] = "no";
bool doSleep = true;

#ifdef TEMP_SENSOR_DHT11
DHTesp dht;
#elif defined(TEMP_SENSOR_DHT12)
DHT12 dht;
#endif

int battRaw;
int solrRaw;
int rainRaw;
int soilRaw;


//
//@********************************** loop ***********************************

void loop(void)
{
  unsigned long now = millis();

#ifdef PIN_HELLO
  static int hello = 0;

  int pos = now % blink_rate;
  int flip = blink_rate * blink_duty / 100;
  int led = (pos > flip);
  if (led != hello) {
    hello = led;
    digitalWrite(PIN_HELLO, hello);
  }
#endif

  //
  // Handle network Events
  //
  ArduinoOTA.handle();
  if (blynkConnected) {
    Blynk.run();
  } else if (strlen(blynk_auth)>0) {
    //
    // Connection failed previously, retry one last time
    //
    if (Blynk.connect()) {
      NOTICE("Blynk connected");
      blynkConnected=true;
      Blynk.run();
    }
  }



  //
  // Poll input devices
  //
  //
  // NOTE: ADC2 cannot be used when wifi is in use, so values are read from
  // setup() prior to wifi turn on.   Pins 32 and up  use ADC1 so can be
  // read here.
  //
  if (PIN_BATTERY>=32) {
    battRaw = analogRead(PIN_BATTERY);
  }
  float voltage = (battRaw/4095.0) * 3.3f * 2.0f;

  if (PIN_SOLR>=32) {
    solrRaw = analogRead(PIN_SOLR);
  }
  float solr = (solrRaw/4095.0) * 3.3f * 2.0f;

  if (PIN_SOIL >= 32) {
    soilRaw = analogRead(PIN_SOIL);
  }
  float soil = ((4095-soilRaw)/4095.0)*100.0f;

  if (PIN_RAIN >= 32) {
    rainRaw = analogRead(PIN_RAIN);
  }
  float rain = (rainRaw/4095.0) * 100.0f;

#ifdef TEMP_SENSOR_DHT11
  float temp = dht.getTemperature();
  float hum = dht.getHumidity();
#elif defined(TEMP_SENSOR_DHT12)
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
#endif
  NOTICE("battRaw=%d soilRaw=%d rainRaw=%d solrRaw=%d", battRaw, soilRaw, rainRaw, solrRaw);

  if (blynkConnected) {
    NOTICE("Sending to blynk");
    Blynk.virtualWrite(BLYNK_BATTERY, voltage);
    Blynk.virtualWrite(BLYNK_SOLR, solr);
    Blynk.virtualWrite(BLYNK_SOIL, soil);
    Blynk.virtualWrite(BLYNK_RAIN, rain);
    Blynk.virtualWrite(BLYNK_TEMP, temp);
    Blynk.virtualWrite(BLYNK_HUM, hum);
  }
  else {
    NOTICE("Unable to send to blynk");
  }

  if (blynkConnected) {
    Blynk.run();
  }
  NOTICE("SENSORS: batt=%.2fv soil=%.2f%% rain=%.2f%% temp=%.1fC hum=%.1f%% solr=%.2fv",
	 voltage, soil, rain, temp, hum, solr);

  if (doSleep) {
    int sleep_secs = atoi(sample_interval);
    if (sleep_secs < 1) sleep_secs = 60;
    NOTICE("Going to sleep for %d sec", sleep_secs);
    esp_sleep_enable_timer_wakeup(sleep_secs * 1000 * 1000);
    Serial.flush();
    delay(1000); // let network packets drain
    esp_deep_sleep_start();
  }
  else {
    // when the device is put into no sleep mode, just keep swimming
    delay(5000);
  }

}

//
//@********************************* setup ***********************************

void setup(void)
{
  //
  // Set up the serial port for diagnostic trace
  //
  Serial.begin(115200);
  Serial.println("\n\nIoTBNE Garden Spike, v1 May 2019");
  _printWakeReason();

  //
  // Set up the IO devices
  //
  NOTICE("IO Setup");
  pinMode(PIN_HELLO, OUTPUT);
  pinMode(PIN_BATTERY, INPUT);
  pinMode(PIN_SOIL, INPUT);
  pinMode(PIN_RAIN, INPUT);

#ifdef TEMP_SENSOR_DHT11
  dht.setup(PIN_TEMP, DHTesp::DHT11);
#elif defined(TEMP_SENSOR_DHT12)
  dht.begin();
#endif

  // We have to read the sensors BEFORE enabling wifi because wifi disables ADC2
  battRaw = analogRead(PIN_BATTERY);
  soilRaw = analogRead(PIN_SOIL);
  rainRaw = analogRead(PIN_RAIN);
  solrRaw = analogRead(PIN_SOLR);

  //
  // Set up the WiFi connection and Blynk client
  //
  wifi_setup();

  DEBUG("Setup complete");
}

//
//
//@****************************** Blynk events *******************************

BLYNK_CONNECTED() {
  ENTER(L_NOTICE);
  Blynk.syncVirtual(V6);
}

BLYNK_WRITE(V6)
{
  ENTER(L_NOTICE);
  doSleep = param.asInt();
  NOTICE("doSleep <= %s", TRUTH(doSleep));
}


//
//@******************************* functions *********************************

void _printWakeReason(void)
{
#ifdef ESP8266
  wake_reason = ESP.getResetReason().c_str();
#else
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0 : wake_reason="RTC IO"; break;
  case ESP_SLEEP_WAKEUP_EXT1 : wake_reason ="RTC CNTL"; break;
  case ESP_SLEEP_WAKEUP_TIMER : wake_reason = "timer"; break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD : wake_reason = "touchpad"; break;
  case ESP_SLEEP_WAKEUP_ULP : wake_reason="ULP"; break;
  case ESP_SLEEP_WAKEUP_GPIO: wake_reason="GPIO"; break;
  case ESP_SLEEP_WAKEUP_UART: wake_reason="UART"; break;
  default: wake_reason="other"; break;
  }
#endif
  NOTICE("ESP Wakeup reason: %s", wake_reason);
}

void _saveConfigCallback()
{
  ALERT("Will save config");
  _shouldSaveConfig = true;
}

void _readConfig()
{
  if (!SPIFFS.begin()) {
    ALERT("NO SPIFFS.  Formatting");
    _writeConfig(true);
    ALERT("Rebooting after format");
    delay(3000);
#ifdef ESP8266
    ESP.reset();
#else
    ESP.restart();
#endif
  }

  NOTICE("mounted file system");
  if (!SPIFFS.exists(CONFIG_FILE)) {
    ALERT("No configuration file found");
    //_writeConfig(false);
  }

  //file exists, reading and loading
  NOTICE("reading config file");
  File configFile = SPIFFS.open(CONFIG_FILE, "r");
  if (!configFile) {
    ALERT("Cannot read config file");
    return;
  }

  size_t size = configFile.size();
  NOTICE("Parsing config file, size=%d", size);

  DynamicJsonDocument doc;
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    ALERT("Failed to parse config file: %s", error.c_str());
    configFile.close();
    return;
  }

  JsonObject root = doc.as<JsonObject>();
  {
    char buf[256];
    serializeJson(root, buf, sizeof(buf));
    NOTICE("Read config: %s", buf);
  }

  strlcpy(blynk_auth, root["blynk_auth"]|"", sizeof(blynk_auth));
  strlcpy(syslog_server, root["syslog_server"]|"", sizeof(syslog_server));
  strlcpy(sample_interval, root["sample_interval"]|"", sizeof(sample_interval));
  strlcpy(device_id, root["device_id"]|"", sizeof(device_id));
  strlcpy(ota_password, root["ota_password"]|"changeme", sizeof(ota_password));

  configFile.close();

}

void _writeConfig(bool force_format)
{
  ALERT("saving config to flash");

  if (force_format) {
    ALERT("Reformatting SPIFFS");
    if (SPIFFS.format()) {
      NOTICE("Reformatted OK");
    }
    else {
      ALERT("SPIFFS Format failed");
      return;
    }
  }

  if (!SPIFFS.begin()) {
    ALERT("failed to mount FS");
    return;
  }

  File configFile = SPIFFS.open(CONFIG_FILE, "w");
  if (!configFile) {
    ALERT("Unable to create new config file");
    return;
  }

  DynamicJsonDocument doc;
  JsonObject root = doc.to<JsonObject>();

  root["blynk_auth"] = blynk_auth;
  root["syslog_server"] = syslog_server;
  root["sample_interval"] = sample_interval;
  root["device_id"] = device_id;
  root["ota_password"] = ota_password;

  if (serializeJson(doc, configFile) == 0) {
    ALERT("Failed to serialise configuration");
  }
  {
    char buf[256];
    serializeJson(root, buf, sizeof(buf));
    NOTICE("Written config: %s", buf);
  }
  configFile.close();
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    ALERT("Failed to obtain time from OS");
    return;
  }
  char timebuf[40];
  strftime(timebuf, sizeof(timebuf), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  NOTICE("Current time is %s", timebuf);
}

#ifdef ESP8266
void _wifi_connect_callback(const WiFiEventStationModeGotIP& event)
#else
void _wifi_connect_callback(WiFiEvent_t event, WiFiEventInfo_t info)
#endif
{
  ENTER(L_INFO);

#ifdef ESP8266
  strlcpy(ip_addr_str, event.ip.toString().c_str(), sizeof(ip_addr_str));
#else
  strlcpy(ip_addr_str, IPAddress(info.got_ip.ip_info.ip.addr).toString().c_str(), sizeof(ip_addr_str));
#endif
  NOTICE("WiFi connected, IP: %s OTA: %s", ip_addr_str, ota_password);
  wifiConnected = true;
  blink_rate = 500;
  blink_duty = 50;

  // Cancel any future connect attempt, as we are now connected
  wifiReconnectTimer.detach();

  if (wake_reason && (wake_reason == "other")) {
    // At cold boot, get the time from NTP
    NOTICE("Getting time from NTP");
    configTime(0,0,"pool.ntp.org");
  }
  printLocalTime();

  // Start trying to connect to Blynk
  if (strlen(blynk_auth)>0) {
    DEBUG("Connecting to blynk app [%s]", blynk_auth);
    if (Blynk.connect()) {
      NOTICE("Connected to Blynk");
      blynkConnected = true;
    }
    else {
      NOTICE("Blynk connect failed (retry later)");
    }
  }
  LEAVE;
}

#ifdef ESP8266
void _wifi_disconnect_callback(const WiFiEventStationModeDisconnected& event)
#else
void _wifi_disconnect_callback(WiFiEvent_t event, WiFiEventInfo_t info)
#endif
{
#ifdef ESP8266
  ALERT("WiFi disconnected (reason %d)", (int)(event.reason));
#else
  ALERT("WiFi disconnected (reason %d)", (int)(info.disconnected.reason));
#endif
  wifiConnected = false;
  blink_rate = 100;
  blink_duty = 50;

  // Cancel any existing timers, and set a new timer to retry wifi in 2 seconds
#ifdef ESP8266
  if (event.reason == WIFI_DISCONNECT_REASON_AUTH_FAIL) {
    NOTICE("Auth failed, reverting to config portal");
    delay(2000);
    _wifiMgr_setup(true);
    return;
  }
// TODO: handle esp32 case here
#endif

  wifiReconnectTimer.once(NETWORK_RECONNECT_SECONDS, wifi_setup);
}

void _wifiMgr_setup(bool reset)
{
  _readConfig();

  DEBUG("Wifi manager setup");
  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_blynk_auth("blynk_auth", "Blynk Auth Token", blynk_auth, sizeof(blynk_auth));
  WiFiManagerParameter custom_syslog_server("syslog_server", "Log server (blank for none)", syslog_server, sizeof(syslog_server));
  WiFiManagerParameter custom_sample_interval("sample_interval", "Sample interval (seconds)", sample_interval, sizeof(sample_interval));
  WiFiManagerParameter custom_device_id("device_id", "Device ID", device_id, sizeof(device_id));
  WiFiManagerParameter custom_ota_password("ota_password", "Update Password", ota_password, sizeof(ota_password));
  WiFiManagerParameter custom_reformat("reformat", "Force format", reformat, sizeof(reformat));


  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  wifiManager.setDebugOutput(WIFI_DEBUG);

  //set config save notify callback
  wifiManager.setSaveConfigCallback(_saveConfigCallback);

  //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //add all your parameters here
  wifiManager.addParameter(&custom_blynk_auth);
  wifiManager.addParameter(&custom_syslog_server);
  wifiManager.addParameter(&custom_sample_interval);
  wifiManager.addParameter(&custom_device_id);
  wifiManager.addParameter(&custom_ota_password);
  wifiManager.addParameter(&custom_reformat);

  //reset settings - for testing
  if (reset
#ifdef PIN_CLEAR
      || (digitalRead(PIN_CLEAR)==LOW)
#endif
    ) {
    ALERT("Doing factory reset of Wifi settings");
    wifiManager.startConfigPortal(HOTSPOT_NAME);
  }

  //set minimum quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);
  //wifiManager.setDebugOutput(true);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()
    ) {
    ALERT("Failed to connect to WiFi after tiemout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
#ifdef ESP8266
    ESP.reset();
#else
    ESP.restart();
#endif
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  DEBUG("Connected to WiFi");
  wifiConnected = true;

  //read updated parameters
  strlcpy(blynk_auth, custom_blynk_auth.getValue(),sizeof(blynk_auth));
  strlcpy(syslog_server, custom_syslog_server.getValue(),sizeof(syslog_server));
  strlcpy(sample_interval, custom_sample_interval.getValue(),sizeof(sample_interval));
  strlcpy(device_id, custom_device_id.getValue(), sizeof(device_id));
  strlcpy(ota_password, custom_ota_password.getValue(), sizeof(ota_password));
  strlcpy(reformat, custom_reformat.getValue(), sizeof(reformat));

  bool force_format = false;
  if (strcasecmp(reformat, "yes") == 0) force_format = true;
  //save the custom parameters to FS
  if (_shouldSaveConfig) {
    _writeConfig(force_format);
  }
  Blynk.config(blynk_auth, BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT);
  MDNS.begin(device_id);

  NOTICE("ESP32 WiFi ready.  My IP Address is %s", WiFi.localIP().toString().c_str());
}

void _OTAUpdate_setup() {
  ArduinoOTA.setHostname(device_id);
  ArduinoOTA.setPassword(ota_password);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    ALERT("Start OTA update (%s)", type.c_str());
  });
  ArduinoOTA.onEnd([]() {
    ALERT("OTA Complete");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    NOTICE("OTA in progress: %3u%% (%u/%u)", (progress / (total / 100)), progress, total);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    ALERT("OTA Error [%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      ALERT("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      ALERT("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      ALERT("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      ALERT("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      ALERT("End Failed");
    }
  });
  ArduinoOTA.begin();
}

void wifi_setup()
{
#ifdef PIN_CLEAR
  // short PIN_CLEAR to ground at boot to clear settings
  pinMode(PIN_CLEAR, INPUT_PULLUP);
#endif

  ENTER(L_DEBUG);
#ifdef ESP8266
  _gotIpEventHandler = WiFi.onStationModeGotIP(_wifi_connect_callback);
#else // ESP32
  WiFi.onEvent(_wifi_connect_callback, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
#endif
  _wifiMgr_setup(SETTINGS_RESET);
  _OTAUpdate_setup();

#ifdef ESP8266
  _disconnectedEventHandler = WiFi.onStationModeDisconnected(_wifi_disconnect_callback);
#else // ESP32
  WiFi.onEvent(_wifi_disconnect_callback, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
#endif
  LEAVE;
}


// Local Variables:
// mode: C++
// c-basic-offset: 2
// End:
