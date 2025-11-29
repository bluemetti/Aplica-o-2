/*
  PROJETO: Lâmpada Inteligente - ESP32 Dev Kit V1
  ESTRUTURA: Abas Separadas
*/

#include "thingProperties.h" // Puxa as senhas da outra aba
#include "DHT.h"
#include "time.h" 
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// --- HARDWARE ---
#define DHTTYPE DHT11
#define PotPin  34
#define LDRpin  35
#define Button  27
#define BuzzPin 26
#define DHTpin  13
#define LedR    18
#define LedG    19
#define LedB    21

const bool COMMON_ANODE = false;

// Variáveis Locais
bool LED_LOCK = false;
bool TEMP_ENABLED = true;
bool LDR_ENABLED = true;
int ultimoValorFisico = 0;
const int THRESHOLD = 100;
volatile unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 250;

DHT dht(DHTpin, DHTTYPE);

// --- FUNÇÕES AUXILIARES ---

void IRAM_ATTR rotina_botao() {
  unsigned long currentTime = millis();
  if ((currentTime - lastButtonPress) > debounceDelay) {
    sistema_ativo = !sistema_ativo; 
    lastButtonPress = currentTime;
  }
}

void setColor(int r, int g, int b) {
  analogWrite(LedR, COMMON_ANODE ? 255-r : r);
  analogWrite(LedG, COMMON_ANODE ? 255-g : g);
  analogWrite(LedB, COMMON_ANODE ? 255-b : b);
}

// Debug de Hora
void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("ERRO: Falha ao obter a hora.");
    return;
  }
  Serial.println(&timeinfo, "Hora Atual: %A, %B %d %Y %H:%M:%S");
}

// ---------------------------------------------------------------------------
// SETUP
// ---------------------------------------------------------------------------
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);
  delay(1500); 

  // Configura Pinos
  pinMode(BuzzPin, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(PotPin, INPUT);
  pinMode(LDRpin, INPUT);
  pinMode(Button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Button), rotina_botao, FALLING);
  
  dht.begin();
  ultimoValorFisico = analogRead(PotPin);

  // --- NTP (HORA CERTA) ---
  Serial.println("Configurando NTP...");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); 

  // --- INICIALIZAÇÃO DA NUVEM ---
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
  Serial.println("Conectando...");
}

// ---------------------------------------------------------------------------
// LOOP
// ---------------------------------------------------------------------------
void loop() {
  ArduinoCloud.update(); 
  
  // VERIFICAÇÃO DE CONEXÃO E HORA (CORRIGIDO PARA COMPILAR)
  static bool horaCerta = false;
  
  // A correção está aqui: Comparamos o estado corretamente
  if (ArduinoIoTPreferredConnection.check() == NetworkConnectionState::CONNECTED && !horaCerta) {
      struct tm timeinfo;
      if(getLocalTime(&timeinfo)){
         if(timeinfo.tm_year > (2023 - 1900)) { 
             horaCerta = true;
             Serial.println(">>> SUCESSO: Conectado e Hora Sincronizada! <<<");
             printLocalTime();
         }
      }
  }

  // --- LÓGICA DO HARDWARE ---
  float tempLida = dht.readTemperature(); 
  float humLida  = dht.readHumidity();
  int ldrLido    = analogRead(LDRpin);
  int leituraPot = analogRead(PotPin);

  if (abs(leituraPot - ultimoValorFisico) > THRESHOLD) {
    cores = leituraPot;           
    ultimoValorFisico = leituraPot; 
  }

  if (isnan(tempLida)) tempLida = 0.0;
  if (isnan(humLida))  humLida = 0.0;

  temperatura = tempLida;
  umidade = humLida;
  luminosidade = ldrLido;

  if (!sistema_ativo) {
    noTone(BuzzPin);
    setColor(0,0,0);
  } 
  else {
    // 1. Segurança
    if (TEMP_ENABLED && (temperatura < 0 || temperatura > 31)) { 
      LED_LOCK = true;
      setColor(0,0,0);     
      tone(BuzzPin, 500);  
    } 
    else {
      noTone(BuzzPin);
      // 2. Luz
      if (LDR_ENABLED) {
        if (luminosidade < 1500) { 
           LED_LOCK = true;
           setColor(0,0,0);
        } else {
           LED_LOCK = false;
        }
      }
      // 3. Cores
      if (!LED_LOCK) {
        if (cores < 682)       setColor(255, 0, 0);    
        else if (cores < 1365) setColor(255, 100, 0);  
        else if (cores < 2048) setColor(255, 255, 0);  
        else if (cores < 2731) setColor(255, 255, 255);
        else if (cores < 3072) setColor(0, 255, 255);  
        else                   setColor(0, 0, 255);    
      }
    }
  }
}

// --- CALLBACKS ---
void onCoresChange() { ultimoValorFisico = analogRead(PotPin); } 
void onSistemaAtivoChange()  { Serial.println(sistema_ativo); }