#include <Network.h>

Network::Network()
{
    BT = new BluetoothSerial();
    preferences = new Preferences();
    preferences->begin("wifi-params", false);
    BT->begin("ESP32_BT");
}

void Network::storeWiFiCredentials(const String &ssid, const String &password)
{
    preferences->putString("ssid", ssid);
    preferences->putString("password", password);
}

void Network::readBluetooth()
{

    if (BT->available())
    {
        // Read the data received from the Bluetooth connection
        String data = BT->readString();

        //Analyzes the received data to obtain the connection parameters
        //Let's assume that the data is sent in the following format: "SSID:PASSWORD"
        int separatorIndex = data.indexOf(':');
        if (separatorIndex != -1)
        {
            String ssid = data.substring(0, separatorIndex);
            String password = data.substring(separatorIndex + 1, data.length()-2);

            //Store connection parameters in EEPROM
            Serial.println("Guardando credenciales en la memoria EEPROM...");
            storeWiFiCredentials(ssid, password);

            //Try to connect to the WiFi network using the new parameters
            connect();
        }
    }
}

void Network::connect()
{
    //Read the connection parameters stored in Preferences
    String ssid = preferences->getString("ssid", "");
    String password = preferences->getString("password", "");
    Serial.println("Conectando a la red WiFi " + String(ssid.length()) + ":" + String(password.length()) + "...");

    //Verify if valid connection parameters have been stored
    if (ssid.length() > 0 && password.length() > 0)
    {
        WiFi.begin(ssid.c_str(), password.c_str());
        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000)
        {
            delay(500);
            Serial.print(".");
        }

        Serial.println(WiFi.localIP());

        //Here the NTP server is configured to get the time from the internet
        configTime(-18000, 0, "time.nist.gov", "pool.ntp.org");
    }
    else
    {
        Serial.println("No se han almacenado parámetros de conexión WiFi.");
    }
}
