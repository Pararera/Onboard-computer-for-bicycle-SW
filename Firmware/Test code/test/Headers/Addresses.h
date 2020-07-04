
/*

	CHIP ADDRESSES & EEPROM MEMORY ADDRESSES

*/

#ifndef _ADDRESSES_H
#define _ADDRESSES_H


// CHIP ADDRESSES
#define AT24CXX																		0x57 // ADDRESS OF EEPROM CHIP
#define DS3231 																		0x68 // ADDRESS OF RTC CHIP
#define SH1106																		0x3C // ADDRESS OF 1.3" OLED DRIVER CHIP (OR 0x3D)


// EEPROM ADDRESSES
#define EEPROM_DISTANCES 															0 // + 60  SIX BYTES PER EACH DISTANCE

#define EEPROM_TIMES 																60 // + 40  FOUR BYTES PER EACH TIME

#define EEPROM_STATS 																100 // + 60  SIX BYTES PER EACH STAT

#define EEPROM_STATES 																160 // + 30  ONE BYTE PER EACH STATE

#define EEPROM_TOUR 																190 // + 30 MIXED BYTES

#define EEPROM_SETTINGS8															220 // + 50  ONE BYTE PER EACH SETTING
#define EEPROM_SETTINGS16 															270 // + 40  TWO BYTES PER EACH SETTING

#define EEPROM_SPEEDS 																310 // + 10  ONE BYTE PER EACH SPEED

#define EEPROM_HL_PROFILES 															320 // + 6  ONE BYTE PER EACH PROFILE

// OTHER EEPROM ADDRESSES
#define EEPROM_DAILY_DIST_DAY 														326
#define EEPROM_MONTHLY_DIST_MONTH 													327
#define EEPROM_WHEELE_EXTENT 														328 // + 4

#define EEPROM_SETUP																400 // DON'T CHANGE IT!


#endif // _ADDRESSES_H