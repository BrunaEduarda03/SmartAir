// Bibliotecas
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Configurações gerais

// Token de Autenticação do Blynk. 
char BLYNK_AUTH_TOKEN[] = "G2U5QNcQP7bSBMxRQa9pyhPybejZEaXS";
// Credenciais do WiFi.
char WIFI_SSID[] = "connect-izaias";
char WIFI_PASS[] = "244466666"; 

// Configurações do Display OLED SSD1306
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaração de um display SSD1306 conectado a I2C (SDA, SCL pins)

// Configurações do sensor DHT.
#define DHTTYPE DHT11     // Define o tipo do DHT.
#define DHTPIN  23        // ESP32 pin GIOP23 connected to DHT11 sensor's DATA pin.
DHT dht(DHTPIN, DHTTYPE); // declaração do DHT conectado no pino DHTPIN, do tipo DHTTYPE. 
float Temp;               // variável para armazenar Temperatura em Celsius.
float Humidity;           // variável para armazenar a humidade em %.
float HITemp;             // variável para armazenar a sensação térmica em Celcius.

// Configurações dos sensores de Proximidade infravermelho
#define TCRT1_PIN 4       // ESP32 pin GIOP4 (ADC2_1)
#define TCRT2_PIN 18      // ESP32 pin GIOP18 (ADC2_0)
int NumeroPessoas = 0;    // Variavel para o número de pessoas na sala.

