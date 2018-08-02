#include <SimpleDHT.h>
#include "graph.h"

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

byte temperature = 0;
byte humidity = 0;

void printReadings()
{
  tft.setCursor(260, 0);
  tft.print((int)temperature);
  tft.println(" C");

  tft.setCursor(260, 8 * 2);
  tft.print((int)humidity);
  tft.println(" %");
}

void printDhtReadings()
{
  tft.setTextColor(WHITE);
  tft.setTextSize(2);

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

    double y = r;
    Serial.println(y);

    if ((int)x % 5 == 0)
    {
      printDhtReadings();
    }

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
        -5,                                            // ylo = lower bound of y axis
        30,                                            // yhi = upper bound of y asis
        5,                                             // yinc = division of y axis (distance not count)
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
}
