/*Programa desenvolvido para uma balança utilizando célula de carga e amplificador Hx711;
 * 
 * Autores: Helena, Jessica, Reimon
 */

#include "HX711.h"                    //Biblioteca HX711 
#include <LiquidCrystal.h>            //Biblioteca display LCD
#include<math.h>

#define SCK  A1                      //HX711 SCK = pino A1 
#define DT  A0                       //HX711 DT = pino A0 

HX711 balanca;                    

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float calibration_factor = 268256;     //fator de calibração para teste inicial
float pesomed = -1;

void setup()
{
  Serial.begin(9600);            //monitor serial 9600 Bps
  lcd.begin(16, 2);              //display lCD 16x2
  balanca.begin(DT, SCK);       //inicializa a balança
  balanca.set_scale(calibration_factor);
  zeraBalanca();                                                  //configura a tara
}

/*Função para ajustar a tara da balança
 */
void zeraBalanca ()
{
  balanca.tare();                                                 //zera a balança
  lcd.setCursor(0,0);
  lcd.print("Balanca Zerada");
  delay(3000);
}

/*Função para inicar a calibração da balança
 */
void calibHx()
{
  String input; int num = -1;
  do{
    printFatordeCalib();
    if (Serial.available())
    {
      input = Serial.readString();
      num = input.toInt();
      calibration_factor += num;
      balanca.set_scale(calibration_factor);
    }
  }while(num != 0);
}

/*Função de repetição que chama as outras funções
 */
void loop()
{
  printPeso();
  if(Serial.available())
  {
    String input = Serial.readString();
    lcd.clear();
    lcd.setCursor(0,0);
    //lcd.print(input);
    delay(500);
    if(input == "c\n"){
       calibHx();
    }
    else if(input == "z\n"){
      zeraBalanca();
    }
    else if(input == "p\n"){
      pegarMedida();
    }
    else {
     lcd.setCursor(0, 1);
     lcd.print("Comando invalido");
   }
  }
  delay(500);
}

/*Função para imprimir o fator de calibração
 */
void printFatordeCalib()
{
  float peso = balanca.get_units();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("kg: ");
  lcd.print(peso, 3);
  lcd.setCursor(0,1);
  lcd.print("Fator: ");
  lcd.print(calibration_factor);
  delay(500);
}

/*Função para imprimir o peso de cada objeto
 */
void printPeso()
{
  float peso = balanca.get_units();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("kg: ");
  lcd.print(peso, 3);
  if(pesomed > 0){
    lcd.setCursor(0,1);
    lcd.print("Unid: ");
    lcd.print(round(peso/pesomed));
  }
  delay(500);
}

/*Função para imprimir o processo experimental
 */
float imprimirMedida(int num)
{
  float peso = balanca.get_units();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Medida ");
  lcd.print(num);
  lcd.print( "/5");
  lcd.setCursor(0,1);
  lcd.print("kg: ");
  lcd.print(peso, 3);
  delay(500);
  return peso;
} 

/*Função do processo experimental
 */
void pegarMedida()
{
  float soma = 0;
  for(int i=1; i<=5; i++){
    float peso;
    while(!Serial.available()){
      peso = imprimirMedida(i);
      }
      soma += peso;
      Serial.readString();
      }
      pesomed = soma/5;
}