// Variavies relacionadas ao Ar condicionado
bool estado = false;            // Estado do ar condicionado (Ligado/Desligado).
int TempAC;                     // Temperatura do Ar condicionado.
const uint16_t kIrLed = 19;     // ESP32 GPIO utilizado pelo led infravermelho.
IRsend irsend(kIrLed);          // Confiura o pino que vai ser utilziado para enviar a mensagem.
// Todos os códigos RAW, utilizados para enviar o comando para o ar condiconado.
// Código RAW (ligar em 18C°)
uint16_t rawData[211] = {9042, 4512, 546, 1700, 546, 1698, 554, 558, 536, 586, 516, 596, 512, 600, 514, 1710, 544, 1716, 522, 1728, 518, 1726, 518, 1724, 512, 600, 512, 1734, 508, 600, 512, 1732, 516, 592, 512, 596, 518, 592, 512, 
592, 514, 596, 516, 592, 510, 1736, 514, 1730, 510, 1734, 518, 590, 514, 594, 512, 596, 520, 588, 514, 596, 514, 592, 518, 594, 510, 600, 512, 594, 514, 594, 508, 598, 518, 590, 516, 592, 514, 1734, 514, 1728, 516, 594, 516, 590, 
520, 586, 522, 594, 508, 598, 516, 588, 522, 592, 514, 582, 532, 586, 518, 592, 522, 586, 520, 590, 514, 596, 510, 588, 524, 1728, 514, 596, 516, 592, 512, 586, 524, 596, 516, 590, 518, 592, 518, 592, 512, 586, 528, 588, 516, 594, 
512, 590, 516, 586, 526, 592, 516, 592, 512, 596, 514, 590, 522, 586, 520, 590, 516, 594, 514, 598, 514, 594, 510, 598, 514, 590, 518, 1730, 510, 594, 518, 596, 514, 580, 528, 582, 532, 588, 508, 596, 512, 590, 526, 590, 514, 594, 
518, 582, 524, 1732, 514, 584, 522, 1732, 516, 590, 518, 584, 524, 588, 520, 592, 518, 584, 520, 1732, 516, 1724, 514, 1732, 518, 1724, 510, 1738, 516, 584, 518, 588, 526, 1728, 512};
// Código RAW (desligar)
uint16_t rawData0[211] = {9036, 4542, 508, 1736, 506, 1738, 512, 596, 510, 600, 510, 600, 510, 598, 512, 1734, 512, 1732, 510, 1738, 506, 1740, 508, 1736, 510, 598, 512, 1732, 510, 598, 510, 1736, 508, 600, 508, 598, 510, 598, 508, 
600, 512, 596, 508, 600, 514, 1734, 512, 1732, 510, 1736, 510, 596, 512, 596, 510, 600, 520, 590, 510, 600, 510, 596, 508, 600, 512, 598, 514, 594, 512, 596, 510, 598, 518, 592, 516, 592, 510, 1736, 504, 1738, 514, 594, 510, 596, 
516, 594, 512, 596, 516, 594, 512, 596, 508, 598, 518, 592, 518, 590, 512, 596, 510, 600, 512, 596, 508, 600, 514, 594, 512, 1734, 512, 594, 514, 594, 514, 594, 516, 594, 510, 598, 516, 592, 516, 590, 516, 594, 512, 596, 514, 592, 
520, 592, 512, 596, 516, 594, 512, 596, 512, 596, 516, 592, 516, 594, 510, 598, 516, 592, 508, 600, 510, 600, 512, 596, 508, 600, 516, 590, 514, 594, 514, 598, 514, 594, 512, 596, 512, 598, 512, 594, 512, 596, 518, 592, 514, 594, 
514, 596, 514, 1714, 522, 604,512, 1710, 528, 602, 514, 594, 516, 592, 508, 598, 510, 600, 516, 1728, 498, 1746, 506, 1736, 514, 1730, 520, 1726, 518, 1724, 526, 1714, 526, 590, 528};
// Código RAW (16C°)
uint16_t rawData16[211] = {9032, 4536, 518, 1724, 514, 1730, 518, 588, 520, 578, 530, 588, 516, 594, 512, 1724, 528, 1710, 534, 1724, 520, 1710, 532, 1716, 532, 578, 534, 572, 534, 574, 534, 1718, 530, 568, 536, 578, 534, 576, 534, 
568, 538, 574, 532, 578, 534, 1706, 542, 1704, 532, 1710, 538, 570, 536, 572, 536, 574, 538, 572, 538, 572, 536, 576, 534, 568, 544, 568, 530, 578, 536, 584, 524, 578, 528, 574, 538, 576, 532, 1710, 534, 1724, 516, 592, 520, 578, 
528, 590, 524, 578, 524, 598, 518, 590, 512, 596, 516, 590, 518, 594, 516, 592, 516, 592, 514, 596, 512, 596, 514, 592, 518, 1728, 518, 592, 512, 596, 512, 596, 514, 596, 516, 592, 514, 592, 512, 596, 516, 594, 514, 594, 514, 594, 
514, 594, 516, 592, 516, 604, 502, 604, 502, 600, 514, 600, 510, 594, 512, 598, 512, 604, 502, 600, 508, 602, 504, 602, 510, 594, 512, 1730, 518, 594, 514, 594, 512, 596, 516, 592, 516, 590, 516, 592, 512, 596, 508, 600, 510, 598, 
512, 596, 514, 1724, 518, 608, 502, 594, 512, 602, 504, 600, 510, 596, 516, 590, 512, 596, 510, 1730, 512, 1732, 516, 600, 506, 1728, 514, 604, 508, 612, 496, 596, 514, 1726, 520};
// Código RAW (17C°)
uint16_t rawData17[211] = {9040, 4546, 494, 1746, 502, 1744, 502, 606, 508, 600, 512, 596, 514, 594, 508, 1742, 504, 1740, 512, 1732, 512, 1732, 510, 1736, 512, 1732, 512, 594, 510, 598, 512, 1736, 514, 592, 516, 594, 514, 592, 514, 
596, 512, 596, 512, 598, 512, 1732, 518, 1726, 516, 1728, 518, 592, 514, 596, 512, 598, 512, 598, 508, 600, 514, 594, 520, 588, 512, 596, 514, 594, 516, 594, 512, 596, 512, 596, 514, 594, 514, 1726, 512, 1728, 520, 598, 514, 594, 
514, 594, 512, 596, 512, 596, 508, 600, 504, 602, 508, 602, 510, 598, 506, 602, 510, 598, 514, 594, 508, 600, 510, 598, 506, 1714, 536, 594, 506, 602, 510, 600, 508, 598, 508, 600, 508, 600, 512, 596, 508, 600, 508, 596, 510, 600, 
512, 596, 512, 598, 508, 600, 514, 594, 506, 602, 512, 596, 512, 598, 510, 598, 506, 602, 510, 598, 510, 598, 510, 598, 510, 600, 508, 1730, 502, 608, 514, 594, 514, 594, 504, 606, 518, 590, 512, 600, 504, 602, 512, 596, 508, 600, 
508, 600, 512, 598, 510, 596, 512, 596, 512, 596, 508, 598, 512, 598, 506, 602, 512, 598, 510, 600, 512, 1732, 502, 608, 510, 596, 516, 1730, 510, 598, 510, 598, 512, 1732, 520};
// Código RAW (18C°)
uint16_t rawData18[211] = {9128, 4428, 616, 1630, 606, 1642, 598, 512, 586, 524, 576, 534, 564, 546, 546, 1702, 534, 1710, 530, 1732, 510, 1740, 506, 1736, 510, 600, 508, 1744, 506, 598, 512, 1734, 518, 588, 514, 598, 512, 600, 510, 
596, 516, 592, 518, 588, 520, 1732, 510, 1734, 516, 1726, 512, 600, 516, 596, 508, 596, 512, 594, 518, 580, 528, 590, 518, 588, 520, 590, 526, 590, 516, 588, 520, 582, 526, 594, 510, 596, 516, 1734, 510, 1726, 524, 576, 530, 586, 
522, 590, 524, 586, 520, 582, 526, 586, 520, 592, 524, 586, 520, 590, 514, 592, 524, 584, 526, 574, 534, 574, 534, 586, 516, 1732, 520, 580, 528, 590, 518, 582, 524, 584, 526, 588, 518, 582, 530, 574, 534, 578, 534, 578, 524, 594, 
512, 602, 512, 590, 518, 586, 522, 592, 518, 584, 526, 588, 518, 590, 516, 592, 518, 594, 516, 580, 534, 588, 518, 586, 526, 590, 518, 1718, 522, 594, 514, 592, 512, 596, 512, 602, 516, 588, 520, 588, 516, 584, 524, 594, 516, 596, 
512, 590, 520, 594, 512, 596, 512, 598, 510, 592, 512, 598, 514, 594, 514, 600, 506, 598, 518, 594, 514, 1726, 514, 592, 520, 1732, 512, 1726, 520, 582, 524, 592, 516, 1736, 510};
// Código RAW (19C°)
uint16_t rawData19[211] = {9120, 4474, 572, 1674, 560, 1688, 546, 562, 538, 570, 528, 582, 514, 598, 506, 1744, 500, 1754, 486, 1766, 486, 1754, 484, 1760, 488, 1762, 488, 1754, 500, 600, 506, 1754, 490, 600, 512, 598, 510, 598, 504, 
606, 512, 596, 506, 604, 512, 1732, 516, 1732, 506, 1736, 508, 600, 508, 600, 510, 600, 512, 598, 508, 600, 506, 602, 512, 596, 512, 594, 516, 592, 508, 600, 508, 600, 508, 600, 512, 596, 510, 1738, 506, 1738, 510, 596, 510, 598, 
510, 598, 514, 594, 508, 602, 516, 594, 512, 598, 510, 596, 512, 598, 508, 600, 504, 606, 512, 596, 508, 602, 514, 596, 506, 1740, 514, 594, 506, 604, 508, 600, 512, 596, 506, 602, 516, 592, 506, 604, 506, 602, 506, 602, 510, 600, 
510, 598, 508, 602, 504, 614, 498, 612, 490, 612, 506, 600, 504, 606, 508, 598, 508, 604, 500, 612, 504, 602, 504, 604, 498, 610, 504, 1732, 516, 596, 516, 592, 512, 596, 510, 600, 508, 598, 510, 598, 508, 602, 510, 596, 510, 598, 
510, 600, 510, 598, 508, 600, 508, 600, 510, 598, 514, 594, 508, 602, 510, 598, 514, 596, 514, 594, 508, 1734, 508, 604, 510, 598, 508, 600, 520, 1722, 512, 600, 506, 1730, 516};
// Código RAW (20°)
uint16_t rawData20[211] = {9036, 4520, 532, 1720, 526, 1712, 534, 576, 528, 582, 528, 582, 530, 582, 524, 1716, 532, 1718, 526, 1720, 526, 1714, 538, 1708, 530, 578, 536, 574, 532, 1710, 538, 1710, 530, 578, 534, 572, 534, 576, 536, 
574, 534, 576, 532, 576, 534, 1712, 532, 1710, 536, 1710, 536, 570, 540, 570, 536, 572, 538, 570, 534, 572, 532, 580, 530, 576, 538, 570, 538, 570, 536, 572, 534, 574, 536, 574, 542, 566, 532, 1714, 534, 1712, 536, 572, 536, 572, 538, 
570, 536, 574, 536, 572, 532, 578, 532, 574, 532, 578, 536, 572, 532, 576, 534, 574, 534, 574, 536, 574, 534, 574, 536, 1710, 534, 574, 538, 570, 534, 576, 538, 570, 534, 574, 534, 574, 536, 572, 532, 574, 536, 574, 538, 572, 538, 572, 
536, 572, 536, 572, 538, 570, 528, 580, 532, 578, 532, 578, 532, 578, 542, 564, 534, 576, 536, 572, 536, 572, 534, 574, 542, 1706, 540, 568, 532, 576, 530, 580, 534, 582, 532, 570, 534, 574, 530, 578, 534, 586, 522, 580, 530, 578, 
530, 578, 534, 572, 534, 576, 528, 584, 524, 588, 520, 582, 526, 588, 526, 584, 520, 590, 522, 1730, 516, 592, 520, 1720, 520, 580, 522, 1730, 520, 584, 522, 1722, 526};
// Código RAW (21°)
uint16_t rawData21[211] = {9096, 4454, 586, 1660, 576, 1672, 558, 550, 550, 560, 534, 584, 516, 600, 516, 1724, 516, 1738, 512, 1726, 516, 1732, 508, 1744, 506, 1740, 506, 586, 522, 1740, 508, 1736, 508, 592, 524, 584, 520, 596, 510, 
602, 508, 600, 512, 594, 520, 1732, 512, 1732, 516, 1720, 522, 594, 514, 596, 512, 596, 512, 590, 522, 588, 528, 578, 526, 592, 514, 600, 512, 592, 518, 590, 520, 586, 524, 586, 518, 600, 512, 1736, 512, 1722, 522, 592, 516, 588, 522, 
594, 514, 594, 520, 588, 520, 594, 516, 588, 512, 590, 522, 594, 514, 590, 522, 582, 522, 588, 520, 596, 518, 594, 512, 1734, 516, 586, 522, 582, 528, 582, 524, 598, 512, 598, 510, 592, 520, 586, 522, 594, 514, 588, 518, 596, 518, 
588, 520, 586, 524, 584, 528, 590, 518, 590, 516, 586, 522, 580, 534, 582, 524, 590, 520, 582, 522, 592, 518, 588, 524, 586, 522, 1722, 526, 584, 520, 596, 512, 598, 518, 588, 516, 586, 522, 588, 522, 590, 514, 596, 516, 582, 526, 
582, 522, 594, 518, 582, 524, 596, 516, 582, 530, 582, 520, 582, 526, 582, 532, 578, 532, 582, 520, 1728, 516, 588, 522, 584, 520, 1740, 512, 1726, 518, 582, 532, 1724, 520};
// Código RAW (22°)
uint16_t rawData22[211] = {9108, 4448, 586, 1662, 576, 1670, 564, 548, 550, 566, 530, 588, 516, 596, 506, 1742, 510, 1736, 506, 1740, 510, 1736, 518, 1728, 508, 602, 504, 1740, 506, 1740, 510, 1736, 512, 596, 508, 600, 506, 602, 
514, 596, 510, 598, 514, 596, 514, 1732, 514, 1728, 516, 1732, 510, 600, 512, 590, 518, 596, 510, 596, 514, 588, 520, 600, 512, 596, 510, 598, 512, 586, 524, 590, 516, 596, 516, 596, 516, 594, 516, 1726, 520, 1730, 514, 590, 516, 
594, 520, 588, 518, 586, 520, 598, 514, 588, 520, 594, 512, 592, 516, 586, 528, 594, 514, 586, 524, 594, 518, 590, 516, 584, 526, 1730, 516, 592, 512, 596, 514, 594, 516, 594, 514, 592, 512, 600, 510, 600, 512, 598, 518, 590, 508, 
600, 514, 596, 516, 592, 516, 594, 512, 596, 512, 596, 512, 596, 512, 596, 508, 600, 510, 600, 504, 606, 512, 598, 512, 596, 512, 596, 510, 1740, 512, 596, 512, 594, 508, 602, 510, 600, 510, 596, 512, 598, 516, 592, 514, 594, 508, 
604, 504, 604, 506, 600, 506, 604, 508, 600, 506, 602, 508, 600, 512, 600, 514, 594, 512, 598, 510, 598, 508, 1740, 508, 600, 510, 1734, 510, 1736, 508, 1736, 506, 602, 504, 1738, 510};
// Código RAW (23°)
uint16_t rawData23[211] = {9090, 4486, 558, 1690, 544, 1706, 532, 576, 524, 586, 516, 594, 504, 606, 502, 1744, 510, 1738, 506, 1740, 502, 1742, 508, 1740, 508, 1736, 506, 1738, 510, 1736, 512, 1732, 518, 578, 530, 586, 522, 588, 520,
576, 538, 576, 530, 574, 536, 1710, 530, 1716, 534, 1710, 528, 582, 530, 578, 532, 580, 532, 578, 526, 578, 536, 572, 538, 574, 528, 584, 528, 586, 524, 584, 520, 588, 522, 582, 524, 584, 530, 1724, 514, 1738, 512, 586, 516, 600, 
516, 594, 512, 598, 518, 582, 524, 586, 520, 592, 520, 596, 512, 594, 512, 592, 516, 594, 518, 594, 514, 584, 524, 596, 520, 1724, 520, 590, 516, 594, 512, 594, 516, 592, 516, 596, 514, 600, 512, 592, 518, 588, 522, 594, 514, 592,
508, 600, 516, 590, 518, 590, 516, 594, 512, 600, 514, 598, 510, 596, 512, 584, 526, 590, 516, 584, 528, 588, 520, 592, 512, 596, 518, 1716, 528, 588, 522, 594, 516, 590, 516, 588, 528, 584, 522, 584, 522, 594, 512, 600, 512, 596,
512, 596, 512, 594, 520, 592, 518, 596, 508, 594, 516, 600, 514, 586, 526, 572, 530, 584, 524, 584, 528, 1724, 524, 586, 524, 586, 520, 582, 524, 590, 522, 1726, 510, 1730, 524};
// Código RAW (24°)
uint16_t rawData24[211] = {9038, 4542, 510, 1730, 514, 1732, 514, 594, 512, 596, 516, 596, 508, 598, 512, 1732, 516, 1732, 508, 1736, 518, 1726, 516, 1732, 508, 600, 520, 590, 516, 592, 516, 594, 512, 1734, 510, 598, 512, 596, 516, 
594, 510, 598, 512, 598, 512, 1732, 516, 1728, 520, 1724, 516, 596, 520, 588, 516, 592, 516, 592, 514, 596, 516, 592, 520, 590, 516, 592, 516, 594, 510, 600, 512, 596, 514, 596, 512, 596, 512, 1710, 534, 1708, 546, 588, 520, 588, 
520, 588, 516, 590, 516, 592, 512, 598, 514, 594, 516, 592, 514, 594, 512, 594, 518, 592, 512, 596, 514, 596, 518, 590, 514, 1708, 544, 580, 522, 592, 516, 586, 524, 590, 516, 590, 518, 588, 524, 588, 514, 592, 520, 588, 520, 578, 
532, 582, 526, 594, 514, 594, 512, 592, 516, 596, 520, 584, 520, 582, 526, 582, 530, 586, 524, 590, 512, 596, 516, 586, 524, 594, 516, 1720, 510, 610, 512, 596, 510, 600, 512, 596, 516, 592, 516, 594, 508, 600, 510, 598, 516, 592, 
514, 596, 514, 596, 508, 602, 508, 600, 514, 594, 514, 596, 512, 594, 512, 598, 510, 598, 508, 602, 512, 1730, 498, 614, 504, 1740, 502, 608, 516, 594, 512, 1730, 506, 1740, 512};
// Código RAW (25°)
uint16_t rawData25[211] = {9044, 4536, 508, 1736, 516, 1730, 512, 616, 494, 618, 488, 618, 498, 606, 498, 1734, 508, 1738, 516, 1728, 520, 1728, 512, 1732, 514, 1732, 514, 606, 510, 606, 500, 606, 500, 1732, 512, 604, 506, 610, 498, 
606, 502, 606, 502, 612, 498, 1730, 518, 1726, 522, 1724, 524, 596, 514, 604, 500, 606, 504, 606, 500, 608, 502, 608, 498, 610, 498, 614, 494, 616, 494, 612, 502, 606, 498, 596, 524, 586, 524, 1722, 520, 1728, 520, 588, 516, 600, 
506, 600, 510, 592, 508, 600, 514, 598, 508, 598, 514, 594, 512, 600, 516, 588, 516, 594, 514, 592, 518, 592, 512, 596, 514, 1734, 514, 592, 516, 592, 518, 588, 516, 594, 516, 594, 518, 590, 518, 586, 522, 586, 522, 588, 520, 584, 
520, 590, 524, 570, 532, 584, 528, 576, 528, 588, 522, 582, 526, 586, 528, 570, 542, 572, 538, 570, 530, 582, 528, 572, 540, 574, 532, 1708, 540, 572, 534, 580, 532, 572, 536, 574, 532, 582, 532, 568, 538, 572, 538, 570, 534, 576, 
532, 582, 528, 574, 538, 576, 530, 578, 532, 574, 536, 574, 534, 578, 536, 570, 534, 576, 540, 568, 538, 1706, 536, 582, 524, 576, 534, 1714, 534, 576, 538, 1706, 538, 1708, 536};
// Código RAW (26°)
uint16_t rawData26[211] = {9092, 4464, 578, 1668, 568, 1678, 556, 554, 544, 568, 534, 578, 528, 580, 532, 1724, 516, 1732, 516, 1718, 526, 1740, 506, 1728, 526, 574, 532, 1724, 520, 586, 530, 572, 534, 1712, 540, 576, 530, 574, 538, 
570, 530, 578, 536, 574, 536, 1710, 536, 1710, 534, 1712, 534, 572, 534, 576, 534, 576, 538, 582, 520, 580, 530, 578, 538, 574, 522, 596, 522, 574, 534, 588, 520, 580, 530, 586, 516, 598, 514, 1728, 520, 1724, 524, 582, 520, 594, 
518, 582, 520, 588, 526, 578, 530, 578, 530, 580, 528, 580, 530, 580, 530, 582, 534, 574, 532, 574, 532, 578, 534, 584, 522, 1716, 530, 574, 534, 584, 524, 582, 526, 578, 530, 586, 522, 586, 524, 580, 528, 576, 536, 574, 530, 578, 
534, 572, 534, 574, 536, 574, 532, 582, 528, 578, 532, 574, 534, 574, 536, 572, 534, 580, 526, 580, 532, 574, 538, 572, 534, 574, 534, 1716, 532, 574, 532, 576, 534, 574, 536, 574, 532, 578, 530, 578, 532, 578, 538, 570, 538, 570, 
530, 578, 534, 574, 534, 576, 530, 578, 528, 580, 540, 570, 534, 576, 528, 580, 532, 578, 534, 574, 538, 1706, 534, 574, 538, 1710, 536, 1704, 544, 566, 532, 1718, 528, 1712, 536};
// Código RAW (27°)
uint16_t rawData27[211] = {9094, 4466, 578, 1668, 558, 1690, 556, 554, 540, 570, 530, 582, 526, 580, 530, 1718, 526, 1724, 518, 1728, 526, 1714, 526, 1732, 514, 1722, 526, 1716, 532, 578, 526, 594, 522, 1716, 530, 580, 532, 574, 528, 
588, 524, 578, 534, 574, 532, 1714, 534, 1718, 526, 1714, 530, 578, 534, 576, 534, 574, 534, 574, 538, 572, 534, 574, 534, 576, 534, 578, 528, 578, 536, 574, 538, 574, 530, 576, 536, 572, 534, 1716, 530, 1710, 538, 574, 536, 572, 
532, 576, 538, 578, 530, 572, 534, 576, 528, 580, 536, 576, 530, 578, 532, 578, 534, 572, 532, 580, 534, 574, 534, 576, 534, 1710, 534, 578, 532, 578, 526, 586, 526, 578, 530, 580, 526, 582, 524, 582, 528, 586, 522, 582, 534, 582, 
520, 584, 526, 578, 532, 576, 528, 588, 518, 592, 522, 586, 522, 582, 526, 590, 516, 586, 526, 596, 510, 594, 520, 574, 534, 576, 530, 1728, 512, 596, 520, 588, 520, 586, 522, 580, 522, 600, 514, 588, 518, 590, 524, 584, 526, 584, 
522, 586, 516, 588, 520, 598, 516, 596, 520, 574, 538, 572, 534, 584, 522, 576, 534, 576, 540, 576, 530, 1720, 520, 586, 528, 588, 514, 596, 516, 1726, 522, 1726, 518, 1734, 514};
// Código RAW (28°)
uint16_t rawData28[211] = {9040, 4520, 534, 1712, 524, 1720, 532, 580, 530, 582, 524, 580, 530, 580, 534, 1722, 518, 1730, 520, 1718, 520, 1730, 516, 1728, 524, 582, 522, 590, 520, 1734, 512, 582, 532, 1718, 520, 596, 516, 588, 524, 
590, 518, 586, 522, 580, 522, 1744, 508, 1728, 520, 1728, 516, 588, 516, 592, 522, 576, 530, 586, 526, 578, 528, 584, 526, 584, 522, 586, 530, 578, 532, 574, 534, 582, 526, 586, 522, 586, 522, 1714, 530, 1734, 506, 590, 526, 580, 
528, 580, 532, 576, 532, 580, 528, 578, 526, 590, 522, 586, 532, 572, 536, 578, 530, 574, 534, 586, 524, 574, 538, 582, 520, 1714, 534, 586, 524, 580, 534, 574, 530, 582, 528, 574, 532, 578, 530, 580, 532, 578, 526, 580, 536, 574, 
536, 574, 528, 578, 530, 578, 532, 588, 524, 574, 534, 572, 536, 572, 536, 574, 536, 574, 536, 572, 536, 574, 534, 576, 534, 578, 534, 1718, 526, 574, 532, 576, 534, 578, 534, 582, 520, 582, 530, 590, 518, 582, 524, 582, 532, 584, 
528, 578, 532, 578, 528, 578, 534, 580, 530, 576, 536, 572, 538, 574, 530, 588, 526, 582, 514, 596, 514, 1734, 516, 586, 522, 1732, 516, 578, 530, 1722, 526, 1724, 520, 1720, 522};
// Código RAW (29°)
uint16_t rawData29[211] = {9036, 4542, 510, 1734, 506, 1740, 506, 604, 506, 602, 506, 602, 510, 598, 510, 1736, 506, 1740, 508, 1740, 508, 1736, 504, 1740, 508, 1738, 512, 596, 506, 1742, 508, 600, 516, 1728, 514, 596, 516, 596, 
506, 596, 516, 596, 510, 600, 512, 1736, 506, 1738, 512, 1732, 510, 602, 512, 594, 512, 596, 516, 594, 512, 598, 512, 598, 516, 588, 518, 598, 506, 600, 510, 604, 510, 596, 512, 598, 508, 600, 508, 1738, 516, 1728, 516, 592, 508, 
602, 512, 596, 516, 594, 512, 598, 516, 592, 514, 596, 510, 598, 512, 600, 510, 596, 510, 600, 512, 602, 504, 600, 516, 594, 514, 1732, 510, 598, 512, 598, 506, 602, 508, 600, 514, 592, 518, 590, 512, 598, 514, 596, 514, 594, 514, 
596, 514, 596, 514, 596, 506, 604, 510, 600, 516, 592, 510, 598, 510, 602, 508, 598, 510, 598, 508, 604, 510, 600, 508, 598, 516, 594, 506, 1738, 506, 602, 512, 598, 508, 600, 512, 596, 512, 596, 506, 604, 510, 600, 508, 600, 512, 
596, 508, 600, 510, 600, 506, 602, 510, 600, 512, 596, 512, 596, 512, 594, 510, 602, 506, 604, 510, 598, 516, 1730, 512, 596, 510, 598, 510, 1736, 512, 1734, 514, 1732, 506, 1740, 508};
// Código RAW (30°)
uint16_t rawData30[211] = {9018, 4550, 504, 1740, 504, 1742, 506, 602, 504, 604, 510, 600, 502, 606, 508, 1740, 500, 1746, 504, 1742, 506, 1740, 506, 1740, 512, 594, 510, 1738, 510, 1734, 510, 598, 508, 1740, 508, 600, 514, 596, 514, 
596, 508, 600, 510, 598, 510, 1736, 514, 1730, 508, 1738, 512, 596, 508, 600, 514, 596, 512, 598, 510, 598, 506, 602, 518, 592, 512, 594, 510, 600, 504, 602, 510, 600, 512, 596, 510, 600, 516, 1728, 508, 1738, 512, 598, 508, 600, 
508, 602, 512, 596, 512, 596, 512, 600, 506, 600, 518, 592, 516, 592, 508, 600, 508, 600, 510, 598, 514, 596, 516, 594, 514, 1734, 504, 602, 506, 604, 512, 598, 504, 604, 506, 602, 506, 604, 502, 606, 506, 604, 508, 600, 506, 604, 
504, 606, 504, 604, 512, 598, 502, 606, 508, 602, 508, 600, 512, 602, 498, 606, 510, 598, 514, 594, 504, 604, 510, 600, 510, 600, 508, 1738, 508, 600, 508, 600, 508, 604, 504, 604, 506, 604, 506, 602, 506, 604, 512, 596, 510, 600, 
504, 604, 506, 602, 510, 598, 508, 602, 506, 602, 504, 610, 508, 598, 508, 600, 504, 604, 512, 596, 512, 1744, 498, 602, 514, 1738, 502, 1756, 488, 1748, 494, 1744, 504, 1750, 492};
// Código RAW (31°)
uint16_t rawData31[211] = {9020, 4560, 490, 1748, 504, 1748, 490, 618, 492, 610, 496, 608, 504, 606, 506, 1748, 496, 1750, 490, 1756, 492, 1764, 484, 1752, 492, 1756, 490, 1762, 488, 1746, 502, 604, 506, 1754, 486, 608, 506, 600, 506, 
606, 504, 604, 502, 606, 508, 1738, 510, 1748, 496, 1750, 494, 608, 506, 600, 510, 600, 506, 610, 498, 602, 506, 604, 512, 596, 508, 602, 506, 606, 500, 606, 506, 618, 492, 602, 512, 596, 504, 1754, 494, 1756, 492, 606, 508, 598, 
506, 610, 502, 602, 502, 616, 498, 608, 496, 604, 504, 604, 504, 604, 510, 606, 496, 608, 506, 612, 492, 612, 502, 604, 510, 1750, 486, 622, 494, 610, 490, 620, 498, 608, 496, 610, 504, 614, 494, 608, 494, 618, 494, 616, 492, 610, 
502, 604, 498, 616, 496, 620, 488, 618, 496, 606, 506, 602, 506, 608, 500, 610, 500, 612, 490, 616, 498, 606, 502, 610, 498, 614, 494, 1752, 492, 610, 498, 610, 500, 604, 498, 612, 500, 604, 500, 612, 508, 596, 508, 600, 508, 608, 
502, 600, 510, 602, 502, 602, 512, 600, 506, 602, 502, 604, 512, 602, 506, 606, 498, 602, 506, 602, 512, 1750, 492, 606, 504, 602, 508, 600, 504, 600, 510, 600, 512, 604, 508};
// Código RAW (32°)
uint16_t rawData32[211] = {9024, 4546, 508, 1750, 484, 1758, 492, 610, 502, 608, 496, 612, 506, 614, 494, 1748, 496, 1744, 498, 1760, 484, 1760, 486, 1762, 488, 608, 502, 608, 498, 614, 498, 1746, 496, 1756, 490, 604, 508, 602, 508, 
600, 508, 602, 504, 604, 506, 1754, 490, 1754, 492, 1754, 490, 610, 504, 604, 506, 600, 512, 594, 506, 606, 502, 604, 504, 606, 508, 600, 504, 604, 512, 596, 502, 606, 510, 600, 512, 598, 504, 1742, 504, 1758, 486, 616, 498, 602, 
506, 602, 506, 600, 504, 606, 502, 606, 506, 606, 504, 602, 510, 596, 504, 606, 504, 604, 504, 606, 506, 602, 504, 604, 504, 1756, 486, 614, 502, 604, 504, 604, 504, 602, 514, 598, 502, 606, 504, 614, 494, 610, 498, 612, 502, 602, 
506, 604, 508, 596, 504, 608, 506, 610, 500, 608, 498, 602, 504, 604, 512, 600, 504, 602, 500, 620, 494, 610, 496, 608, 504, 602, 502, 1760, 484, 614, 502, 604, 502, 608, 500, 620, 490, 618, 486, 626, 486, 608, 500, 618, 496, 606, 
502, 610, 494, 622, 486, 616, 492, 620, 492, 606, 508, 606, 494, 612, 502, 616, 490, 618, 490, 608, 502, 1760, 488, 606, 498, 1760, 494, 602, 504, 602, 504, 616, 498, 614, 492};

