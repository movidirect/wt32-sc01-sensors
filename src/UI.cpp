/*
  Author Ramiro Arenas
  Based on project https://github.com/Bodmer/TFT_HX8357 by Bodmer
*/
#include "UI.h"

UI::UI(bool render)
{

  tft = new TFT_eSPI();
  tft->begin();

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, 128);

  tft->setRotation(1);
  tft->fillScreen(TFT_BLACK);

  if (render)
  {
    tft->drawRoundRect(5, 5, 475, 305, 5, TFT_SKYBLUE);
    tft->setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft->drawString("Temperatura", 110, 270, 2);
    tft->drawString("Humedad", 320, 270, 2);
    draw(0, 0);
  }
}

void UI::draw(u_int16_t humidity_value, u_int16_t temperature_value)
{
  if ((millis() - dhttime) > 4000)
  {
    dhttime = millis();
  }

  if (millis() - runTime >= 0L)
  {
    runTime = millis();

    // Test with a slowly changing value from a Sine function
    d += 4;
    if (d >= 360)
      d = 0;

    // Set the the position, gap between meters, and inner radius of the meters
    int xpos = 0, ypos = 5, gap = 4, radius = 52;
    xpos = 60, ypos = 90, gap = 50, radius = 80;
    xpos = gap + ringMeter(temperature_value, minTemp, maxTemp, xpos, ypos, radius, "", temperatureColor(temperature_value)); 
    xpos = gap + ringMeter(humidity_value, 0, 100, xpos, ypos, radius, "", humidityColor(humidity_value));             
    setTime();
    initialized = true;
  }
}

int UI::temperatureColor(u_int16_t value)
{
  if (value < 10) return COLOR_GREEN;
  if (value > 29) return COLOR_RED;

  return COLOR_BLUE;

}

int UI::humidityColor(u_int16_t value)
{
  if (value < 30) return COLOR_GREEN;
  if (value > 80) return COLOR_RED;

  return COLOR_BLUE;
}

bool UI::isInitialized()
{
  return initialized;
}

void UI::setTime()
{
  if (getLocalTime(&new_ts))
  {
    if (new_ts.tm_year > 100)
    {

      tft->setTextDatum(TL_DATUM);
      tft->setTextSize(1);
      tft->setTextColor(TFT_SKYBLUE, TFT_BLACK);

      tft->drawString(formatTime(), 140, 20, 7);
      tft->drawString(formatAMPM(), 305, 52, 2);
      tft->drawString(formatDate(), 150, 291, 2);
    }
  }
}

String UI::formatDate()
{
  char dateStringBuff[50] = {'\0'};
  strftime(dateStringBuff, sizeof(dateStringBuff), "  %A, %B %d %Y  ", &new_ts); //%A, %B %d %Y %H:%M:%S
  return String(dateStringBuff);
}

String UI::formatTime()
{
  char timeStringBuff[30] = {'\0'};
  int hour = new_ts.tm_hour;
  if (hour > 12)
  {
    hour = hour - 12;
  }
  sprintf(timeStringBuff, "  %2d:%02d  ", hour, new_ts.tm_min);
  return String(timeStringBuff);
}

String UI::formatAMPM()
{
  String ampm = "AM";
  if (new_ts.tm_hour > 12)
  {
    ampm = "PM";
  }

  return ampm;
}

int UI::ringMeter(int value, int vmin, int vmax, int x, int y, int r, const char *units, int scheme)
{
    // Minimum value of r is about 52 before value text intrudes on ring
    // drawing the text first is an option

    x += r;
    y += r; // Calculate coords of centre of ring

    int w = r / 3; // Width of outer ring is 1/4 of radius

    int angle = 150; // Half the sweep angle of meter (300 degrees)

    int v = map(value, vmin, vmax, -angle, angle); // Map the value to an angle v

    byte seg = 3; // Segments are 3 degrees wide = 100 segments for 300 degrees
    byte inc = 6; // Draw segments every 3 degrees, increase to 6 for segmented ring

    // Variable to save "value" text colour from scheme and set default
    int colour = TFT_SKYBLUE; // TFT_BLUE;

    // Draw colour blocks every inc degrees
    for (int i = -angle + inc / 2; i < angle - inc / 2; i += inc)
    {
      // Calculate pair of coordinates for segment start
      float sx = cos((i - 90) * 0.0174532925);
      float sy = sin((i - 90) * 0.0174532925);
      uint16_t x0 = sx * (r - w) + x;
      uint16_t y0 = sy * (r - w) + y;
      uint16_t x1 = sx * r + x;
      uint16_t y1 = sy * r + y;

      // Calculate pair of coordinates for segment end
      float sx2 = cos((i + seg - 90) * 0.0174532925);
      float sy2 = sin((i + seg - 90) * 0.0174532925);
      int x2 = sx2 * (r - w) + x;
      int y2 = sy2 * (r - w) + y;
      int x3 = sx2 * r + x;
      int y3 = sy2 * r + y;

      if (i < v)
      { // Fill in coloured segments with 2 triangles
        colour= scheme;
        
        switch (scheme)
        {
        case COLOR_RED:
          colour = TFT_ORANGE;
          break;
        case COLOR_GREEN:
          colour = TFT_GREEN;
          break;
        case COLOR_BLUE:
          colour = TFT_SKYBLUE;
          break;               
        case COLOR_YELLOW:
          colour = TFT_YELLOW;
          break;               
        case COLOR_ORANGE:
          colour = TFT_ORANGE;
          break;               
        default:
          colour = TFT_SKYBLUE;
          break;               
        }
        
        tft->fillTriangle(x0, y0, x1, y1, x2, y2, colour);
        tft->fillTriangle(x1, y1, x2, y2, x3, y3, colour);
        // text_colour = colour; // Save the last colour drawn
      }
      else // Fill in blank segments
      {
        tft->fillTriangle(x0, y0, x1, y1, x2, y2, TFT_DARKGREY);
        tft->fillTriangle(x1, y1, x2, y2, x3, y3, TFT_DARKGREY);
      }
    }
    // Convert value to a string
    char buf[10];
    byte len = 3;
    if (value > 999)
      len = 5;
    dtostrf(value, len, 0, buf);
    buf[len] = ' ';
    buf[len + 1] = 0; // Add blanking space and terminator, helps to centre text too!
    // Set the text colour to default
    tft->setTextSize(2);

    if (value < vmin || value > vmax)
    {
      // drawAlert(x, y + 90, 50, 1);
    }
    else
    {
      // drawAlert(x, y + 90, 50, 0);
    }

    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    // Uncomment next line to set the text colour to the last segment value!
    // tft->setTextColor(colour, TFT_BLACK);
    tft->setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft->setTextDatum(MC_DATUM);
    // Print value, if the meter is large then use big font 8, othewise use 4
    if (r > 84)
    {
      tft->setTextPadding(55 * 3);   // Allow for 3 digits each 55 pixels wide
      tft->drawString(buf, x, y, 8); // Value in middle
    }
    else
    {
      tft->setTextPadding(3 * 14);   // Allow for 3 digits each 14 pixels wide
      tft->drawString(buf, x, y, 4); // Value in middle
    }
    tft->setTextSize(1);
    tft->setTextPadding(0);
    // Print units, if the meter is large then use big font 4, othewise use 2
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    if (r > 84)
      tft->drawString(units, x, y + 60, 4); // Units display
    else
      tft->drawString(units, x, y + 15, 2); // Units display

    // Calculate and return right hand side x coordinate
    return x + r;
}

