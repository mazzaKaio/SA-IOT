// INCLUSÃO DE BIBLIOTECAS
#include <Wire.h>            
#include <LiquidCrystal_I2C.h> //Fonte da Biblioteca I2C: Canal Brincando com Ideias https://www.youtube.com/brincandocomideias

//DEFININDO ENDEREÇO DO DISPLAY, NÚMERO DE COLUNAS E LINHAS
#define endereco  0x27 // Endereços comuns: 0x27, 0x3F
#define colunas   16
#define linhas    2
LiquidCrystal_I2C lcd(endereco, colunas, linhas);

// VARIÁVEIS
int sensordechuva = A0; //Determina a ligação do sensor com o pino analógica A0
int valor = 0; //Valor lido pela constante que o arduino recebe


void setup() {
  
  lcd.init(); //Inicia a comunicação com o Display
  lcd.backlight(); //Liga a iluminação do Display
  Serial.begin(1200);//Ligando Serial, LEMBRE DE ALTERAR O VALOR NO SERIAL

}

void loop() 

{
 
  valor = analogRead(sensordechuva); //Lendo valor analógica

  if (valor>750); // Ver como o sensor se comporta quando em contato com a água e ir colocando os valores adequados, você pode alterar conforme achar necessário
  {
  lcd.clear();
  lcd.setBacklight(HIGH);
  lcd.setCursor(0,0);
  lcd.print(" TEMPO SECO ");   
  Serial.print(" TEMPO SECO, ESTENDA AS ROUPAS NO VARAL ");
  
  }  
    
  
   if (valor<=750)
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CHUVA"); 
  Serial.print(" CHUVAAA, RECOLHA AS ROUPAS! ");
  lcd.setCursor(0,1);
  lcd.print("Recolha a Roupa");
  delay(3000);
  
}
 
}
