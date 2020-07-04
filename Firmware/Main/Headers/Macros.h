
/*

	MACROS HEADER FILE

*/


#ifndef MACROS_H
#define MACROS_H


// EXPRESSIONS
#define NO 																			0
#define YES 																		1

#define METRIC 																		0
#define IMPERIAL 																	1

#define TD_24CLOCK 																	0
#define TD_AMPM 																	1

#define LIGHT 																		0
#define DARK 																		1

#define LEFT 																		0
#define RIGHT 																		1

#define SUB 																		-1
#define ADD 																		1

#define AUTO_SCR_DIM 																2


// UNIT SETTINGS & FUNCTIONS 
#define one 																		10000
#define one_km 																		10000000ULL // 1000m * 10000(one meter)
#define one_mi 																		52800000ULL // 5280ft * 10000(one feet)
#define one_m2ft 																	32808 // 3.2808
#define one_ft2m 																	3048 // 0.3048


#define km2m(value) \
	value * one_km

#define mi2ft(value) \
	value * (float)one_mi

#define kmh2mih(value) \
	value * (6213 / (float)one)

#define mih2kmh(value) \
	value * (16093 / (float)one)

#define cel2far(value) \
	32 + (1.80 * (value))

#define far2cel(value) \
	(value - 32) / 1.80

#define m2ft(value) \
	value * (one_m2ft / (float)one)

#define ft2m(value) \
	value * (one_ft2m / (float)one)


#define m2km(value) \
	value / (float)one_km

#define ft2mi(value) \
	value / (float)one_mi


// MACRO FUNCTIONS
#define TEXT_WHITE() \
	Screen.setTextColor(WHITE, BLACK)

#define TEXT_BLACK() \
	Screen.setTextColor(BLACK, WHITE)

#define SIZE(size) \
	Screen.setTextSize(size)

#define CURSOR(x, y) \
	Screen.setCursor(x, y)

#define UI_ClearIcon(x, y) \
	Screen.fillRect(x, y, ICON_SIZE_X, ICON_SIZE_Y, BLACK)

#define UI_ClearServo() \
	Screen.fillRect(SERVO_LEVEL_POS_X, SERVO_BAR_POS_Y, SERVO_LEVEL_W, SERVO_BAR_H, BLACK)

#define UI_ClearTDT() \
	Screen.fillRect(CLOCK_POS_X_12H, CLOCK_POS_Y, 49, 22, BLACK)

#define UI_ClearInfo() \
	Screen.fillRect(INFO_POS_X, INFO_POS_Y, 88, 16, BLACK)

#define UI_SpeedIcon(unit) \
	Screen.drawBitmap(SPEED_UNIT_POS_X, SPEED_UNIT_POS_Y, speedIcon[unit], SPEED_UNIT_W, SPEED_UNIT_H, WHITE)

#define UI_RPMIcon() \
	Screen.drawBitmap(RPM_UNIT_POS_X, RPM_UNIT_POS_Y, rpmIcon, RPM_UNIT_W, RPM_UNIT_H, WHITE)


#define System_LowBattery() \
	!digitalRead(IND_BATT)


#define DEBUG_VAL(text, value) \
	Serial.print(text); \
	Serial.println(value, DEC)

#define DEBUG(text) \
	Serial.println(text)


#endif // MACROS_H