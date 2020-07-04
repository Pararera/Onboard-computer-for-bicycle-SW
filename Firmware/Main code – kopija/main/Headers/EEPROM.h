
/*

	EEPROM HEADER FILE

*/


#ifndef EEPROM_H
#define EEPROM_H


// LIBRARIES
#include 																			<Arduino.h>
#include 																			<Wire.h>

// MODULES
#include 																			"Addresses.h"



// SETTINGS
#define EEPROM_DELAY 																2 // IN MS



// CLASS
class EEPROMClass {
public:
	void write8(int16_t const, uint8_t const);
	uint8_t read8(int16_t const);
	void update8(int16_t const, uint8_t const);

	void write16(int16_t const, uint16_t const);
	uint16_t read16(int16_t const);
	void update16(int16_t const, uint16_t const);

	void write32(int16_t const, uint32_t const);
	uint32_t read32(int16_t const);
	void update32(int16_t const, uint32_t const);

	void write48(int16_t const, uint64_t const);
	uint64_t read48(int16_t const);
	void update48(int16_t const, uint64_t const);

private:
	int8_t highAddressByte(int16_t);
	int8_t lowAddressByte(int16_t);
};



// OBJECT
extern EEPROMClass EEPROM;


#endif // EEPROM_H