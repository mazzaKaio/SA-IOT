// O SENSOR DE CHUVA POSSUI TANTO PORTA DIGITAL, COMO ANALÓGICO, SENDO USADO O QUE FOR NECESSÁRIO OU REQUIRIDO
// PORTAS: VCC, GND, PORTA DIGITAL, PORTA ANALÓGICA
#include <Servo.h>
#define pino_sensor 1
#define servo_janela 0

bool is_raining = 0;
Servo micro;

void setup() {
  Serial.begin(9600);
  micro.attach(servo_janela);
  micro.write(0);
}

void loop() {
  is_raining = digitalRead(pino_sensor);

  if (is_raining) {
    micro.write(180);
  } else {
    micro.write(0);
  }
}
