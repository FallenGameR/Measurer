#include <SimpleDHT.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 5
#define TFT_CS 4
#define TFT_MOSI 3
#define TFT_CLK 2
#define TFT_RST 0
#define TFT_MISO 0

#define ADJ_PIN A5

int pinDHT11 = 6;

SimpleDHT11 dht11;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define LTBLUE 0xB6DF
#define LTTEAL 0xBF5F
#define LTGREEN 0xBFF7
#define LTCYAN 0xC7FF
#define LTRED 0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW 0xFFF8
#define LTORANGE 0xFE73
#define LTPINK 0xFDDF
#define LTPURPLE 0xCCFF
#define LTGREY 0xE71C

#define BLUE 0x001F
#define TEAL 0x0438
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define ORANGE 0xFC00
#define PINK 0xF81F
#define PURPLE 0x8010
#define GREY 0xC618
#define WHITE 0xFFFF
#define BLACK 0x0000

#define DKBLUE 0x000D
#define DKTEAL 0x020C
#define DKGREEN 0x03E0
#define DKCYAN 0x03EF
#define DKRED 0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW 0x8400
#define DKORANGE 0x8200
#define DKPINK 0x9009
#define DKPURPLE 0x4010
#define DKGREY 0x4A49

//Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC);

// this is the only external variable used by the graph
// it's a flat to draw the coordinate system only on the first pass
boolean display1 = true;
boolean display2 = true;
boolean display3 = true;
boolean display4 = true;
boolean display5 = true;
boolean display6 = true;
boolean display7 = true;
boolean display8 = true;
boolean display9 = true;
double ox, oy;

/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = flag to redraw graph on fist call only
*/

void Graph(Adafruit_ILI9341 &d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean &redraw)
{

  double ydiv, xdiv;
  // initialize old x and old y in order to draw the first point of the graph
  // but save the transformed value
  // note my transform funcition is the same as the map function, except the map uses long and we need doubles
  //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
  //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  double i;
  double temp;
  int rot, newrot;

  if (redraw == true)
  {

    redraw = false;
    ox = (x - xlo) * (w) / (xhi - xlo) + gx;
    oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    // draw y scale
    for (i = ylo; i <= yhi; i += yinc)
    {
      // compute the transform
      temp = (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == 0)
      {
        d.drawLine(gx, temp, gx + w, temp, acolor);
      }
      else
      {
        d.drawLine(gx, temp, gx + w, temp, gcolor);
      }

      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(gx - 40, temp);
      // precision is default Arduino--this could really use some format control
      d.println(i);
    }
    // draw x scale
    for (i = xlo; i <= xhi; i += xinc)
    {

      // compute the transform

      temp = (i - xlo) * (w) / (xhi - xlo) + gx;
      if (i == 0)
      {
        d.drawLine(temp, gy, temp, gy - h, acolor);
      }
      else
      {
        d.drawLine(temp, gy, temp, gy - h, gcolor);
      }

      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(temp, gy + 10);
      // precision is default Arduino--this could really use some format control
      d.println(i);
    }

    //now draw the labels
    d.setTextSize(2);
    d.setTextColor(tcolor, bcolor);
    d.setCursor(gx, gy - h - 30);
    d.println(title);

    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(gx, gy + 20);
    d.println(xlabel);

    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(gx - 30, gy - h - 10);
    d.println(ylabel);
  }

  //graph drawn now plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized as static above
  x = (x - xlo) * (w) / (xhi - xlo) + gx;
  y = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  d.drawLine(ox, oy, x, y, pcolor);
  d.drawLine(ox, oy + 1, x, y + 1, pcolor);
  d.drawLine(ox, oy - 1, x, y - 1, pcolor);
  ox = x;
  oy = y;
}

#define X_AXES_PADDING 50
#define Y_AXES_PADDING 30
#define TITLE_PADDING 30

// the value of the 'other' resistor
#define SERIESRESISTOR 1e4

// Termistor resistance at 25c
#define THERMISTORNOMINAL 1e4

// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25

// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950

#define PIN_TERMISOTOR_ENABLE 7

void drawTermistorReading()
{
  boolean display7 = true;

  for (double x = 0; x <= 60; x += 1)
  {
    digitalWrite(PIN_TERMISOTOR_ENABLE, HIGH);
    int vi = analogRead(ADJ_PIN);
    digitalWrite(PIN_TERMISOTOR_ENABLE, LOW);
    double r = SERIESRESISTOR / (1023.0 / vi - 1.0);

    double steinhart;
    steinhart = r / THERMISTORNOMINAL;                // (R/Ro)
    steinhart = log(steinhart);                       // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                        // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                      // Invert
    steinhart -= 273.15;                              // convert to C

    /*
    //  Temp Kelvin
    double tempK = log(r);
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK)) * tempK);

    // Convert Kelvin to Celcius
    double y = tempK - 273.15;

    */

    double y = r;
    Serial.println(y);

    Graph(
        tft,                                           // &d name of your display object
        x,                                             // x = x data point
        steinhart,                                     // y = y datapont
        X_AXES_PADDING,                                // gx = x graph location (lower left) - needed for y axes numbers
        tft.height() - Y_AXES_PADDING,                 // gy = y graph location (lower left) - needed for x axes numbers and title
        tft.width() - X_AXES_PADDING - 1,              // w = width of graph                 - needed for y and last x axes numbers
        tft.height() - Y_AXES_PADDING - TITLE_PADDING, // h = height of graph                - needed for title
        0,                                             // xlo = lower bound of x axis
        60,                                            // xhi = upper bound of x asis
        10,                                            // xinc = division of x axis (distance not count)
        20,                                            // ylo = lower bound of y axis
        30,                                            // yhi = upper bound of y asis
        1,                                             // yinc = division of y axis (distance not count)
        "Temparature",                                 // title = title of graph
        "seconds",                                     // xlabel = x asis label
        "celcius",                                     // ylabel = y asis label
        DKBLUE,                                        // gcolor = graph line colors
        RED,                                           // acolor = axi ine colors
        GREEN,                                         // pcolor = color of your plotted data
        WHITE,                                         // tcolor = text color
        BLACK,                                         // bcolor = background color
        display7);                                     // &redraw = flag to redraw graph on fist call only
    delay(250);
  }

  tft.fillScreen(BLACK);
}

byte temperature = 0;
byte humidity = 0;

void printReadings()
{
  tft.setCursor(0, 0);
  tft.print((int)temperature);
  tft.println(" C");

  tft.print((int)humidity);
  tft.println(" %");
}

void printDhtReadings()
{
  tft.setTextColor(WHITE);
  tft.setTextSize(8);

  byte new_temperature = 0;
  byte new_humidity = 0;
  byte data[40] = {0};
  int err = dht11.read(pinDHT11, &new_temperature, &new_humidity, data);
  if (err)
  {
    Serial.print("Read DHT11 failed, error: ");
    Serial.println(err);
    return;
  }

  bool needToErase = ((temperature != new_temperature) || (humidity != new_humidity));
  needToErase = true;

  if (needToErase)
  {
    Serial.println("erase");

    tft.setTextColor(ILI9341_BLACK);
    printReadings();

    temperature = new_temperature;
    humidity = new_humidity;

    tft.setTextColor(ILI9341_WHITE);
    printReadings();
  }
  else
  {
    Serial.println("the same");
  }

  // DHT11 sampling rate is 1HZ.
  delay(1300);
}

void setup()
{
  Serial.begin(9600);
  //analogReference(EXTERNAL);

  pinMode(PIN_TERMISOTOR_ENABLE, OUTPUT);

  tft.begin();
  tft.setRotation(1);
  Serial.println("ILI9341 screen");
  Serial.print("Width: ");
  Serial.println(tft.width());
  Serial.print("Height: ");
  Serial.println(tft.height());

  tft.fillScreen(BLACK);
}

void loop(void)
{
  drawTermistorReading();
  printDhtReadings();
  delay(1);
}
