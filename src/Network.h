#ifndef NETWORK_H
#define NETWORK_H
#include <WiFi.h>
#include <time.h>
#include <Preferences.h>
#include "BluetoothSerial.h"

class Network
{   
    private:
        BluetoothSerial *BT;
        Preferences *preferences;
        void storeWiFiCredentials(const String &ssid, const String &password);
    public:
        Network();
        void connect();
        void readBluetooth();        
};
#endif
