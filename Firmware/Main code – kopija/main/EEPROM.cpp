
/*

	EEPROM FUNCTIONS
	AT24C32

*/


// MODULES
#include 																			"Headers\EEPROM.h"
#include <Arduino.h>



// OBJECT
EEPROMClass EEPROM;



// FUNCTIONS
int8_t EEPROMClass::highAddressByte(int16_t address)
{
	return (address >> 8);
}

int8_t EEPROMClass::lowAddressByte(int16_t address)
{
	return (address - (highAddressByte(address) << 8));
}


// ONE BYTE FUNCTIONS
void EEPROMClass::write8(int16_t const address, uint8_t const value)
{
	Wire.beginTransmission(AT24C32);

	Wire.write(highAddressByte(address));
	Wire.write(lowAddressByte(address));

	Wire.write(value);
	delay(EEPROM_DELAY);
	Wire.endTransmission();	
}

uint8_t EEPROMClass::read8(int16_t const address)
{
	Wire.beginTransmission(AT24C32);

	Wire.write(highAddressByte(address));
	Wire.write(lowAddressByte(address));
	Wire.endTransmission();

	Wire.requestFrom(AT24C32, 1);
	delay(EEPROM_DELAY);

	return (Wire.read());
}

void EEPROMClass::update8(int16_t const address, uint8_t const value)
{
	if (read8(address) != value) write8(address, value);
}


// TWO BYTES FUNCTIONS
void EEPROMClass::write16(int16_t const address, uint16_t const value)
{
    uint8_t byteValue = (value & 0xFF);
    write8(address, byteValue);

    byteValue = ((value >> 8) & 0xFF);
    write8(address + 1, byteValue);
}

uint16_t EEPROMClass::read16(int16_t const address)
{
	uint16_t second = read8(address);
    uint16_t first = read8(address + 1);

    return ((second << 0) & 0xFF) + ((first << 8) & 0xFFFF);
}

void EEPROMClass::update16(int16_t const address, uint16_t const value)
{
	if (read16(address) != value) write16(address, value);
}


// FOUR BYTES FUNCTIONS
void EEPROMClass::write32(int16_t address, uint32_t value)
{
    uint8_t byteValue = (value & 0xFF);
    write8(address, byteValue);

    byteValue = ((value >> 8) & 0xFF);
    write8(address + 1, byteValue);

    byteValue = ((value >> 16) & 0xFF);
    write8(address + 2, byteValue);

    byteValue = ((value >> 24) & 0xFF);
    write8(address + 3, byteValue);
}

uint32_t EEPROMClass::read32(int16_t address)
{
	uint32_t buffer[4];
	for (uint8_t i = 0; i < 4; i++) buffer[i] = read8(address + i);

    return (((buffer[0] << 0) & 0xFF) +
    ((buffer[1] << 8) & 0xFFFF) +
    ((buffer[2] << 16) & 0xFFFFFF) +
    ((buffer[3] << 24) & 0xFFFFFFFF));
}

void EEPROMClass::update32(int16_t address, uint32_t value)
{
	if (read32(address) != value) write32(address, value);
}


// SIX BYTES FUNCTIONS
void EEPROMClass::write48(int16_t address, uint64_t value)
{
    uint8_t byteValue = (value & 0xFF);
    write8(address, byteValue);

    byteValue = ((value >> 8) & 0xFF);
    write8(address + 1, byteValue);

    byteValue = ((value >> 16) & 0xFF);
    write8(address + 2, byteValue);

    byteValue = ((value >> 24) & 0xFF);
    write8(address + 3, byteValue);

    byteValue = ((value >> 32) & 0xFF);
    write8(address + 4, byteValue);

    byteValue = ((value >> 40) & 0xFF);
    write8(address + 5, byteValue);
}

uint64_t EEPROMClass::read48(int16_t address)
{
	uint64_t buffer[6];
	for (uint8_t i = 0; i < 6; i++) buffer[i] = read8(address + i);

    return (((buffer[0] << 0) & 0xFF) +
    ((buffer[1] << 8) & 0xFFFF) +
    ((buffer[2] << 16) & 0xFFFFFF) +
    ((buffer[3] << 24) & 0xFFFFFFFF) +
    ((buffer[4] << 32) & 0xFFFFFFFFFF) +
    ((buffer[5] << 40) & 0xFFFFFFFFFFFF));
}

void EEPROMClass::update48(int16_t address, uint64_t value)
{
	if (read48(address) != value) write48(address, value);
}