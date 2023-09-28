/*
  Author Ramiro Arenas
  Based on project https://github.com/Bodmer/TFT_HX8357 by Bodmer
*/
#ifndef UI_H
#define UI_H
#include <TFT_eSPI.h>
#include <SPI.h>

#define minimum(a,b) (((a) < (b)) ? (a) : (b))

//DEFINE COLOR IN BYTES
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_BLUE 3
#define COLOR_YELLOW 4
#define COLOR_ORANGE 5

class UI
{
    private:
        TFT_eSPI *tft;
        boolean range_error = 0;
        u_int16_t humidity_value = 0;
        u_int16_t temperature_value = 0;
        uint32_t runTime = -99999; 
        int d = 0;
        int dhttime = 2000;
        bool initialized = false;
        int minTemp = 0;
        int maxTemp = 50;
        struct tm new_ts;
        int temperatureColor(u_int16_t value);
        int humidityColor(u_int16_t value);
        int ringMeter(int value, int vmin, int vmax, int x, int y, int r, const char *units, int scheme);
        String formatDate();
        String formatTime();
        String formatAMPM();
        
    public:
        UI(bool render);
        
        void setTime();
        void draw(u_int16_t humidity_value,u_int16_t temperature_value);
        bool isInitialized();   
  

};
#endif
