#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verde
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

const int buttonPin = 18;  // O número do pino do botão
int buttonState = 0;  // Variável para ler o status do botão

const int ldrPin = 4;  // Define o pino para controle do sensor LDR
int threshold=600; // Limiar para luz baixa

unsigned long ultimoMs = 0;  // Variável para controlar o tempo de pisca do LED
const long intervalo = 1000;  // Intervalo de 1 segundo para piscar o LED
bool estadoLed = LOW; // Variável para alternar o estado do LED

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo,OUTPUT);
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);

  // Inicialização das entradas
  pinMode(buttonPin, INPUT); // Inicia o pin do botão como entrada

  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  // Verifica estado do botão
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode > 0) {
      Serial.print("Código de resposta HTTP: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Código de Erro: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else {
    Serial.println("WiFi desconectado");
  }
}


void loop() {
  int ldrstatus=analogRead(ldrPin);
  int valorLuz = analogRead(ldrPin);
  Serial.print("Luminosidade: ");
  Serial.println(valorLuz);

  // Verifica se a luminosidade está abaixo do limiar (escuro)
  if (lightValue <= threshold) {
    // Modo noturno: piscar o LED amarelo a cada segundo
    unsigned long miliAtual = millis();
    
    if (msAtual - ultimoMs >= intervalo) {
      // Salvar o último tempo que o LED foi alternado
      ultimoMs = msAtual;

      // Alternar o estado do LED
      estadoLed = !estadoLed;
      digitalWrite(led_amarelo, estadoLed);
    }
  } else {
    // Quando está claro, desligar o LED amarelo
    digitalWrite(led_amarelo, LOW);
  }

  delay(100); // Pequeno atraso para estabilidade
}


    