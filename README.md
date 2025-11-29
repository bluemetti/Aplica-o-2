# 💡 Lâmpada Inteligente IoT (Smart Lamp)

Este projeto consiste em um sistema de iluminação inteligente baseado no microcontrolador **ESP32 Dev Kit V1**, integrado à plataforma **Arduino IoT Cloud**. O sistema permite o controle híbrido (físico e remoto), monitoramento ambiental e possui sistemas de segurança autônomos.

Desenvolvido como parte da avaliação da disciplina de **Práticas Integradas: Camada de Aplicação**.

---

## 📋 Funcionalidades

### 🎮 Controle e Interação
* **Controle Híbrido de Cor:** A cor do LED RGB pode ser alterada tanto pelo **Slider no Dashboard** quanto pelo **Potenciômetro físico**. O sistema detecta automaticamente qual foi acionado por último.
* **Interruptor Mestre:** Botão físico e virtual para ligar/desligar todo o sistema.
* **Dashboard Web/Mobile:** Interface gráfica para controle remoto via Arduino IoT Cloud Remote.

### 🛡️ Segurança e Automação
* **Proteção contra Superaquecimento:** Se a temperatura ultrapassar **31°C**, o sistema desliga o LED automaticamente e aciona um **Buzzer** de alerta.
* **Modo Noturno (LDR):** O sistema monitora a luminosidade do ambiente. Se estiver muito claro/escuro (configurável), o LED pode ser bloqueado para economizar energia.
* **Sincronização NTP:** Ajuste automático de data e hora via internet para garantir a segurança da conexão SSL (resolução do Error 5).

---

## 🛠️ Hardware e Pinagem

O projeto utiliza o **ESP32 DOIT DEVKIT V1**. Abaixo está o mapa de conexões:

| Componente | Tipo | Pino ESP32 (GPIO) | Observação |
| :--- | :--- | :--- | :--- |
| **Potenciômetro** | Entrada Analógica | **GPIO 34** | ADC1 (Wi-Fi Safe) |
| **Sensor LDR** | Entrada Analógica | **GPIO 35** | ADC1 + Divisor de Tensão |
| **Sensor DHT11** | Entrada Digital | **GPIO 13** | Dados |
| **Botão (Push Button)** | Entrada Digital | **GPIO 27** | Configurado como INPUT_PULLUP |
| **Buzzer** | Saída Digital | **GPIO 26** | Ativo/Passivo |
| **LED Vermelho** | Saída PWM | **GPIO 18** | Com resistor 220Ω |
| **LED Verde** | Saída PWM | **GPIO 19** | Com resistor 220Ω |
| **LED Azul** | Saída PWM | **GPIO 21** | Com resistor 220Ω |

---

## ☁️ Configuração do Arduino IoT Cloud

Para replicar este projeto, crie uma "Thing" no Arduino Cloud com as seguintes variáveis:

| Nome da Variável | Tipo (Type) | Permissão | Função |
| :--- | :--- | :--- | :--- |
| `sistema_ativo` | **CloudSwitch** (bool) | Read & Write | Liga/Desliga o sistema |
| `cores` | **Integer** (int) | Read & Write | Slider de 0 a 4095 (Cor) |
| `temperatura` | **CloudTemperature** | Read Only | Exibe temperatura do DHT11 |
| `umidade` | **CloudRelativeHumidity** | Read Only | Exibe umidade do DHT11 |
| `luminosidade` | **Integer** (int) | Read Only | Valor cru do LDR (0-4095) |

---

## 🚀 Como Rodar o Projeto

### Pré-requisitos
* IDE do Arduino instalada.
* Bibliotecas instaladas: `ArduinoIoTCloud`, `DHT sensor library`, `Adafruit Unified Sensor`.
* Placa ESP32 configurada na IDE (`DOIT ESP32 DEVKIT V1`).

### Passo a Passo
1.  Clone este repositório.
2.  Abra o arquivo `.ino` na Arduino IDE.
3.  Abra a aba `thingProperties.h` e insira suas credenciais:
    * **SSID:** Nome da sua rede Wi-Fi.
    * **PASS:** Senha da rede.
    * **Device ID & Secret Key:** Obtenha na aba "Devices" do Arduino Cloud.
4.  Conecte o ESP32 via USB.
5.  Selecione a placa **DOIT ESP32 DEVKIT V1** e a porta correta.
6.  Faça o Upload.

---

## 📸 Demonstração

### Circuito Físico
> *[Insira aqui uma foto da sua protoboard montada]*

### Dashboard Online
> *[Insira aqui um print da tela do seu celular ou PC com o painel]*

---

## 🐛 Solução de Problemas Comuns

* **Error 5 (ArduinoIoTCloudTCP):** Ocorre se o relógio do ESP32 não estiver sincronizado. Este código possui uma correção via NTP (`configTime`) no `setup()` que resolve isso automaticamente.
* **Brownout Detector:** Se a placa reiniciar ao ligar o Wi-Fi, troque o cabo USB ou a porta de alimentação. O código possui `WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);` para mitigar isso.

---

**Autor:** Davi Blumetti, Arthur Adriando, Filipe Alves, João Maia.
**Instituição:** SENAI CIMATEC