// Task Handle da função dos sensores. Faz a função sensores funcionar simultaneamente em outra parte do Core do ESP32.
TaskHandle_t Sensor_Task;       

// This function creates the timer object. It's part of Blynk library 
BlynkTimer timer;

void myTimer() 
{
  // This function describes what will happen with each timer tick
  // timer é criado para impedir que aconteça uma sobrecarga no servidor
  Blynk.virtualWrite(V0,Temp);      // variável Temp é alocada na variável virtual V0
  Blynk.virtualWrite(V1,Humidity);  // variável Humidity é alocada na variável virtual V1
  Blynk.virtualWrite(V4,HITemp);    // variável HITemp é alocada na variável virtual V4
  Blynk.virtualWrite(V3,NumeroPessoas);
}

BLYNK_WRITE(V2) // Executa quando a variável virtual V2 mudar de estado no aplicativo (Botão liagr/desliar).
{
  if(param.asInt() == 1) // Caso V2 mude para 1
  {
    if(estado == false)
    {
      TempAC = 18;
      Serial.println("Ligando");
      irsend.sendRaw(rawData, 221, 38);  // Envia dados RAW, relacionados a ligar o ar condicionado(capture at 38kHz).
      estado = 1;                        // Define o estado do ar para true(liagdo).
      delay(500);
    }
    else
    {
      Serial.println("Desligando");
      irsend.sendRaw(rawData0, 221, 38);  // Envia dados RAW, relacionados a desligar o ar condicionado(capture at 38kHz).
      estado = 0;                         // Define o estado do ar para false (desligado).
      delay(500);
    }
  }
}

