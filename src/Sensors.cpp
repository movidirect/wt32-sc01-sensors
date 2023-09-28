#include "Sensors.h"

Sensors::Sensors()
{
    dht = new DHT_Unified(DHTPIN, DHTTYPE); 
    dht->begin();
}

void Sensors::readSensors()
{
    
    sensors_event_t event;

    dht->temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
        temperature = (u_int16_t)event.temperature;
    }

    dht->humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
        humidity = (u_int16_t)event.relative_humidity;
    }
}

u_int16_t Sensors::getHumidity(){
    return humidity;
}

u_int16_t Sensors::getTemperature(){
    return temperature;
}