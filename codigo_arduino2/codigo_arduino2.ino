// =============================================================
// PROJETO COMPLETO: GARAGEM AUTOMÁTICA
// RFID + BOTÕES + SERVO LENTO + SENSOR DE FOGO + SENSOR DE CHUVA
// =============================================================

#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>

// =============================================================
// Servo motor DA GARAGEM
// =============================================================
Servo SERVO_GARAGE;

const int botaoAbrir  = 12;
const int botaoFechar = 13;

int leituraAbrirAtual,  leituraAbrirAnterior  = HIGH;
int leituraFecharAtual, leituraFecharAnterior = HIGH;

int anguloAtual   = 0;
int anguloDestino = 0;

// =============================================================
// RFID
// =============================================================
#define PINO_RST 9
#define PINO_SDA 10

#define LED_RED     2
#define LED_GREEN   3
#define BUZZER_DOOR 4

MFRC522 rfid(PINO_SDA, PINO_RST);

String cartao_rf   = " 93 d3 75 e4";
String chaveiro_rf = " 07 e3 41 66";

int FREQUENCIA = 3300;

// =============================================================
// SENSOR DE FOGO
// =============================================================
#define sensor_fogo 8
#define buzzer_fogo 7
#define freq_fogo 960

// =============================================================
// SENSOR DE CHUVA + SERVO DA JANELA
// =============================================================
#define pino_sensor_chuva A1   // Porta analógica
#define servo_janela 6         

Servo MICRO_JANELA;
bool is_raining = false;

// =============================================================
// SETUP
// =============================================================
void setup() {

  Serial.begin(9600);

  // ---- SERVO E BOTÕES DA GARAGEM ----
  SERVO_GARAGE.attach(5);
  SERVO_GARAGE.write(0);

  pinMode(botaoAbrir, INPUT_PULLUP);
  pinMode(botaoFechar, INPUT_PULLUP);

  // ---- RFID ----
  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_DOOR, OUTPUT);

  digitalWrite(LED_RED, HIGH);

  // ---- SENSOR DE FOGO ----
  pinMode(sensor_fogo, INPUT);
  pinMode(buzzer_fogo, OUTPUT);

  // ---- SENSOR DE CHUVA E SERVO DA JANELA ----
  pinMode(pino_sensor_chuva, INPUT);
  MICRO_JANELA.attach(servo_janela);
  MICRO_JANELA.write(0);

  Serial.println("Sistema iniciado. Aproxime a TAG.");
}

// =============================================================
// LOOP PRINCIPAL
// =============================================================
void loop() {

  // ===========================================================
  // 1) SENSOR DE FOGO (emergência)
  // ===========================================================
  if (digitalRead(sensor_fogo) == HIGH) {
    alarmeIncendio();
  }

  // ===========================================================
  // 2) SENSOR DE CHUVA → ABRE/FECHA JANELA
  // ===========================================================
  is_raining = digitalRead(pino_sensor_chuva);

  if (is_raining) {
    servoJanela.write(180);   // Fecha janela quando chove
  } else {
    servoJanela.write(0);     // Abre janela quando não chove
  }

  // ===========================================================
  // 3) BOTÕES DA GARAGEM
  // ===========================================================
  leituraAbrirAtual  = digitalRead(botaoAbrir);
  leituraFecharAtual = digitalRead(botaoFechar);

  if (leituraAbrirAtual == LOW && leituraAbrirAnterior == HIGH) {
    anguloDestino = 90;
    delay(200);
  }

  if (leituraFecharAtual == LOW && leituraFecharAnterior == HIGH) {
    anguloDestino = 0;
    delay(200);
  }

  leituraAbrirAnterior  = leituraAbrirAtual;
  leituraFecharAnterior = leituraFecharAtual;

  // ===========================================================
  // 4) RFID
  // ===========================================================
  String conteudo = "";

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

    for (byte i = 0; i < rfid.uid.size; i++) {
      conteudo.concat(String(rfid.uid.uidByte[i] < 16 ? " 0" : " "));
      conteudo.concat(String(rfid.uid.uidByte[i], HEX));
    }

    if (conteudo == chaveiro_rf || conteudo == cartao_rf) {
      Serial.println("Acesso Liberado!");
      efeitoAcesso();
      MICRO_DOOR(0);
    }
    else {
      Serial.println("TAG desconhecida:");
      Serial.println(conteudo);
      efeitoErro();
    }
  }

  // ===========================================================
  // 5) MOVIMENTO LENTO DO SERVO DA GARAGEM
  // ===========================================================
  moverServoLento();
}

// =============================================================
// FUNÇÕES DO SISTEMA
// =============================================================

void moverServoLento() {
  if (anguloAtual < anguloDestino) {
    anguloAtual++;
    MICRO_GARAGE.write(anguloAtual);
    delay(15);
  }
  else if (anguloAtual > anguloDestino) {
    anguloAtual--;
    MICRO_GARAGE.write(anguloAtual);
    delay(15);
  }
}

void abrirEFecharGarage() {
  anguloDestino = 90;
  delay(15000);
  anguloDestino = 0;
}

void efeitoAcesso() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);

  tone(BUZZER_DOOR, FREQUENCIA); delay(150);
  noTone(BUZZER_DOOR);           delay(150);
  tone(BUZZER_DOOR, FREQUENCIA); delay(150);
  noTone(BUZZER_DOOR);           delay(500);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
}

void efeitoErro() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_RED, LOW);
    tone(BUZZER_DOOR, FREQUENCIA);
    delay(100);
    digitalWrite(LED_RED, HIGH);
    noTone(BUZZER_DOOR);
    delay(100);
  }
}

void alarmeIncendio() {
  tone(buzzer_fogo, freq_fogo);
  delay(1500);
  noTone(buzzer_fogo);
  delay(1500);
}
