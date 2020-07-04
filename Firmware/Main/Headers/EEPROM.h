
/*

	EEPROM FUNCTIONS
	AT24C32 AND AT24C64 CHIP
    LAST UPDATE: 11. 4. 2019.

*/


#ifndef _EEPROM_H
#define _EEPROM_H


// LIBRARIES
#include 																			<Arduino.h>
#include 																			<Wire.h>

// MODULES
#include 																			"Addresses.h"



// SETTINGS
/*
	Delay after writing data onto EEPROM chip
	@values: 2-10
	@unit: ms
*/
#define EEPROM_DELAY 																5

/*
	Page size of EEPROM chip
	@values: 32/64 (depends on chip you have)
*/
#define EEPROM_PAGE         														32

/*
	Buffer size of EEPROM chip
	Two bytes are reserved for address
*/
#define EEPROM_BUFFER																(EEPROM_PAGE - 2)

/*
	Buffer size for read operations
	Same size as buffer from Wire library
*/
#define EERPOM_READ_BUFFER 															BUFFER_LENGTH



// CLASS
class EEPROMClass {
public:
	// OBJECT CONTRUCTOR
	EEPROMClass();

	/*	
		Writes multiple bytes in one go. Maximum 32 bytes. 
		@params:
			- adress: Address in EEPROM where data will be written
			- data: Data to write in EEPROM
			- len: Size of @data in bytes
	*/
	void writePage(uint16_t const, char*, uint8_t);

	/*
		Writes @len-byte @value in EEPROM at @address		
		@params:
			- adress: EEPROM address of first byte
			- value: Value to write in EEPROM
			- len: Size of @data (maximum 5 for 6-byte data and 4 for 8-byte data)
	*/
	void write(uint16_t, uint64_t const, uint8_t const);

	/*	
		Reads @len-byte value from @address in EEPROM
		If enabled, it will write only bytes that are different from previous value	
		@params:
			- adress: Data address in EEPROM
			- len: Size of @data (maximum 5 for 6-byte data and 4 for 8-byte data)
		@return:
			- Data from @address in EEPROM
	*/
	uint64_t read(uint16_t const, uint8_t const);


	/*	
		Writes @len-byte @value at given @address only if current value at @address is different from @value	
		@params:
			- adress: Address in EEPROM where data will be written
			- value: Value to write in EEPROM
			- len: Size of @data (maximum 5 for 6-byte data and 4 for 8-byte data)
	*/
	void update(uint16_t const, uint64_t const, uint8_t const);


private:
	/*
		Writes buffer into EEPROM chip
		@params:
			- address: EEPROM address of first byte
			- data: Data to write in EEPROM
			- len: Size of @data in bytes
	*/
	void writeBuffer(uint16_t, char*, uint8_t);

	/*

	*/
	void writeBytes(uint16_t, char*, uint8_t const);
};



// OBJECT
extern EEPROMClass EEPROM;


#endif // _EEPROM_H