BLYNK_WRITE(V5) // Executa quando a variável virtual 5 mudar de estado no aplicativo (Botão diminuir temperatura).
{
  if(param.asInt() == 1)
  {
    TempAC --;
    controle();
    Serial.println(TempAC);
  }
}

BLYNK_WRITE(V6) // Executa quando a variável virtual 5 mudar de estado no aplicativo (Botão aumentar temperatura).
{
  if(param.asInt() == 1)
  {
    TempAC ++;
    controle();
    Serial.println(TempAC);
  }
}

//Função que contem todos as possiveis temperaturas de envio para os dados RAW
void controle()
{
  switch (TempAC)
  {
    case 16:  // Caso que TempAC for 16°, envia o dado RAW relacionado a 16°
      irsend.sendRaw(rawData16, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 17:
      irsend.sendRaw(rawData17, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 18:
      irsend.sendRaw(rawData18, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 19:
      irsend.sendRaw(rawData19, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 20:
      irsend.sendRaw(rawData20, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 21:
      irsend.sendRaw(rawData21, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 22:
      irsend.sendRaw(rawData22, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 23:
      irsend.sendRaw(rawData23, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 24:
      irsend.sendRaw(rawData24, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 25:
      irsend.sendRaw(rawData25, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 26:
      irsend.sendRaw(rawData26, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 27:
      irsend.sendRaw(rawData27, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 28:
      irsend.sendRaw(rawData28, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 29:
      irsend.sendRaw(rawData29, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 30:
      irsend.sendRaw(rawData30, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 31:
      irsend.sendRaw(rawData31, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    case 32:
      irsend.sendRaw(rawData32, 221, 38);  // Send a raw data capture at 38kHz.
    break;

    default:
      if (TempAC > 32) // Limite superior do ar condicionado é 32°
        TempAC = 32;
      if (TempAC < 16) // Limite inferior do ar condicionado é 16°
        TempAC = 16;
  }
}

void setup() 
{
  Serial.begin(115200);                                 // Inicializa monitor serial
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);  // Inicializa o Blynk
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);            // Inicializa Display OLED com endereço I2C 0x3C
  display.clearDisplay();
  display.setTextColor(WHITE);
  dht.begin();                                  // Inicializa o DHT111
  irsend.begin();                               // Inicializa o envio por infravermelho
  timer.setInterval(1000L, myTimer);            // Inicializa o Timer do blynk para enviar os dados para o APP a cada 1s
  
  pinMode(TCRT1_PIN, INPUT);    //definindo o pino do sensor como entrada
  pinMode(TCRT2_PIN, INPUT);    //definindo o pino do sensor como entrada

  // Criação da Task onde acontece a captura de dados do DHT11, funcionando em um core separado de forma simultanea
  xTaskCreatePinnedToCore(
      SensoresTask,       /* Function to implement the task */
      "Task2",            /* Name of the task */
      10000,              /* Stack size in words */
      NULL,               /* Task input parameter */
      0,                  /* Priority of the task */
      &Sensor_Task,       /* Task handle. */
      1);                 /* Core where the task should run */
  delay(1000);
}

// Task que verifica a captura de valores do DHT, e conta a entrada e saída de pessoas.
void SensoresTask(void * parameter) 
{
  for(;;) 
  {
    // Captura valor da temperatura e Humidade
    Humidity = dht.readHumidity();                        // captura a humidade do ar
    Temp = dht.readTemperature();                         // captura temperatura em °C
    HITemp = dht.computeHeatIndex(Temp, Humidity, false); // calcula o indice de calor em celcius

    // verifica se o sensor 1 está coberto
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
  // Runs BlynkTimer
  timer.run(); 

  // Exibe os valores no Display OLED.
  display.clearDisplay();               // clear display
  // exibir sensação térmica
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
  // exibir número de pessoas
  display.setTextSize(1);               // set text size
  display.setCursor(0,32);              // set position to display
  display.print("Numero de pessoas:");  // set text
  display.setTextSize(2);
  display.setCursor(0,42);
  display.print(NumeroPessoas);
  display.display();                    // display on OLED
  delay(500);
}
