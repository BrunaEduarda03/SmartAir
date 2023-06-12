#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

/*-------- Configurações ----------- */  
#define DHTTYPE DHT11     // define o tipo do DHT
#define DHTPIN  23        // ESP32 pin GIOP23 connected to DHT11 sensor's DATA pin
#define TCRT1_PIN 4       // ESP32 pin GIOP4 (ADC2_1)
#define TCRT2_PIN 18      // ESP32 pin GIOP18 (ADC2_0)
#define RELAY_PIN 19      // ESP32 pin GIOP19
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
/*-------- Variáveis ----------- */


const uint16_t kIrLed = 19;     // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);          // Set the GPIO to be used to sending the message.
// Código RAW (ligar)
uint16_t rawData[211] = {9042, 4512,  546, 1700,  546, 1698,  554, 558,  536, 586,  516, 596,  512, 600,  514, 1710,  544, 1716,  522, 1728,  518, 
1726,  518, 1724,  512, 600,  512, 1734,  508, 600,  512, 1732,  516, 592,  512, 596,  518, 592,  512, 592,  514, 596,  516, 592,  510, 1736,  514,
 1730,  510, 1734,  518, 590,  514, 594,  512, 596,  520, 588,  514, 596,  514, 592,  518, 594,  510, 600,  512, 594,  514, 594,  508, 598,  518, 590,
   516, 592,  514, 1734,  514, 1728,  516, 594,  516, 590,  520, 586,  522, 594,  508, 598,  516, 588,  522, 592,  514, 582,  532, 586,  518, 592,  
   522, 586,  520, 590,  514, 596,  510, 588,  524, 1728,  514, 596,  516, 592,  512, 586,  524, 596,  516, 590,  518, 592,  518, 592,  512, 586,  
   528, 588,  516, 594,  512, 590,  516, 586,  526, 592,  516, 592,  512, 596,  514, 590,  522, 586,  520, 590,  516, 594,  514, 598,  514, 594,  
   510, 598,  514, 590,  518, 1730,  510, 594,  518, 596,  514, 580,  528, 582,  532, 588,  508, 596,  512, 590,  526, 590,  514, 594,  518, 582, 
    524, 1732,  514, 584,  522, 1732,  516, 590,  518, 584,  524, 588,  520, 592,  518, 584,  520, 1732,  516, 1724,  514, 1732,  518, 1724,  510, 
    1738,  516, 584,  518, 588,  526, 1728,  512};

//desligar
uint16_t rawData1[211] = {9036, 4542,  508, 1736,  506, 1738,  512, 596,  510, 600,  510, 600,  510, 598,  512, 1734,  512, 1732,  510, 1738,  506, 1740,  508, 1736,  510,
 598,  512, 1732,  510, 598,  510, 1736,  508, 600,  508, 598,  510, 598,  508, 600,  512, 596,  508, 600,  514, 1734,  512, 1732,  510, 1736,  510, 596,  512, 596,  510, 
 600,  520, 590,  510, 600,  510, 596,  508, 600,  512, 598,  514, 594,  512, 596,  510, 598,  518, 592,  516, 592,  510, 1736,  504, 1738,  514, 594,  510, 596,  516, 594,  
 512, 596,  516, 594,  512, 596,  508, 598,  518, 592,  518, 590,  512, 596,  510, 600,  512, 596,  508, 600,  514, 594,  512, 1734,  512, 594,  514, 594,  514, 594,  516, 594, 
  510, 598,  516, 592,  516, 590,  516, 594,  512, 596,  514, 592,  520, 592,  512, 596,  516, 594,  512, 596,  512, 596,  516, 592,  516, 594,  510, 598,  516, 592,  508, 600,
    510, 600,  512, 596,  508, 600,  516, 590,  514, 594,  514, 598,  514, 594,  512, 596,  512, 598,  512, 594,  512, 596,  518, 592,  514, 594,  514, 596,  514, 1714,  522, 604,
      512, 1710,  528, 602,  514, 594,  516, 592,  508, 598,  510, 600,  516, 1728,  498, 1746,  506, 1736,  514, 1730,  520, 1726,  518, 1724,  526, 1714,  526, 590,  528};

/*-------- Token de Autenticação ----------- */
char auth[] = "G2U5QNcQP7bSBMxRQa9pyhPybejZEaXS";
/*-------- Configurações de Wi-Fi ----------- */
char ssid[] = "connect-izaias";
char password[] = "244466666"; 

TaskHandle_t Sensor_Task;       // task handle da função dos sensores
int estado = 0;
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

BLYNK_WRITE(V2) // Executes when the value of virtual pin 2 changes
{
  if(param.asInt() == 0)
  {
    if(estado == 0){
      Serial.println("Ligando");
      irsend.sendRaw(rawData, 221, 38);  // Send a raw data capture at 38kHz.
      delay(1000);
      estado = 1;
    }
    else{
      
      Serial.println("Desligando");
      irsend.sendRaw(rawData1, 221, 38);  // Send a raw data capture at 38kHz.
      delay(1000);
      estado = 0;
    }
    
  }
}

void setup() 
{
  Serial.begin(115200);                         // Inicializa monitor serial
  irsend.begin();                               // Inicializa o envio por infravermelho
  Blynk.begin(auth, ssid, password);            // Inicializa o Blynk
  timer.setInterval(1000L, myTimer);            // Inicializa o Timer do blynk para enviar os dados para o APP
  dht.begin();                                  // Inicializa o DHT111
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);    // Inicializa Display OLED com endereço I2C 0x3C
  display.clearDisplay();
  display.setTextColor(WHITE);

  pinMode(TCRT1_PIN, INPUT);    //definindo o pino do sensor como entrada
  pinMode(TCRT2_PIN, INPUT);    //definindo o pino do sensor como entrada

  xTaskCreatePinnedToCore(
      SensoresTask,       /* Function to implement the task */
      "Task2",            /* Name of the task */
      10000,              /* Stack size in words */
      NULL,               /* Task input parameter */
      0,                  /* Priority of the task */
      &Sensor_Task,       /* Task handle. */
      0);                 /* Core where the task should run */

  delay(1000);
}

/*-------- Função para verificar entrada de pessoas na sala ----------- */
void SensoresTask(void * parameter) 
{
  for(;;) 
  {
    /*--------  captura valor da temperatura e Humidade ----------- */
    Humidity = dht.readHumidity();                        // lê a humidade do ar
    Temp = dht.readTemperature();                         // lê temperatura em °C
    HITemp = dht.computeHeatIndex(Temp, Humidity, false); //calcula o indice de calor em celcius

    /*-------- Verifica se sensor 1 está coberto ----------- */
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
