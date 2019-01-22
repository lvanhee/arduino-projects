// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>

/**Lois
 * Wiring: 5V->VCC
 * GND->GND
 * SCL->A5
 * SDA->A4
 * INT->2
 */

const int MPU_addr=0x68;  // I2C address of the MPU-6050
double AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

double meanAcX,meanAcY,meanAcZ;
double meanVarianceAcX, meanVarianceAcY,meanVarianceAcZ;
void accelerationSetup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
 // Serial.begin(9600);
}

boolean peakDetected;

double maxVarianceX, maxVarianceY, maxVarianceZ;

boolean isPeakDetected()
{
  return peakDetected;
}

double getRelativeExcitation()
{
  double maximum = meanVarianceAcX/maxVarianceX;
  
  if(maximum < meanVarianceAcY/maxVarianceY) maximum = meanVarianceAcY/maxVarianceY;
  if(maximum < meanVarianceAcZ/maxVarianceZ) maximum = meanVarianceAcZ/maxVarianceZ;
  return maximum;
}

double maxX, maxY, maxZ = 0;
double getAcceleration(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  peakDetected = false;
  
  meanAcX = meanAcX*0.9+0.1*AcX;
  meanVarianceAcX = meanVarianceAcX*0.99 + 0.01 * abs(meanAcX-AcX);
  if(meanVarianceAcX>maxVarianceX) maxVarianceX=meanVarianceAcX;
  else maxVarianceX=0.9999*maxVarianceX +0.0001*meanVarianceAcX;
  if(abs(AcX - meanAcX) > 4*maxVarianceX) peakDetected = true;
  
  meanAcY = meanAcY*0.9+0.1*AcY;
  meanVarianceAcY = meanVarianceAcY*0.99 + 0.01 * abs(meanAcY-AcY);
  if(meanVarianceAcY>maxVarianceY) maxVarianceY=meanVarianceAcY;
  else maxVarianceY=0.9999*maxVarianceY +0.0001*meanVarianceAcY;
  if(abs(AcY - meanAcY) > 4*maxVarianceY) peakDetected = true;
  
  meanAcZ = meanAcZ*0.9+0.1*AcZ;
  meanVarianceAcZ = meanVarianceAcZ*0.99 + 0.01 * abs(meanAcZ-AcZ);
  if(meanVarianceAcZ>maxVarianceZ) maxVarianceZ=meanVarianceAcZ;
  else maxVarianceZ=0.9999*maxVarianceZ +0.0001*meanVarianceAcZ;
  if(abs(AcZ - meanAcZ) > 4*maxVarianceZ) peakDetected = true;
  
  //Serial.print("AcX = ");
  Serial.print(""); 
  //Serial.print(AcX);
  //Serial.print(" ");

 Serial.print(getRelativeExcitation());
   Serial.print(" "); 

 Serial.print(maxVarianceY);
   Serial.print(""); 


 /*Serial.print("");
 Serial.print(meanVarianceAcX);
 Serial.print("/");
 Serial.print(maxVarianceX);
 Serial.print(" Y");
 Serial.print(meanVarianceAcY);
 Serial.print("/");
 Serial.print(maxVarianceY);
 Serial.print(" Z");
 Serial.print(meanVarianceAcZ);
 Serial.print("/");
 Serial.print(maxVarianceZ);*/
  /*Serial.print(meanAcX);
  Serial.print(" ");
  Serial.print(meanAcX+meanVarianceAcX);
  Serial.print(" "); 
  Serial.print(meanAcX-meanVarianceAcX);
  Serial.print(" "); */

  
  //Serial.print(" | AcY = "); 
 /* Serial.print(AcY);
 
  Serial.print(" "); 
  //Serial.print(" | AcZ = ");
  Serial.print(AcZ);
  Serial.print(" ");*/
  
/*  //Serial.print(" | Tmp = "); 
  Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" "); */
  //Serial.print(" | GyX = ");
/*  Serial.print(GyX);
  Serial.print(" ");
  //Serial.print(" | GyY = ");
  Serial.print(GyY);
  Serial.print(" "); 
 // Serial.print(" | GyZ = ");
  Serial.print(GyZ);*/
  return abs(AcX)+abs(AcY)+abs(AcZ);
  //delay(333);
}

double prevX, prevY, prevZ=0;
boolean isHighChange(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

boolean res = abs(AcX-prevX) > 1000 || abs(AcY-prevY) > 1000 || abs(AcZ-prevZ) > 1000; 
prevX = AcX;
prevY = AcY;
prevZ = AcZ;
  return res;
 
}
