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
#define TCRT1_PIN 4       // ESP32 pin GIOP4 (ADC2_1)
#define TCRT2_PIN 18      // ESP32 pin GIOP18 (ADC2_0)
#define RELAY_PIN 19      // ESP32 pin GIOP19
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

/*-------- Token de Autenticação ----------- */
char auth[] = "G2U5QNcQP7bSBMxRQa9pyhPybejZEaXS";
/*-------- Configurações de Wi-Fi ----------- */
char ssid[] = "connect-izaias";
char password[] = "244466666"; 

/*-------- Handle das tasks ----------- */
TaskHandle_t SensorPessoas;     // task handle da função que conta o numero de pessoas
TaskHandle_t Sensor_DHT11;      // task handle da função de aquisição do DHT11
float Temp;                     // variável para armazenar Temperatura em Celsius
float Humidity;                 // variável para armazenar a humidade em %
float HITemp;                   // variável para armazenar a sensação térmica em Celcius

int NumeroPessoas = 0;          // número de pessoas na sala

DHT dht(DHTPIN, DHTTYPE);       // declaração do DHT conectado no pino DHTPIN, do tipo DHTTYPE 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaração de um display SSD1306 conectado a I2C (SDA, SCL pins)

// This function creates the timer object. It's part of Blynk library 
BlynkTimer timer;
void myTimer() 
{
  // This function describes what will happen with each timer tick
  // timer é criado para impedir que aconteça uma sobrecarga no servidor
  Blynk.virtualWrite(V0,Temp);      // variável Temp é alocada na variável virtual V0
  Blynk.virtualWrite(V1,Humidity);
  Blynk.virtualWrite(V4,HITemp);
  Blynk.virtualWrite(V3,NumeroPessoas);
}

void setup() 
{
  Serial.begin(115200);                         // Inicializa monitor serial
  Blynk.begin(auth, ssid, password);            // Inicializa o Blynk
  timer.setInterval(1000L, myTimer);            // Inicializa o Timer do blynk para enviar os dados para o APP
  dht.begin();                                  // Inicializa o DHT111
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);    // Inicializa Display OLED com endereço I2C 0x3C
  display.clearDisplay();
  display.setTextColor(WHITE);

  pinMode(TCRT1_PIN, INPUT);    //definindo o pino do sensor como entrada
  pinMode(TCRT2_PIN, INPUT);    //definindo o pino do sensor como entrada
  pinMode(RELAY_PIN, OUTPUT);   //Define o Pino como saída
  digitalWrite(RELAY_PIN, LOW); //LED INICIA DESLIGADO

  xTaskCreatePinnedToCore(
      SensorDHT11,        /* Function to implement the task */
      "Task1",            /* Name of the task */
      10000,              /* Stack size in words */
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
    // Verifica se sensor 1 está coberto
    if(digitalRead(TCRT1_PIN) == 0)
    {
      while(digitalRead(TCRT1_PIN) == 0); // Entra em loop até sensor 1 ser descoberto
        delay(100); //We can put the delay here to avoid people doesn't cover the sensor2 right-after sensor 1 is uncovered
        if(digitalRead(TCRT2_PIN) == 0)   // Se o sensor 2 for coberto logo depois que o sensor 1 é descoberto
        {
          while(digitalRead(TCRT2_PIN)==0); // Entra em loop até sensor 2 ser descoberto
            NumeroPessoas++;                // Incrementa numero de pessoas
        }
    }
    // Decrease also like that
    else if (digitalRead(TCRT2_PIN) == 0)
    {
      while(digitalRead(TCRT2_PIN) == 0);
        delay(100);
        if(digitalRead(TCRT1_PIN) == 0)
        {
          while(digitalRead(TCRT1_PIN)==0);
            if(NumeroPessoas != 0) //Número de pessoa tem que ser > 0
              NumeroPessoas--;     //Decrementa numero de pessoas
        }
    }
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
    // executa o codigo se o widget do botão no APP é pressionado (ON) 
    digitalWrite(RELAY_PIN,HIGH);  // Set digital pin 19 HIGH
  }
  else
  {
    // executa o codigo se o widget do botão no APP é desligado (OFF) 
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
  display.clearDisplay();               // clear display
  //exibir sensação térmica
  display.setTextSize(1);               // set text size
  display.setCursor(0,0);               // set position to display
  display.print("Sensacao Térmica ");   // set text
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(HITemp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);                  // Escrever o símbolo º 
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  //exibir número de pessoas
  display.setTextSize(1);               // set text size
  display.setCursor(0,32);              // set position to display
  display.print("Numero de pessoas:");  // set text
  display.setTextSize(2);
  display.setCursor(0,42);
  display.print(NumeroPessoas);
  display.display();                    // display on OLED
  delay(100);
}
