#include <Wire.h>

#define MPU 0x68  // Endereço I2C do MPU-6050

double AcX, AcY, AcZ;
float Pitch, Roll;

const int numMedicoes = 100;
float rollchArray[numMedicoes];

void setup() {
  Serial.begin(9600);
  init_MPU(); // Inicialização MPU6050
}

void loop() {
  static int contMedicoes = 0;

  FunctionsMPU(); // Adquire eixos AcX, AcY, AcZ.

  Roll = FunctionsPitchRoll(AcX, AcY, AcZ);   // Cálculo do ângulo de rotação
  Pitch = FunctionsPitchRoll(AcY, AcX, AcZ);  // Cálculo do ângulo de inclinação

  // Armazena as medidas do Pitch no array
  if (contMedicoes < numMedicoes) {
    rollchArray[contMedicoes] = Roll;
    contMedicoes++;
  } else {
    // Exibe e reinicia o array quando coleta 100 medidas
    Serial.println("Medidas do Pitch:");
    for (int i = 0; i < numMedicoes; i++) {
      Serial.print(rollchArray[i]);
      Serial.print(", \t");
    }
    Serial.println();
    Serial.println("Fim das medidas");
    delay(5000);

    // Reinicia as variáveis para a próxima iteração
    contMedicoes = 0;
    memset(rollchArray, 0, sizeof(rollchArray));
  }

  // Exibe os resultados no monitor serial
  Serial.print("Roll: ");
  Serial.print(Pitch);
  Serial.print("\n");
  delay(100);  // Atraso entre medições
}

void init_MPU() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // definido como zero (ativa o MPU-6050)
  Wire.endTransmission(true);
  delay(1000);
}

// Função para calcular os ângulos de inclinação e rotação
double FunctionsPitchRoll(double A, double B, double C) {
  double DatoA, DatoB, Value;
  DatoA = A;
  DatoB = (B * B) + (C * C);
  DatoB = sqrt(DatoB);

  Value = atan2(DatoA, DatoB);
  Value = Value * 180 / 3.14;

  return Value;
}

// Função para aquisição dos eixos X, Y, Z do MPU6050
void FunctionsMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
}
