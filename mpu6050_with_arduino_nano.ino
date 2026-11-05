#include <Wire.h>

const uint8_t MPU_ADDR = 0x68;   // use 0x69 if AD0 is tied to VCC

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  // Wake up the MPU6050 (it starts in sleep mode)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);              // PWR_MGMT_1
  Wire.write(0x00);              // clear sleep bit
  if (Wire.endTransmission(true) != 0) {
    Serial.println("MPU6050 not responding. Check wiring.");
    while (1);
  }

  // Accelerometer range: +/-2g
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Gyroscope range: +/-250 deg/s
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Serial.println("MPU6050 initialized");
}

void loop() {
  // Read 14 bytes starting at ACCEL_XOUT_H (0x3B)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  if (Wire.requestFrom(MPU_ADDR, (uint8_t)14, (uint8_t)true) != 14) {
    Serial.println("Read failed");
    delay(500);
    return;
  }

  int16_t accX  = (Wire.read() << 8) | Wire.read();
  int16_t accY  = (Wire.read() << 8) | Wire.read();
  int16_t accZ  = (Wire.read() << 8) | Wire.read();
  int16_t rawT  = (Wire.read() << 8) | Wire.read();
  int16_t gyroX = (Wire.read() << 8) | Wire.read();
  int16_t gyroY = (Wire.read() << 8) | Wire.read();
  int16_t gyroZ = (Wire.read() << 8) | Wire.read();

  // Convert to real units
  float ax = accX / 16384.0;           // g
  float ay = accY / 16384.0;
  float az = accZ / 16384.0;
  float gx = gyroX / 131.0;            // deg/s
  float gy = gyroY / 131.0;
  float gz = gyroZ / 131.0;
  float temp = rawT / 340.0 + 36.53;   // deg C

  Serial.print("Accel (g)  X: "); Serial.print(ax, 2);
  Serial.print("  Y: ");         Serial.print(ay, 2);
  Serial.print("  Z: ");         Serial.print(az, 2);

  Serial.print(" | Gyro (deg/s)  X: "); Serial.print(gx, 2);
  Serial.print("  Y: ");               Serial.print(gy, 2);
  Serial.print("  Z: ");               Serial.print(gz, 2);

  Serial.print(" | Temp: "); Serial.print(temp, 1);
  Serial.println(" C");

  delay(500);
}