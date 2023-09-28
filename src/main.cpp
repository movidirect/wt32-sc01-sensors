/*
    Just for fun and learning purposes.
    Demo Running in WT32-SC01, a ESP32 board with OLED display and DHT11 sensor.
*/

#include <UI.h>
#include <Network.h>
#include <Sensors.h>

Network *network;
UI *ui;
Sensors *sensors;

void setup(void)
{
    Serial.begin(115200);

    sensors = new Sensors();
    ui = new UI(true);
    network = new Network();
    network->connect();

}

void loop()
{
    if(ui->isInitialized()){
        network->readBluetooth();
    }
    sensors->readSensors();
    ui->draw(sensors->getHumidity(),sensors->getTemperature());

    delay(3000);
}
