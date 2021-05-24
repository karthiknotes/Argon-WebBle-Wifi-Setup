/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Projects/Argon-WebBLE-Wifi-Setup/Argon-firmware/Argon-WifiSetup-Firmware/src/Argon-WifiSetup-Firmware.ino"
static void onSetWifiSSID(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
static void onSetWifiPass(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
void configureBLE();
void setup();
void loop();
#line 1 "c:/Projects/Argon-WebBLE-Wifi-Setup/Argon-firmware/Argon-WifiSetup-Firmware/src/Argon-WifiSetup-Firmware.ino"
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

const BleUuid serviceUuid("5c1b9a0d-b5be-4a40-8f7a-66b36d0a5176");
BleCharacteristic connectedCharacteristic("connected", BleCharacteristicProperty::NOTIFY | BleCharacteristicProperty::READ, BleUuid("fdcf0001-3fed-4ed2-84e6-04bbb9ae04d4"), serviceUuid);
BleCharacteristic setWifiSSIDCharacteristic("setWifiSSID", BleCharacteristicProperty::WRITE_WO_RSP, BleUuid("fdcf0002-3fed-4ed2-84e6-04bbb9ae04d4"), serviceUuid, onSetWifiSSID, NULL);
BleCharacteristic setWifiPassCharacteristic("setWifiPass", BleCharacteristicProperty::WRITE_WO_RSP, BleUuid("fdcf0003-3fed-4ed2-84e6-04bbb9ae04d4"), serviceUuid, onSetWifiPass, NULL);
String setSSID;


static void onSetWifiSSID(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context)
{
    char buf[100];
    if (len > sizeof(buf)) {
        Serial.println("SSID is too long");
        return;
    }
    strncpy(buf, (char*)data, len);
    buf[len] = 0;
    Serial.printf("Set wifi SSID: %s\n", buf);
    setSSID = buf;
}

static void onSetWifiPass(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context)
{
#if Wiring_WiFi
    char buf[100];
    if (len > sizeof(buf)) {
        Serial.println("Password is too long");
        return;
    }

    strncpy(buf, (char*)data, len);
    buf[len] = 0;
    Serial.printf("Set wifi pass: %s\n", buf);
    if (PLATFORM_ID == PLATFORM_ARGON) {
        WiFi.disconnect();
        if (!WiFi.clearCredentials()) {
            Serial.println("Failed to clear Wifi credentials");
        }
        if (!WiFi.setCredentials(setSSID.c_str(), buf)) {
            Serial.println("Failed to set Wifi credentials");
        } else {
            Serial.println("WiFi credentials set");
            System.reset();
        }
    }
#endif
}

void configureBLE()
{
    BLE.addCharacteristic(connectedCharacteristic);
    BLE.addCharacteristic(setWifiSSIDCharacteristic);
    BLE.addCharacteristic(setWifiPassCharacteristic);
    BleAdvertisingData advData;
    // Advertise our private service only
    advData.appendServiceUUID(serviceUuid);
    // Continuously advertise when not connected
    BLE.advertise(&advData);
    connectedCharacteristic.setValue(Particle.connected());
}


void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("BLE WiFi Setup running");
    configureBLE();
    Particle.connect();
    Serial.println("Particle connected");
}

void loop()
{

}
