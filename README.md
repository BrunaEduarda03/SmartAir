# SmartAir :iphone:

## Sobre  :speech_balloon:
O SmartAir é um sistema IoT de gerenciamento de ambientes, que permite controlar e monitorar aparelhos de ar-condicionado de forma remota, otimozando o consumo de energia. Com o auxílio de um aplicativo, é possível visualizar os dados em tempo real, além de propocionar conforto e praticidade aos usuários.

## Objetivos Especificos :memo:
* Controlar as funcionalidades do ar-condicionado, tais como, ligar/desligar, aumentar/diminuir temperatura;
* Realizar controle manual através de dispositivo instalado no ambiente ou via servidor web através de rede Wifi;
* Compartilhar informações de Status do ar-condicionado via servidor web;
* Otimizar o consumo de energia elétrica dos aparelhos de ar-condicionado, visando reduzir os custos e diminuir o impacto ambiental.

## Materiais utilizados :card_index_dividers:

* Microcontrolador ESP32;
* Sensor de temperatura DHT11;
* Display LED SSD1306;
* Sensores TCRT5000
* Led emissor infravermelho 1838T;
* Led emissor infravermelho;
* Módulo de Fonte YwRobot MB102;
* Resistores;
* Jumpers;
* Protoboard;

## 🛠️ Como instalar

**Clonar este repositório**

```
git clone git@github.com:BrunaEduarda03/SmartAir.git
```


* Baixe e instale o Esp32 no ambiente de desenvolvimento do Arduino ou baixe o VsCode e instale a extensão plataformIO.
* o Esp32Instale as bibliotecas da pasta lib no seu arquivo local de bibliotecas

Insira sua rede wifi no código
```
#define SSID   nome_do_wifi
#define PASSWD senha_do_wifi 
```

## Arquitetura

### Esquemático :bar_chart:
<h1>
<img alt="smartAir" src="https://github.com/BrunaEduarda03/SmartAir/assets/73250271/f12555ca-d0a7-4aa1-9da5-a482a096b312" width="800px" align="center" />
</h1> 

### Montagem do Circuito :zap:
<h1>
<img alt="smartAir" src="https://github.com/BrunaEduarda03/SmartAir/assets/73250271/8c0a19e1-ce4e-4180-b771-27e887fce724" width="800px" align="center" />
</h1> 

### Inteface Gráfica - Blynk
#### Aplicativo :iphone:
<h1>
<img alt="smartAir" src="https://github.com/BrunaEduarda03/SmartAir/assets/73250271/57144797-1da3-47ce-82af-8434ccef1940" width="800px" align="center" />
</h1> 
#### Navegador :technologist:
<h1>
<img alt="smartAir" src="https://github.com/BrunaEduarda03/SmartAir/assets/73250271/cacc5cd9-40f2-448c-a1cd-127545383daf" width="800px" align="center" />
</h1> 

## Demo


