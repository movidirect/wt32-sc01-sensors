
#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 5
#define DHTTYPE DHT22

class Sensors
{
    private:
        DHT_Unified *dht;
        int dhttime = 2000;
        u_int16_t humidity;
        u_int16_t temperature;
    public:
        Sensors();
        void readSensors();
        u_int16_t getHumidity();
        u_int16_t getTemperature();
};
#endif