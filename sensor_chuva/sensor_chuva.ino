// O SENSOR DE CHUVA POSSUI TANTO PORTA DIGITAL, COMO ANALÓGICO, SENDO USADO O QUE FOR NECESSÁRIO OU REQUIRIDO
// PORTAS: VCC, GND, PORTA DIGITAL, PORTA ANALÓGICA

#define pino_sensor 9

bool is_raining = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  is_raining = digitalRead(pino_sensor);

  if (is_raining) {
    Serial.println("Está chovendo!");
  } else {
    Serial.println("Não está chovendo!")
  }
}
