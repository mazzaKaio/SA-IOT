// ======================
// VERIFICAÇÃO RFID
// ======================

//CONEXÕES RFID: SDA - 10 | SCK - 13 | MOSI - 11 | MISO - 12 | RST - 9
#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

#define PINO_RST 9
#define PINO_SDA 10
#define BUZZER_DOOR 2
#define SERVO_DOOR 3

MFRC522 rfid(PINO_SDA, PINO_RST); // Objeto do RFID
Servo MICRO_DOOR; // Objeto do Servo responsável pela porta

// Variáveis usadas para verificação
String cartao_rf = " 53 51 08 e4";
String chaveiro_rf = " 07 e3 41 66";
int FREQUENCIA = 3300;

// ======================
// GARAGEM
// ======================
#define SERVO_GARAGE 4
#define botaoAbrir A1
#define botaoFechar A2

Servo MICRO_GARAGE;

int leituraAbrirAtual,  leituraAbrirAnterior  = HIGH;
int leituraFecharAtual, leituraFecharAnterior = HIGH;

int anguloAtual   = 0;
int anguloDestino = 0;

// ======================
// SENSOR DE FOGO
// ======================
#define sensor_fogo 5
#define buzzer_fogo 6
#define freq_fogo 960

// ======================
// JANELA
// ======================
#define SENSOR_CHUVA A3
#define SERVO_JANELA 7

Servo MICRO_JANELA;

// ======================
// VARAL
// ======================
#define POTEN_VARAL A4
#define SERVO_VARAL 8

Servo MICRO_VARAL;

int largura_v = 0;
int angulo_v = 0;
int leitura_v = 0;


void setup() {
  // GERAL
  Serial.begin(9600);

  // GARAGEM
  MICRO_GARAGE.attach(SERVO_GARAGE);
  MICRO_GARAGE.write(0);

  pinMode(botaoAbrir, INPUT_PULLUP);
  pinMode(botaoFechar, INPUT_PULLUP);

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  pinMode(BUZZER_DOOR, OUTPUT);

  MICRO_DOOR.attach(SERVO_DOOR);
  MICRO_DOOR.write(180);

  // SENSOR DE FOGO
  pinMode(sensor_fogo, INPUT);
  pinMode(buzzer_fogo, OUTPUT);

  // JANELA
  pinMode(SENSOR_CHUVA, INPUT);
  MICRO_JANELA.attach(SERVO_JANELA);
  MICRO_JANELA.write(180);

  // VARAL
  MICRO_VARAL.attach(SERVO_VARAL);
  MICRO_VARAL.write(0);

  // APÓS TUDO
  Serial.println("Sistema iniciado. Aproxime a TAG.");
}

void loop() {
  // ======================
  // 1. FOGO
  // ======================
  if (!digitalRead(sensor_fogo)) {
    alarmeIncendio();
    Serial.println("FOGO!");
  }

  // ======================
  // 2. CHUVA
  // ======================
  if (!digitalRead(SENSOR_CHUVA)) {
    MICRO_JANELA.write(0);   // Fecha janela quando chove
    MICRO_VARAL.write(0);
    Serial.println("SENSOR CHUVA --> Está chovendo!");
    delay(350);
  } else {
    MICRO_JANELA.write(180);     // Abre janela quando não chove
  }

  // ======================
  // 3. GARAGEM
  // ======================
  leituraAbrirAtual  = digitalRead(botaoAbrir);
  leituraFecharAtual = digitalRead(botaoFechar);

  if (leituraAbrirAtual == LOW && leituraAbrirAnterior == HIGH) {
    anguloDestino = 100;
    Serial.println("GARAGEM --> Abrindo.");
    delay(100);
  }

  if (leituraFecharAtual == LOW && leituraFecharAnterior == HIGH) {
    anguloDestino = 0;
    Serial.println("GARAGEM --> Fechando.");
    delay(100);
  }

  leituraAbrirAnterior  = leituraAbrirAtual;
  leituraFecharAnterior = leituraFecharAtual;

  // ======================
  // 4. RFID
  // ======================
  String conteudo = "";

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

    for (byte i = 0; i < rfid.uid.size; i++) {
      conteudo.concat(String(rfid.uid.uidByte[i] < 16 ? " 0" : " "));
      conteudo.concat(String(rfid.uid.uidByte[i], HEX));
    }

    if (conteudo == chaveiro_rf || conteudo == cartao_rf) {
      Serial.println("Acesso Liberado!");
      efeitoAcesso();
      MICRO_DOOR.write(0);
      delay(4000);
      MICRO_DOOR.write(180);
    }
    else {
      Serial.println("TAG desconhecida:");
      Serial.println(conteudo);
      efeitoErro();
      delay(1500);
    }
  }

  // ======================
  // 5. MOVIMENTO LENTO - GARAGEM
  // ======================
  moverServoLento();


  // ======================
  // 6. VARAL
  // ======================
  // Ler o potenciômetro (0 a 1023)

  if (digitalRead(SENSOR_CHUVA)) {
    leitura_v = analogRead(POTEN_VARAL);

    // Converter para ângulo (0 a 90) 
    angulo_v = map(leitura_v, 0, 1023, 0, 90); //map(valor, minEntrada, maxEntrada, minSaida, maxSaida)
    
    // Mover o servo
    MICRO_VARAL.write(angulo_v);

    // Mostrar no serial
    Serial.print("VARAL --> Leitura: ");
    Serial.print(leitura_v);
    Serial.print(" | Ângulo: ");
    Serial.println(angulo_v);

    delay(50);
  }
}

  // ======================
  // FUNÇÕES USADAS NO SISTEMA
  // ======================
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
  tone(BUZZER_DOOR, FREQUENCIA); delay(150);
  noTone(BUZZER_DOOR);           delay(150);
  tone(BUZZER_DOOR, FREQUENCIA); delay(150);
  noTone(BUZZER_DOOR);           delay(500);
}

void efeitoErro() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_DOOR, FREQUENCIA);
    delay(100);
    noTone(BUZZER_DOOR);
    delay(100);
  }
}

void alarmeIncendio() {
  tone(BUZZER_DOOR, freq_fogo);
  delay(1500);
  noTone(BUZZER_DOOR);
  delay(1500);
}
