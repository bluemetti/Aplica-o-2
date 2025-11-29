#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

// ---------------------------------------------------------------------------
// 1. CREDENCIAIS WI-FI (ATUALIZADO)
// ---------------------------------------------------------------------------
const char SSID[] = "MAIA";         // Novo nome da rede
const char PASS[] = "1234567890";   // Nova senha

// ---------------------------------------------------------------------------
// 2. CREDENCIAIS DO DISPOSITIVO (Seus últimos dados)
// ---------------------------------------------------------------------------
const char DEVICE_LOGIN_NAME[]  = "f6a5d396-cd26-4e80-8d94-02d913ae056e";
const char DEVICE_KEY[]         = "hwU2wCeT?DT#JlqedohAFyZM0";

// ---------------------------------------------------------------------------
// 3. VARIÁVEIS DA NUVEM
// ---------------------------------------------------------------------------
void onSistemaAtivoChange();
void onCoresChange();

bool sistema_ativo;
float temperatura;
float umidade;
int luminosidade;
int cores;

// ---------------------------------------------------------------------------
// 4. INICIALIZAÇÃO
// ---------------------------------------------------------------------------
void initProperties(){
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(sistema_ativo, READWRITE, ON_CHANGE, onSistemaAtivoChange);
  ArduinoCloud.addProperty(temperatura, READ, ON_CHANGE);
  ArduinoCloud.addProperty(umidade, READ, ON_CHANGE);
  ArduinoCloud.addProperty(luminosidade, READ, ON_CHANGE);
  ArduinoCloud.addProperty(cores, READWRITE, ON_CHANGE, onCoresChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);