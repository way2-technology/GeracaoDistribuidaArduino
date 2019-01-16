#define PINO 9
#define TENSAO A0
#define CORRENTE A1

void setup() {
  TCCR1B = TCCR1B & 0b11111000 | 0X01;
  pinMode(PINO, OUTPUT);
  pinMode(TENSAO, INPUT);
  pinMode(CORRENTE, INPUT);
  Serial.begin(9600);
}

float potenciaAnterior = 0;
int razaoCiclica = 100;
int razaoCiclicaAnterior = 100;

void loop() {
  analogWrite(PINO, razaoCiclica);
  delay(2000);
  float tensao = (analogRead(TENSAO)*0.0048828125) * 1000 / 110;
  float corrente = (analogRead(CORRENTE) - 512) * (0.05859375);
  float potenciaAtual = tensao * corrente;

  Serial.print(tensao);
  Serial.print(" - ");
  Serial.print(corrente);
  Serial.print(" - ");
  Serial.print(potenciaAtual);
  Serial.print(" - ");
  Serial.print(razaoCiclica);
  Serial.println();

  if ((potenciaAtual >= potenciaAnterior) && (razaoCiclica >= razaoCiclicaAnterior)) {
    razaoCiclicaAnterior = razaoCiclica;
    razaoCiclica += 10;
  } else if ((potenciaAtual < potenciaAnterior) && (razaoCiclica >= razaoCiclicaAnterior)) {
    razaoCiclicaAnterior = razaoCiclica;
    razaoCiclica -= 10;
  } else if ((potenciaAtual >= potenciaAnterior) && (razaoCiclica <= razaoCiclicaAnterior)) {
    razaoCiclicaAnterior = razaoCiclica;
    razaoCiclica -= 10;
  } else if ((potenciaAtual < potenciaAnterior) && (razaoCiclica <= razaoCiclicaAnterior)) {
    razaoCiclicaAnterior = razaoCiclica;
    razaoCiclica += 10;
  }
  
  
  potenciaAnterior = potenciaAtual;
  if (razaoCiclica > 255) {
    razaoCiclica = 255;
  } else if (razaoCiclica < 0) {
    razaoCiclica = 0;
  }
}
