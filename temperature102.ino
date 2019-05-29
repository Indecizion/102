#include <application.h>
#include <spark_wiring_i2c.h>
#define Addr 0x48

double cTemp = 0.0, fTemp = 0.0;
int temp = 0;
void setup()
{
  Particle.variable("i2cdevice", "TCN75A");
  Particle.variable("cTemp", cTemp);

  Wire.begin();

  Serial.begin(9600);


  Wire.beginTransmission(Addr);

  Wire.write(0x01);

  Wire.write(0x60);
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[2];


  Wire.beginTransmission(Addr);

  Wire.write(0x00);

  Wire.endTransmission();


  Wire.requestFrom(Addr, 2);


  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  temp = (((data[0] * 256) + (data[1] & 0xF0)) / 16);
  if(temp > 2047)
  {
    temp -= 4096;	
  }
  cTemp = temp * 0.0625;
  fTemp = (cTemp * 1.8) + 32;

  Particle.publish("Temperature in Celsius : ", String(cTemp));
  Particle.publish("Temperature in Fahrenheit : ", String(fTemp));
  delay(1000);
}