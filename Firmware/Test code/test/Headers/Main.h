
/*
	
	MAIN HEADER FILE

*/


#ifndef MAIN_H
#define MAIN_H


// LIBRARIES
#include 																			<stdint.h>
#include 																			<Servo.h>
#include                                                                       		<RTClib.h>

// MODULES
#include 																			"EEPROM.h"
#include 																			"Accelerometer.h"



// INFO IDS
#define INFO_TOTAL_DIST 															0




// ARRAY DECLARATIONS
#define ENTER			                                                         	0 // ENTER BUTTON
#define BACK		                                                         		1 // BACK BUTTON
#define LTS		                                                             		2 // LETF TURN SIGNAL BUTTON
#define RTS		                                                            		3 // RIGHT TURN SIGNAL BUTTON
#define LI		                                                              		4 // LIGHTS BUTTON
#define HB			                                                          		5 // HIGH BEAM BUTTON  ALWAYS LAST!
extern int32_t btnPressedTick[6];

#define TOTAL_DISTANCE 																0
#define TRIP_DISTANCE 																1
#define DAILY_DISTANCE 																2
#define MONTHLY_DISTANCE 															3
#define MAX_DISTANCES 																4
extern volatile uint64_t distances[MAX_DISTANCES];

#define AVG_DIST_TIME 																0
#define AVG_SPEED_TIME 																1
#define TOTAL_TIME 																	2
#define TRIP_TIME 																	3
#define WORK_TIME 																	4
#define MAX_TIMES 																	5
extern uint32_t times[MAX_TIMES];

#define TOP_SPEED 																	0
#define MAX_SPEED 																	1
#define MAX_SPEEDS																	2
extern uint8_t speeds[MAX_SPEEDS];

#define MAX_SOUNDS 																	16
extern uint16_t sounds[MAX_SOUNDS];

#define AVG_DIST 																	0
#define AVG_SPEED 																	1
#define MAX_STATS 																	2
extern uint64_t stats[MAX_STATS];


// VARIABLE DECLARATIONS
extern volatile uint32_t wheeleExtent;



// OBJECTS
extern RTC_DS3231 RTC;
extern EEPROMClass EEPROM;
extern AccelClass Accelerometer;
extern Servo HLServo;



// FUNCTION DECLARATIONS
void number2Digits(uint64_t, uint8_t*, uint8_t);


#endif // MAIN_H