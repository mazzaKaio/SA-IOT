#include <MFRC522.h>
#include <SPI.h>
#define PINO_RST 9
#define PINO_SDA 10

MFRC522 rfid(PINO_SDA, PINO_RST);

void setup() {
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Aproxime sua tag...");
  Serial.println();
}

void loop() {
  // Procurando pela TAG
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Seleciona uma TAG
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Mostra UID na Serial
  Serial.print("UID da TAG: ");
  String conteudo = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    conteudo.concat(String(rfid.uid.uidByte[i] < HEX ? " 0" : " "));
    conteudo.concat(String(rfid.uid.uidByte[i], HEX));
  }

  Serial.println(conteudo);
}
