//CONEXÕES RFIS: SDA - 10 | SCK - 13 | MOSI - 11 | MISO - 12 | RST - 9

#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

#define PINO_RST 9
#define PINO_SDA 10
#define LED_RED 2
#define LED_GREEN 3
#define BUZZER_DOOR 4
#define SERVO_DOOR 5

MFRC522 rfid(PINO_SDA, PINO_RST);
Servo micro;

String cartao_rf = " 93 d3 75 e4";
String chaveiro_rf = " 07 e3 41 66";

int FREQUENCIA = 3300; // Hz

void setup() {
  Serial.begin(9600);
  SPI.begin();

  rfid.PCD_Init();
  micro.attach(SERVO_DOOR);

  Serial.println("Aproxime sua tag...");
  Serial.println();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_DOOR, OUTPUT);

  micro.write(180);
  digitalWrite(LED_RED, HIGH);
  noTone(BUZZER_DOOR);
}

void loop() {
  String conteudo = "";
  // Procurando pela TAG
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Seleciona uma TAG
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < rfid.uid.size; i++) {
    conteudo.concat(String(rfid.uid.uidByte[i] < HEX ? " 0" : " "));
    conteudo.concat(String(rfid.uid.uidByte[i], HEX));
  }

  // Verificação
  if (conteudo == chaveiro_rf) {
    Serial.println("Bem-vindo, CHAVEIRO!");
    ascender();
    micro.write(0);
    delay(15000);
    micro.write(180);

  } else if (conteudo == cartao_rf) {
    Serial.println("Bem-vindo, CARTÃO!");
    ascender();
    micro.write(0);
    delay(15000);
    micro.write(180);

  } else {
    Serial.println("Desconhecido!!!");
    Serial.println(conteudo);
    piscar();
    delay(1500);
  }
}

void ascender() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);

  tone(BUZZER_DOOR, FREQUENCIA);
  delay(150);
  noTone(BUZZER_DOOR);
  delay(150);
  tone(BUZZER_DOOR, FREQUENCIA);
  delay(150);
  noTone(BUZZER_DOOR);
  delay(500);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
}

void piscar() {
  digitalWrite(LED_RED, LOW);
  tone(BUZZER_DOOR, FREQUENCIA);
  delay(100);
  digitalWrite(LED_RED, HIGH);
  noTone(BUZZER_DOOR);
  delay(100);
  digitalWrite(LED_RED, LOW);
  tone(BUZZER_DOOR, FREQUENCIA);
  delay(100);
  digitalWrite(LED_RED, HIGH);
  noTone(BUZZER_DOOR);
  delay(100);
  digitalWrite(LED_RED, LOW);
  tone(BUZZER_DOOR, FREQUENCIA);
  delay(100);
  digitalWrite(LED_RED, HIGH);
  noTone(BUZZER_DOOR);
}
