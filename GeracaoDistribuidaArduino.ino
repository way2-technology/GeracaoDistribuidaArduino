#include <Wire.h>
#include "rgb_lcd.h"

#define SAIDA_RAZAO_CICLICA 9
#define TENSAO A0
#define CORRENTE A1

#define RESISTENCIA_1 1000.0f
#define RESISTENCIA_2 110.0f
#define DIVISOR_DE_TENSAO RESISTENCIA_1 / RESISTENCIA_2

#define SAMPLE_COUNT 100

template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}

const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

rgb_lcd lcd;

float potenciaAnterior = 0.0f;
int razaoCiclica = 100;
int razaoCiclicaAnterior = 100;
int step = 1;

void setup() {
  TCCR1B = TCCR1B & 0b11111000 | 0X01; // Seta pinos 9 e 10 para 31250 Hz

  pinMode(SAIDA_RAZAO_CICLICA, OUTPUT);
  pinMode(TENSAO, INPUT);
  pinMode(CORRENTE, INPUT);

  lcd.begin(16, 2); // 16 colunas, 2 linhas

  lcd.setRGB(colorR, colorG, colorB);

  delay(1000);

  Serial.begin(9600);
}

void loop() {
  analogWrite(SAIDA_RAZAO_CICLICA, razaoCiclica);

  delay(1000);

  float tensaoMapeada = readAverage(TENSAO, 0.0f, 5.0f);
  float tensao =  tensaoMapeada * DIVISOR_DE_TENSAO;
  float corrente = readAverage(CORRENTE, -30.0f, 30.0f);
  float potencia = tensao * corrente;

  printInfoOnLcd(tensao, corrente, potencia, razaoCiclica);

  bool potenciaAumentou = potencia >= potenciaAnterior;
  bool razaoCiclicaAumentou = razaoCiclica >= razaoCiclicaAnterior;

  int sinalStep = (potenciaAumentou == razaoCiclicaAumentou) ? 1 : -1;

  razaoCiclicaAnterior = razaoCiclica;

  razaoCiclica += step * sinalStep;
  razaoCiclica = constrain(razaoCiclica, 0, 255);

  potenciaAnterior = potencia;
  
}

float readAverage(int pin, float min, float max) {
  float readings = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    float mappedReading = map((float)analogRead(pin), 0.0f, 1023.0f, min, max);
    readings += mappedReading;
  }
  return (float)(readings / SAMPLE_COUNT);
}

void printInfoOnLcd(float tensao, float corrente, float potencia, float razaoCiclica) {
  float razaoCiclicaPorcentagem = 100.0f * (razaoCiclica / 255.0f);

  Serial << tensao << "V " << corrente << "A " << potencia << "W " << razaoCiclicaPorcentagem << "%" << '\n';
  
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(String(tensao) + String("V"));
  lcd.setCursor(9, 0);
  lcd.print(String(corrente) + String("A"));

  lcd.setCursor(0, 1);
  lcd.print(String(potencia) + String("W"));
  lcd.setCursor(9, 1);
  lcd.print(String(razaoCiclicaPorcentagem) + String("%"));
}

float map(float x, float inMin, float inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}
