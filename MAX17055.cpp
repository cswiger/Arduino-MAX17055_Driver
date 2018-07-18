#include <MAX17055.h>
#include <Wire.h>
 

// Constructors 
MAX17055::MAX17055(void)
  
{
	//doesn't need anything here
}

MAX17055::MAX17055(uint16_t batteryCapacity)
{
	//calcuation based on AN6358 page 13 figure 1.3 for Capacity, but reversed to get the register value 
	writeReg16Bit(DesignCap, batteryCapacity*2);
}
// Public Methods
void MAX17055::setCapacity(uint16_t batteryCapacity)
{
	//calcuation based on AN6358 page 13 figure 1.3 for Capacity, but reversed to get the register value 
	writeReg16Bit(DesignCap, batteryCapacity*2);	
}	

float MAX17055::getCapacity()
{
   	// uint16_t capacity_raw = readReg16Bit(RepCap);
   	uint16_t capacity_raw = readReg16Bit(DesignCap);
	return (capacity_raw * capacity_multiplier_mAH);
}

void MAX17055::setResistSensor(float resistorValue)
{
	resistSensor = resistorValue;
}

float MAX17055::getResistSensor()
{
	return resistSensor;
}

float MAX17055::getInstantaneousCurrent()
{
   	int16_t current_raw = readReg16Bit(Current);
	return current_raw * current_multiplier_mV;
}

float MAX17055::getInstantaneousVoltage()
{
   	uint16_t voltage_raw = readReg16Bit(VCell);
	return voltage_raw * voltage_multiplier_V;
}

float MAX17055::getSOC()
{
   	uint16_t SOC_raw = readReg16Bit(RepSOC);
	return SOC_raw * percentage_multiplier;
}

float MAX17055::getTimeToEmpty()
{
	uint16_t TTE_raw = readReg16Bit(TimeToEmpty);
	return TTE_raw * time_multiplier_Hours;
}

// Private Methods

void MAX17055::writeReg16Bit(uint8_t reg, uint16_t value)
{
  //Write order is LSB first, and then MSB. Refer to AN635 pg 35 figure 1.12.2.5
  Wire2.beginTransmission(I2CAddress);
  Wire2.write(reg);
  Wire2.write( value       & 0xFF); // value low byte
  Wire2.write((value >> 8) & 0xFF); // value high byte
  uint8_t last_status = Wire2.endTransmission();
}

uint16_t MAX17055::readReg16Bit(uint8_t reg)
{
  uint16_t value = 0;  
  Wire2.beginTransmission(I2CAddress); 
  Wire2.write(reg);
  uint8_t last_status = Wire2.endTransmission(false);
  
  Wire2.requestFrom(I2CAddress, (uint8_t) 2); 
  value  = Wire2.read();
  value |= (uint16_t)Wire2.read() << 8;      // value low byte
  return value;
}
