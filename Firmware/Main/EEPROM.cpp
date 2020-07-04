
/*

	EEPROM FUNCTIONS
	AT24C32 AND AT24C64 CHIP
    LAST UPDATE: 11. 4. 2019.

    CREDITS:
    Some functions are taken from Jlesch's library and adjusted.
    www.github.com/jlesech/Eeprom24C32_64

*/


// MODULES
#include 																			"Headers\EEPROM.h"
#include 																			<Arduino.h>
#include 																			<Wire.h>



// OBJECT
EEPROMClass EEPROM;


// OBJECT CONSTRUCTOR
EEPROMClass::EEPROMClass()
{
	
}

// WRITE FUNCTION
void EEPROMClass::write(uint16_t address, uint64_t const value, uint8_t const len)
{  
	uint8_t val = 0;

    Wire.beginTransmission(AT24CXX);
	Wire.write(address >> 8);
	Wire.write(address & 0xFF);

    for (uint8_t i = 0; i < len; i++)
    {
    	val = ((value >> 8 * i) & 0xFF);
    	Wire.write(val);
    }

    Wire.endTransmission();
    delay(EEPROM_DELAY); // DON'T DELETE
}

// PAGE WRITE FUNCTION
void EEPROMClass::writePage(uint16_t address, char *data, uint8_t len)
{

	uint8_t excessLen = 0;
	uint8_t pageOffset = address % EEPROM_PAGE;
	uint8_t pages = 0;

	if (pageOffset > 0) // PAGE OFFSET EXISTS
	{
		excessLen = EEPROM_PAGE - pageOffset;
		if (len < excessLen) excessLen = len;
		len -= excessLen;
		writeBytes(address, data, excessLen);
	}

	if (len > 0)
	{
		uint8_t onPage = 0;
		address += excessLen;
		data += excessLen;
		pages = len / EEPROM_PAGE;

		while (onPage < pages)
		{
			writeBytes(address, data, EEPROM_PAGE);

			address += EEPROM_PAGE;
			data += EEPROM_PAGE;
			len -= EEPROM_PAGE;
			onPage++;
		}

		if (len > 0) writeBuffer(address, data, len); // JUST TO MAKE SURE THERE IS NO UNWRITTEN DATA LEFT
	}
}


// BUFFER WRITE FUNCTION
void EEPROMClass::writeBuffer(uint16_t address, char *data, uint8_t const len)
{
	uint8_t i = 0;

	Wire.beginTransmission(AT24CXX);
	Wire.write(address >> 8);
	Wire.write(address & 0xFF);

	do
	{
		Wire.write(data[i]);
		i++;
	}
	while (i < len);

    Wire.endTransmission();
    delay(EEPROM_DELAY); // DON'T DELETE	
}


// WRITE BYTES FUNCTION
void EEPROMClass::writeBytes(uint16_t address, char *data, uint8_t const len)
{
	uint8_t pages = len / EEPROM_BUFFER;
	uint8_t onPage = 0;
	uint8_t writeOffset = 0;
	uint8_t leftBytes = len % EEPROM_BUFFER;

	do
	{
		writeOffset = onPage * EEPROM_BUFFER;
		onPage++;

		writeBuffer(address + writeOffset, data + writeOffset, EEPROM_BUFFER);
	}
	while (onPage < pages);

	writeOffset = len - leftBytes;
	writeBuffer(address + writeOffset, data + writeOffset, leftBytes);
}

// READ FUNCTION
uint64_t EEPROMClass::read(uint16_t const address, uint8_t const len)
{
	uint64_t value = 0;

	Wire.beginTransmission(AT24CXX);
	Wire.write(address >> 8);
	Wire.write(address & 0xFF);
    Wire.endTransmission();
	Wire.requestFrom(AT24CXX, len);

    for (uint8_t i = 0; i < len; i++)
    {
        if (Wire.available()) value += ((Wire.read() << i * 8) & static_cast<uint64_t>(pow(2, 8 * (i + 1)) - 1));
    }

	return (value);
}


// BUFFER READ FUNCTION



// UPDATE FUNCTION
void EEPROMClass::update(uint16_t const address, uint64_t const value, uint8_t const len)
{
	if (read(address, len) != value) write(address, value, len);
}