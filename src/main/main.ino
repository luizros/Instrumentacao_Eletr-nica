#include<Wire.h>

// Endereco I2C do sensor MPU-6050
const int MPU = 0x68;

// Salvamento das leituras para calibração
int leituras_pos[100];
int leituras_neg[100];

float ganho;

// Variaveis para armazenar valores do sensor
float AccX, AccY, AccZ, Temp, GyrX, GyrY, GyrZ;

void setup() {
  // Inicializa Serial
  Serial.begin(9600);

  // Inicializa o MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Configura Giroscópio para fundo de escala desejado
  /*
    Wire.write(0b00000000); // fundo de escala em +/-250°/s
    Wire.write(0b00001000); // fundo de escala em +/-500°/s
    Wire.write(0b00010000); // fundo de escala em +/-1000°/s
    Wire.write(0b00011000); // fundo de escala em +/-2000°/s
  */
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x00011000);  // Trocar esse comando para fundo de escala desejado conforme acima
  Wire.endTransmission();

  // Configura Acelerometro para fundo de escala desejado
  /*
      Wire.write(0b00000000); // fundo de escala em +/-2g
      Wire.write(0b00001000); // fundo de escala em +/-4g
      Wire.write(0b00010000); // fundo de escala em +/-8g
      Wire.write(0b00011000); // fundo de escala em +/-16g
  */
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00011000);  // Trocar esse comando para fundo de escala desejado conforme acima
  Wire.endTransmission();
}


void adicionarLeitura(int novaLeitura, char signal) {
  // Desloca todas as leituras para a direita para abrir espaço para a nova leitura
  if signal == 'p'{
    // Adiciona leituras positivas
    for (int i = 10 - 1; i > 0; i--) {
      leituras_pos[i] = leituras_pos[i - 1];
    }

  // Adiciona a nova leitura à primeira posição da lista
  leituras_pos[0] = novaLeitura;
  }else{
    // Adiciona leituras negativas
    for (int i = 10 - 1; i > 0; i--) {
      leituras_neg[i] = leituras_neg[i - 1];
    }

  // Adiciona a nova leitura à primeira posição da lista
  leituras_neg[0] = novaLeitura;
  }
}

float media(int leituras[100]){
  /*
    Função que calcula a média das amostras
  */
  int total = 0;
  int media = 0;
  for(int i = 0; i<100; i ++){
    total = media + leituras[i]
  }
    media = total/100;
  return media
}

void loop() {
  // Comandos para iniciar transmissão de dados
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // Solicita os dados ao sensor

  // Armazena o valor dos sensores nas variaveis correspondentes
  AccX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AccY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AccZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Temp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyrX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyrY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyrZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  adicionarLeitura(GyrX);
  // Imprime na Serial os valores obtidos
  /* Alterar divisão conforme fundo de escala escolhido:
      Acelerômetro
      +/-2g = 16384
      +/-4g = 8192
      +/-8g = 4096
      +/-16g = 2048

      Giroscópio
      +/-250°/s = 131
      +/-500°/s = 65.6
      +/-1000°/s = 32.8
      +/-2000°/s = 16.4
  */
/*
  Serial.print(AccX / 2048);
  Serial.print(" ");
  Serial.print(AccY / 2048);
  Serial.print(" ");
  Serial.println(AccZ / 2048);
*/
  Serial.print(GyrX / 16.4);
  Serial.print(" ");
  Serial.print(GyrY / 16.4);
  Serial.print(" ");
  Serial.println(GyrZ / 16.4);

  // Atraso de 100ms
  delay(5000);
  cont = cont + 1;
  }
}


