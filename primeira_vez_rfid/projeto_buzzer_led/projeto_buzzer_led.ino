#include <MFRC522.h>
#include <SPI.h>
#define PINO_RST 9
#define PINO_SDA 10
#define red 2
#define green 3
#define buzz 4

MFRC522 rfid(PINO_SDA, PINO_RST);

String cartao_rf = " 93 d3 75 e4";
String chaveiro_rf = " 34 dd 7a a7";

int freq = 3300; // Hz

void setup() {
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Aproxime sua tag...");
  Serial.println();

  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzz, OUTPUT);

  digitalWrite(red, HIGH);
  noTone(buzz);
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
    delay(1500);

  } else if (conteudo == cartao_rf) {
    Serial.println("Bem-vindo, CARTÃO!");
    ascender();
    delay(1500);

  } else {
    Serial.println("Desconhecido!!!");
    Serial.println(conteudo);
    piscar();
    delay(1500);
  }
}

void ascender() {
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);

  tone(buzz, freq);
  delay(150);
  noTone(buzz);
  delay(150);
  tone(buzz, freq);
  delay(150);
  noTone(buzz);
  delay(500);

  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);
}

void piscar() {
  digitalWrite(red, LOW);
  tone(buzz, freq);
  delay(100);
  digitalWrite(red, HIGH);
  noTone(buzz);
  delay(100);
  digitalWrite(red, LOW);
  tone(buzz, freq);
  delay(100);
  digitalWrite(red, HIGH);
  noTone(buzz);
  delay(100);
  digitalWrite(red, LOW);
  tone(buzz, freq);
  delay(100);
  digitalWrite(red, HIGH);
  noTone(buzz);
}
