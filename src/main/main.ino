#include <Wire.h>

// Valores obtidos para compensação
#define GANHO_X  1.0
#define OFFSET_X  0.030000000000000027
#define GANHO_Y  1.0
#define OFFSET_Y  0.0
#define GANHO_Z  1.0
#define OFFSET_Z  0.040000000000000036

// Endereco I2C do sensor MPU-6050
const int MPU = 0x68;

// Variaveis para armazenar valores do sensor
float AccX, AccY, AccZ, Temp, GyrX, GyrY, GyrZ;

// variaveis para salvar os valores de aceleração dos eixos
float aceleracao[100]; // Talvez seja melhor mudar para um array de 3 dimensões para armazenar todos os eixos

// contador para controlar qtd. de iterações
int cont = 0;

// Função para calibrar os dados
void calibrar(float& valor, float ganho, float offset) {
  valor = 1 * valor - offset;
}

void plota(bool setCalibration) {
  // Se setCalibration == true faz a calibração, se false, não faz a calibração
  // Calibração : A_saida = GANHO_EIXO * aceleração i +- offset_EIXO(O OFFSSET depende se a saida for negativa OU POSITIVA)

  Serial.print("Vou plotar os dados\n\n");
  for (int i = 0; i < 100; i++) {
    Serial.println(aceleracao[i]);
    if (i % 20 == 0){
      Serial.print("Printei 25 amostras n calibradas");
    }
  }
  Serial.print("Terminei de plotar amostras n calibradas\n\n");
  Serial.print("Vou plotar imprimir as amostras calibradas\n\n");

  for (int i = 0; i < 100; i++) {
    if (setCalibration) {
      // Aplica calibração se necessário
      calibrar(aceleracao[i], GANHO_X, OFFSET_X);
    }
    Serial.println(aceleracao[i]);
    if (i % 20 == 0){
      Serial.print("Printei 25 amostras calibradas");
    }
  }
  Serial.print("Terminei de plotar\n\n");
  delay(100000);
}


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
  Wire.write(0x00000000);  // Trocar esse comando para fundo de escala desejado conforme acima
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
  Serial.print("Preparando para coletar as 100 amostras\n");
  delay(10000);
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

  Serial.print((AccX / 2048));
  Serial.print(" ");
  Serial.print(AccY / 2048);
  Serial.print(" ");
  Serial.println(AccZ / 2048);

  // Salva as 100 amostras positivas ou negativas em +- 1g
  // Para trocar o eixo basta mudar Acc<eixo> para 'x', 'y' ou 'z'
  if ((AccX / 2048) <= 0 && (cont < 100)) {
    Serial.print("Entrei no if, estou coletando dados\n\n");
    aceleracao[cont] = AccX / 2048;
    cont += 1;
  }

  if (cont == 100) {
    cont = 0;
    Serial.print("Entrei no plot\n");
    plota(true); // Define como true para calibrar os dados ao plotar
  }

  // Atraso de 100ms
  delay(100);
}