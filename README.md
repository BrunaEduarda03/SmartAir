# EconoWize :iphone:
Seu assistente para climatização personalizada.

## :speech_balloon: Sobre  
O SmartAir é um sistema IoT de gerenciamento de ambientes, que permite controlar e monitorar aparelhos de ar-condicionado de forma remota, otimozando o consumo de energia. Com o auxílio de um aplicativo, é possível visualizar os dados em tempo real, além de propocionar conforto e praticidade aos usuários.

## :memo: Objetivos Especificos 
* Controlar as funcionalidades do ar-condicionado, tais como, ligar/desligar, aumentar/diminuir temperatura;
* Realizar controle manual através de dispositivo instalado no ambiente ou via servidor web através de rede Wifi;
* Compartilhar informações de Status do ar-condicionado via servidor web;
* Otimizar o consumo de energia elétrica dos aparelhos de ar-condicionado, visando reduzir os custos e diminuir o impacto ambiental.

## :card_index_dividers: Materiais utilizados 

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


## 📱 Desenvolvimento do app

O aplicativo mobile do SmartAir foi totalmente desenvolvido em React Native, garantindo performance, fluidez e compatibilidade com dispositivos Android e iOS.
A interface foi planejada para ser intuitiva, moderna e prática, permitindo que o usuário controle o ambiente com poucos toques.

🧰 Tecnologias utilizadas no aplicativo

React Native

CLI

TypeScript

React Navigation

Axios

Context API / Async Storage para gerenciamento de estado e persistência

API REST integrada ao ESP32 para comandos e monitoramento em tempo real



## 🛠️ Como instalar

**Clonar este repositório**

```
git clone git@github.com:BrunaEduarda03/SmartAir.git
```


* Baixe e instale o Esp32 no ambiente de desenvolvimento do Arduino ou baixe o VsCode e instale a extensão plataformIO.
* Instale o nodeMCU com a IDE e Instale as bibliotecas da pasta lib no seu arquivo local de bibliotecas.

Insira sua rede wifi no código
```
#define SSID   nome_do_wifi
#define PASSWD senha_do_wifi 
```

## 🗃️ Arquitetura   

### Esquemático :bar_chart:
<h1>
<img alt="smartAir" src="https://github.com/BrunaEduarda03/SmartAir/assets/73250271/f12555ca-d0a7-4aa1-9da5-a482a096b312" width="800px" align="center" />
</h1> 

### Montagem do Circuito :zap:
<h1>
<img alt="smartAir" src="https://github.com/BrunaEduarda03/SmartAir/assets/73250271/0aac75bf-3616-47ba-ab8a-0e5eb851a117" width="800px" align="center" />
</h1> 


### 📶 Inteface Gráfica 
Aplicativo :iphone:

SplashScreen

![image](https://github.com/user-attachments/assets/040a37f4-e112-4cd7-94d9-093a08eef0f3)

Onboarding

![image](https://github.com/user-attachments/assets/0d0b2694-d7eb-4b0a-8b59-c572ddd396ce)

Auntenticação

![image](https://github.com/user-attachments/assets/c56cf4cc-9721-4725-be93-560aab5d7e23)

Home

![image](https://github.com/user-attachments/assets/0d754d15-4b6f-4461-88a0-a0b77729786c)

Dispositivo: Ar-condicionado

![image](https://github.com/user-attachments/assets/a5681bfa-a7d2-4cf8-8dfa-31636fbc9cda)

Menu

![image](https://github.com/user-attachments/assets/a553c2ec-7f6e-4422-9666-e11f63f6e5fd)

Configurações de Rede

![image](https://github.com/user-attachments/assets/f6ac5ea6-6ead-4100-9599-469b2175ec2a)



## :technologist: Demo 

https://github.com/BrunaEduarda03/SmartAir/assets/73250271/d5100913-022e-424e-9b7b-e9a12c8bc001

## :speaking_head: Nos dê feedbacks sobre o projeto!
E-mail: [**brunaduda37@gmail.com**](mailto:brunaduda37@gmail.com)

Linkedin: [Bruna Eduarda](https://www.linkedin.com/in/bruna-eduarda-a06a1b18b/)


