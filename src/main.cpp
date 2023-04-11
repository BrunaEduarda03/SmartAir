#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SENSOR_PIN  23   // ESP32 pin GIOP23 connected to DS18B20 sensor's DATA pin
const int Fototransistor = 2;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
OneWire oneWire(SENSOR_PIN);                                      // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature DS18B20(&oneWire);                              // Pass our oneWire reference to Dallas Temperature sensor

TaskHandle_t SensorTemperatura; // Task handle da função de temperatura

float Temp; // Temperatura em Celsius

void setup() 
{
  
  pinMode(Fototransistor, INPUT);

  Serial.begin(115200); // initialize serial

  DS18B20.begin();      // initialize the DS18B20 sensor
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize OLED display with I2C address 0x3C

  display.clearDisplay();
  display.setTextColor(WHITE);

  xTaskCreatePinnedToCore(
      Temperatura,        /* Function to implement the task */
      "Task1",            /* Name of the task */
      10000,              /* Stack size in words */
      NULL,               /* Task input parameter */
      0,                  /* Priority of the task */
      &SensorTemperatura, /* Task handle. */
      0);                 /* Core where the task should run */

  delay(1000);
}

void Temperatura(void * parameter) // Task para capturar valor da temperatura
{
  for(;;) 
  {
    DS18B20.requestTemperatures();       // comando para pegar temperatura
    Temp = DS18B20.getTempCByIndex(0);   // lê temperatura em °C

    Serial.print("Temperatura: ");
    Serial.print(Temp);                  // print da temperature em °C
    Serial.println("°C");
  }
}

void loop() 
{
  display.clearDisplay();         // clear display
  // display temperature
  display.setTextSize(1);         // set text size
  display.setCursor(0,0);         // set position to display
  display.print("Temperature: "); // set text
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(Temp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);            // Escrever o símbolo º 
  display.write(167);
  display.setTextSize(2);
  display.print("C");

  display.display();              // display on OLED
  delay(100);
}
