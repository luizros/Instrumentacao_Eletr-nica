/*
  Autor: Embarcados
*/

// Inclui a biblioteca para configurar o barramento I2C
#include <Wire.h>

// Inclui bibliotecas para habilitar os métodos
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Declaração do objeto
Adafruit_MPU6050 mpu;

// Estrutura para receber as leituras dos sensores
sensors_event_t acc, gyr, temp;

void setup()
{
  // Inicializa a comunicação serial
  Serial.begin(115200);
  Serial.println("Iniciando demo MPU6050...");

  // Inicializa o módulo MPU6050 e verifica se foi configurado corretamente
  if (!mpu.begin())
  {
	// Caso não seja configurado corretamente, informa no monitor serial
	Serial.println("Falha ao configurar MPU6050");
  }

  Serial.println("MPU6050 encontrado");
}

void loop()
{
  // Realiza as leituras do acelerômetro, giroscópio e do sensor de temperatura.
  mpu.getEvent(&acc, &gyr, &temp);

  // Mostra na tela as leituras realizadas no Monitor Serial
  Serial.println(" # Acelerômetro: ");
  Serial.print("  X: ");
  Serial.print(acc.acceleration.x);
  Serial.print("\t Y: ");
  Serial.print(acc.acceleration.y);
  Serial.print("\t Z: ");
  Serial.println(acc.acceleration.z);

  Serial.println(" # Giroscópio: ");
  Serial.print("  X: ");
  Serial.print(gyr.gyro.x);
  Serial.print("\t Y: ");
  Serial.print(gyr.gyro.y);
  Serial.print("\t Z: ");
  Serial.println(gyr.gyro.z);
  Serial.println("--------------------");

  // Mostra as leituras realizadas no Plotter Serial

  // Descomente para mostrar as leituras individuais do acelerômetro no Plotter Serial
  /*
  Serial.print(acc.acceleration.x);
  Serial.print("\t");
  Serial.print(acc.acceleration.y);
  Serial.print("\t");
  Serial.println(acc.acceleration.z);
  */

  // Descomente para mostrar as leituras individuais do giroscópio no Plotter Serial
  /*
  Serial.print(gyr.gyro.x);
  Serial.print("\t");
  Serial.print(gyr.gyro.y);
  Serial.print("\t");
  Serial.println(gyr.gyro.z);
  */

  delay(50);
}