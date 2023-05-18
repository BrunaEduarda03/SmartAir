/*-------- Bibliotecas ----------- */
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

/*-------- Configurações ----------- */
#define DHTTYPE DHT11     // define o tipo do DHT
#define DHTPIN  23        // ESP32 pin GIOP23 connected to DHT11 sensor's DATA pin
#define LDR1_PIN 18       // ESP32 pin GIOP15 (ADC2_1)
#define LDR2_PIN 4        // ESP32 pin GIOP4 (ADC2_0)
#define RELAY_PIN 19      // ESP32 pin GIOP19
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

/*-------- Token de Autenticação ----------- */
char auth[] = "G2U5QNcQP7bSBMxRQa9pyhPybejZEaXS";
/*-------- Configurações de Wi-Fi ----------- */
char ssid[] = "connect-izaias";
char password[] = "244466666"; 

const int LDR_Min = 2000;
bool LDR1 = LOW;
bool LDR2 = LOW;
int NumeroPessoas = 0; //Número de pessoas na sala

/*-------- Handle das tasks ----------- */
TaskHandle_t SensorPessoas;     // Task handle da função que conta o numero de pessoas
TaskHandle_t Sensor_DHT11;      // Task handle da função de aquisição do DHT11
float Temp;                     // Variável para armazenar Temperatura em Celsius
float Humidity;                 // Variável para armazenar a humidade em %
float HITemp;                   // Variável para armazenar a sensação térmica em Celcius

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

// This function creates the timer object. It's part of Blynk library 
BlynkTimer timer;
void myTimer() 
{
  // This function describes what will happen with each timer tick
  // timer é criado para impedir que aconteça uma sobrecarga no servidor
  Blynk.virtualWrite(V0,Temp);
  Blynk.virtualWrite(V1,Humidity);
  Blynk.virtualWrite(V4,HITemp);
  Blynk.virtualWrite(V3,NumeroPessoas);
}

void setup() 
{
  Serial.begin(9600); // initialize serial
  Blynk.begin(auth, ssid, password);
  timer.setInterval(1000L, myTimer); 

  dht.begin();                           // initialize the DS18B20 sensor
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize OLED display with I2C address 0x3C
  display.clearDisplay();
  display.setTextColor(WHITE);

  pinMode(RELAY_PIN, OUTPUT);  //Define o Pino como saída
  digitalWrite(RELAY_PIN, LOW); //LED INICIA DESLIGADO

  xTaskCreatePinnedToCore(
      SensorDHT11,        /* Function to implement the task */
      "Task1",            /* Name of the task */
      10000,               /* Stack size in words */
      NULL,               /* Task input parameter */
      0,                  /* Priority of the task */
      &Sensor_DHT11,      /* Task handle. */
      0);                 /* Core where the task should run */

  xTaskCreatePinnedToCore(
      SensorNumeroTask,   /* Function to implement the task */
      "Task2",            /* Name of the task */
      10000,              /* Stack size in words */
      NULL,               /* Task input parameter */
      0,                  /* Priority of the task */
      &SensorPessoas,     /* Task handle. */
      0);                 /* Core where the task should run */

  delay(1000);
}

/*-------- Função para verificar entrada de pessoas na sala ----------- */ //(Utilizando LDR por enquanto)
void SensorNumeroTask(void * parameter) 
{
  for(;;) 
  {
    if(analogRead(LDR1_PIN) < LDR_Min && analogRead(LDR2_PIN) > LDR_Min && LDR1 == LOW && LDR2 == LOW)
      LDR1 = HIGH;
    if(analogRead(LDR1_PIN) > LDR_Min && analogRead(LDR2_PIN) < LDR_Min && LDR1 == HIGH && LDR2 == LOW)
      {
        while(analogRead(LDR2_PIN)<LDR_Min)
        { }
          NumeroPessoas++;
          Serial.print("Pesosas na sala: ");
          Serial.println(NumeroPessoas);
          LDR1 = LOW;
          LDR2 = LOW;
      }
    if(analogRead(LDR1_PIN) > LDR_Min && analogRead(LDR2_PIN) < LDR_Min && LDR1 == LOW && LDR2 == LOW)
      LDR2 = HIGH;
    if(analogRead(LDR1_PIN) < LDR_Min && analogRead(LDR2_PIN) > LDR_Min && LDR1 == LOW && LDR2 == HIGH)
      {
        while(analogRead(LDR1_PIN) < LDR_Min)
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

/*-------- Task para capturar valor da temperatura e Humidade ----------- */
void SensorDHT11(void * parameter)
{
  for(;;) 
  {
    Humidity = dht.readHumidity();                        // lê a humidade do ar
    Temp = dht.readTemperature();                         // lê temperatura em °C
    HITemp = dht.computeHeatIndex(Temp, Humidity, false); //calcula o indice de calor em celcius
    delay(100);
  }
}

BLYNK_WRITE(V2) // Executes when the value of virtual pin 2 changes
{
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(RELAY_PIN,HIGH);  // Set digital pin 19 HIGH
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(RELAY_PIN,LOW);  // Set digital pin 19 LOW 
  }
}

void loop() 
{
  // Runs all Blynk stuff
  Blynk.run(); 
  // runs BlynkTimer
  timer.run(); 

  /*-------- Exibe os valores no Display OLED ----------- */
  display.clearDisplay();              // clear display
  //exibir sensação térmica
  display.setTextSize(1);               // set text size
  display.setCursor(0,0);              // set position to display
  display.print("Sensacao Térmica "); // set text
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(HITemp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);            // Escrever o símbolo º 
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  //exibir número de pessoas
  display.setTextSize(1);         // set text size
  display.setCursor(0,32);         // set position to display
  display.print("Numero de pessoas:"); // set text
  display.setTextSize(2);
  display.setCursor(0,42);
  display.print(NumeroPessoas);
  display.display();              // display on OLED
  delay(100);
}
