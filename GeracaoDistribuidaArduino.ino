template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
} 

#define PINO 9
#define TENSAO A0
#define CORRENTE A1

void setup() {
  TCCR1B = TCCR1B & 0b11111000 | 0X01; // Seta pinos 9 e 10 para 31250 Hz

  pinMode(PINO, OUTPUT);
  pinMode(TENSAO, INPUT);
  pinMode(CORRENTE, INPUT);

  Serial.begin(9600);
}

float potenciaAnterior = 0;
int razaoCiclica = 100;
int razaoCiclicaAnterior = 100;
int step = 10;

void loop() {
  analogWrite(PINO, razaoCiclica);
  delay(2000);
  float tensao = (analogRead(TENSAO) * 0.0048828125) * 1000 / 110;
  float corrente = (analogRead(CORRENTE) - 512) * (0.05859375);
  float potencia = tensao * corrente;

  Serial << "V: " << tensao << " I: " << corrente << " P: " << potencia << " D: " << razaoCiclica; 
  
  bool potenciaAumentou = potencia >= potenciaAnterior;
  bool razaoCiclicaAumentou = razaoCiclica >= razaoCiclicaAnterior;  

  float sinalStep = (potenciaAnterior == razaoCiclicaAumentou) ? 1.0 : -1.0;

  razaoCiclicaAnterior = razaoCiclica;
  razaoCiclica += step * sinalStep;
  
  potenciaAnterior = potencia;
  razaoCiclica = constrain(razaoCiclica, 0, 255);
}
