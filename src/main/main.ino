<<<<<<< HEAD
#include<Wire.h>

// Endereco I2C do sensor MPU-6050
=======
#include <Wire.h>
#include <Arduino_JSON.h>

>>>>>>> f6cf749be33ed142cc5417e8a3ed2083c49fe4a9
const int MPU = 0x68;

int leituras_pos_x[100];
int leituras_neg_x[100];

int leituras_pos_y[100];
int leituras_neg_y[100];

int leituras_pos_z[100];
int leituras_neg_z[100];

int cont = 0;

void adicionarLeituraX(int novaLeitura, char signal) {
  if (signal == 'p') {
    for (int i = 99; i > 0; i--) {
      leituras_pos_x[i] = leituras_pos_x[i - 1];
    }
    leituras_pos_x[0] = novaLeitura;
  } else if (signal == 'n') {
    for (int i = 99; i > 0; i--) {
      leituras_neg_x[i] = leituras_neg_x[i - 1];
    }
    leituras_neg_x[0] = novaLeitura;
  }
}

void adicionarLeituraY(int novaLeitura, char signal) {
  if (signal == 'p') {
    for (int i = 99; i > 0; i--) {
      leituras_pos_y[i] = leituras_pos_y[i - 1];
    }
    leituras_pos_y[0] = novaLeitura;
  } else if (signal == 'n') {
    for (int i = 99; i > 0; i--) {
      leituras_neg_y[i] = leituras_neg_y[i - 1];
    }
    leituras_neg_y[0] = novaLeitura;
  }
}

void adicionarLeituraZ(int novaLeitura, char signal) {
  if (signal == 'p') {
    for (int i = 99; i > 0; i--) {
      leituras_pos_z[i] = leituras_pos_z[i - 1];
    }
    leituras_pos_z[0] = novaLeitura;
  } else if (signal == 'n') {
    for (int i = 99; i > 0; i--) {
      leituras_neg_z[i] = leituras_neg_z[i - 1];
    }
    leituras_neg_z[0] = novaLeitura;
  }
}

float calcularMedia(int leituras[100]) {
  int total = 0;
  for (int i = 0; i < 100; i++) {
    total += leituras[i];
  }
  return total / 100.0;
}

float aceleracao[100];
int cont = 0;

void plota(){
  Serial.print("Vou plotar os dados\n\n");
  for(int i = 0; i<100; i++){
    Serial.println(aceleracao[i]);
  }
  Serial.print("Terminei de plotar\n\n");
  delay(100000);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Configura Giroscópio para fundo de escala desejado
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
<<<<<<< HEAD
  Wire.write(0x00000000);  // Trocar esse comando para fundo de escala desejado conforme acima
=======
  Wire.write(0b00011000);
>>>>>>> f6cf749be33ed142cc5417e8a3ed2083c49fe4a9
  Wire.endTransmission();

  // Configura Acelerometro para fundo de escala desejado
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00011000);
  Wire.endTransmission();
  Serial.print("Preparando para coletar as 100 amostras\n");
  delay(10000);
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  int AccX = Wire.read() << 8 | Wire.read();
  int AccY = Wire.read() << 8 | Wire.read();
  int AccZ = Wire.read() << 8 | Wire.read();
  int Temp = Wire.read() << 8 | Wire.read();
  int GyrX = Wire.read() << 8 | Wire.read();
  int GyrY = Wire.read() << 8 | Wire.read();
  int GyrZ = Wire.read() << 8 | Wire.read();

  // Adiciona leituras aos vetores correspondentes
  adicionarLeituraX(AccX, (AccX >= 0) ? 'p' : 'n');
  adicionarLeituraY(AccY, (AccY >= 0) ? 'p' : 'n');
  adicionarLeituraZ(AccZ, (AccZ >= 0) ? 'p' : 'n');

<<<<<<< HEAD
      Giroscópio
      +/-250°/s = 131
      +/-500°/s = 65.6
      +/-1000°/s = 32.8
      +/-2000°/s = 16.4
  */

  Serial.print(AccX / 2048);
  Serial.print(" ");
  Serial.print(AccY / 2048);
  Serial.print(" ");
  Serial.println(AccZ / 2048);


  // Salva as 100 amostras positivas em +- 1g
  // Para trocar o eixo basta mudo Acc<eixo> para 'x', 'y' ou 'z'
  if ((AccX / 2048) >= 0 && (cont < 100)){
    Serial.print("Entrei no if, estou coletando dados\n\n");
    aceleracao[cont] = AccX/2048;
    cont += 1;
  }
  if (cont == 100){
    cont = 0;
    Serial.print("Entrei no plot\n");
    plota();
  }
  

/*
  Serial.print(GyrX / 131);
  Serial.print(" ");
  Serial.print(GyrY / 131);
  Serial.print(" ");
  Serial.println(GyrZ / 131);
*/


 // Atraso de 100ms
  delay(100);
=======
  cont++;

  // Coleta 100 amostras para cada eixo
  if (cont == 100) {
    // Calcula médias
    float media_pos_x = calcularMedia(leituras_pos_x);
    float media_neg_x = calcularMedia(leituras_neg_x);
    float media_pos_y = calcularMedia(leituras_pos_y);
    float media_neg_y = calcularMedia(leituras_neg_y);
    float media_pos_z = calcularMedia(leituras_pos_z);
    float media_neg_z = calcularMedia(leituras_neg_z);
    Serial.print("Media x pos: ");
    Serial.println(media_pos_x);
    Serial.print("Media x neg: ");
    Serial.println(media_neg_x);
    Serial.print("Media y pos: ");
    Serial.println(media_pos_y);
    Serial.print("Media y neg: ");
    Serial.println(media_neg_y);
    Serial.print("Media z pos: ");
    Serial.println(media_pos_z);
    Serial.print("Media z neg: ");
    Serial.println(media_neg_z);
    Serial.print("-------------------------------------------\n");
    cont = 0;
    delay(10000);
  }

  delay(50); // Atraso para evitar leituras muito rápidas
>>>>>>> f6cf749be33ed142cc5417e8a3ed2083c49fe4a9
}
