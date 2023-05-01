#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SENSOR_PIN  23   // ESP32 pin GIOP23 connected to DS18B20 sensor's DATA pin

#define LDR1_PIN 15 // ESP32 pin GIOP15 (ADC2_2)
#define LDR2_PIN 4 // ESP32 pin GIOP4 (ADC2_0)
const int LDR_Min = 2000;
bool LDR1 = LOW;
bool LDR2 = LOW;
int NumeroPessoas=0; //Número de pessoas na sala

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
OneWire oneWire(SENSOR_PIN);                                      // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature DS18B20(&oneWire);                              // Pass our oneWire reference to Dallas Temperature sensor

TaskHandle_t SensorPessoas;     // Task handle da função que conta o numero de pessoas
TaskHandle_t SensorTemperatura; // Task handle da função de temperatura
float Temp;                     // Temperatura em Celsius

void setup() 
{
  Serial.begin(9600); // initialize serial

  pinMode(Pino_relay, OUTPUT); //Define o Pino como saída
  pinMode(pinoSensor, INPUT); //Define o Pino como entrada

  DS18B20.begin();      // initialize the DS18B20 sensor
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize OLED display with I2C address 0x3C
  display.clearDisplay();
  display.setTextColor(WHITE);

  xTaskCreatePinnedToCore(
      SensorNumeroTask,   /* Function to implement the task */
      "Task1",            /* Name of the task */
      10000,              /* Stack size in words */
      NULL,               /* Task input parameter */
      0,                  /* Priority of the task */
      &SensorPessoas,     /* Task handle. */
      0);                 /* Core where the task should run */

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

void SensorNumeroTask(void * parameter) // Task para contar número de pessoas na sala (Utilizando LDR por enquanto)
{
  for(;;) 
  {
    if(analogRead(LDR1_PIN)<LDR_Min && analogRead(LDR2_PIN)>LDR_Min && LDR1==LOW && LDR2==LOW)
      LDR1=HIGH;
    if(analogRead(LDR1_PIN)>LDR_Min && analogRead(LDR2_PIN)<LDR_Min && LDR1==HIGH && LDR2==LOW)
      {
        while(analogRead(LDR2_PIN)<LDR_Min)
        { }
          NumeroPessoas++;
          Serial.print("Pesosas na sala: ");
          Serial.println(NumeroPessoas);
          LDR1 = LOW;
          LDR2 = LOW;
      }
    if(analogRead(LDR1_PIN)>LDR_Min && analogRead(LDR2_PIN)<LDR_Min && LDR1==LOW && LDR2==LOW)
      LDR2=HIGH;
    if(analogRead(LDR1_PIN)<LDR_Min && analogRead(LDR2_PIN)>LDR_Min && LDR1==LOW && LDR2==HIGH)
      {
        while(analogRead(LDR1_PIN)<LDR_Min)
        { }
          NumeroPessoas=NumeroPessoas>0?NumeroPessoas-1:0;//Compara se é maior que 0, se sim, diminui o valor, se não iguala a 0
          Serial.print("Pesosas na sala: ");
          Serial.println(NumeroPessoas);
          LDR2 = LOW;
          LDR1 = LOW;
      }
    Serial.println(analogRead(LDR1_PIN));
    Serial.println(analogRead(LDR2_PIN));
  }
}

void Temperatura(void * parameter) // Task para capturar valor da temperatura
{
  for(;;) 
  {
    DS18B20.requestTemperatures();       // comando para pegar temperatura
    Temp = DS18B20.getTempCByIndex(0);   // lê temperatura em °C
  }
}

void loop() 
{
  display.clearDisplay();         // clear display
  //exibir temperatura
  display.setTextSize(1);         // set text size
  display.setCursor(0,0);         // set position to display
  display.print("Temperatura: "); // set text
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(Temp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);            // Escrever o símbolo º 
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  //exibir número de pessoas na sala
  display.setTextSize(1);          // set text size
  display.setCursor(0,32);         // set position to display
  display.print("Numero de pessoas:"); // set text
  display.setTextSize(2);
  display.setCursor(0,42);
  display.print(NumeroPessoas);
  display.display();              // display on OLED
  delay(100);
}
