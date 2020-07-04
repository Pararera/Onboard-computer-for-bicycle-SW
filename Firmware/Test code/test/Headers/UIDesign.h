
/*
	
	UI DESIGN HEADER FILE
	*** SETUP LOGO DESIGN IS IN BOOT.CPP

*/


#ifndef UI_DESIGN_H
#define UI_DESIGN_H


// MODULES
#include 																			<Arduino.h>

// MODULES
#include 																			"Macros.h"



// SETTINGS
#define BOOT_ANIM_OFFSET 															12
#define ICON_SIZE_X 																16
#define ICON_SIZE_Y 																16
#define SETTING_IND 																RIGHT // LEFT ; RIGHT
#define SPEED_UNIT_METRIC 															"km/h"
#define SPEED_UNIT_IMPERIAL 														"mi/h"


// UI DESIGN
#define SPEED_POS_X																	0
#define SPEED_POS_Y																	0
/*#define SPEED_UNIT_POS_X															0
#define SPEED_UNIT_POS_Y															11*/
#define SPEED_UNIT_POS_X 															24
#define SPEED_UNIT_POS_Y															0
#define SPEED_UNIT_W 																7
#define SPEED_UNIT_H 																15

#define RPM_POS_X																	92
#define RPM_POS_Y																	0
/*#define RPM_UNIT_POS_X															108
#define RPM_UNIT_POS_Y																10*/
#define RPM_UNIT_POS_X 																83
#define RPM_UNIT_POS_Y																0
#define RPM_UNIT_W 																	7
#define RPM_UNIT_H 																	15

#define DATE_POS_X																	37
#define DATETEMP_POS_Y																10
#define TEMP_POS_X_3DIG																46
#define TEMP_POS_X_4DIG																44

#define CLOCK_POS_X_24H																41
#define CLOCK_POS_X_12H																33
#define CLOCK_POS_Y																	0

#define INFO_POS_X																	0
#define INFO_POS_Y																	48

#define ICON_LIGHT_POS_X 															60
#define ICON_LIGHT_POS_Y 															24

#define ICON_BATTERY_POS_X 															0
#define ICON_BATTERY_POS_Y 															ICON_LIGHT_POS_Y

#define ICON_HIGH_BEAM_POS_X 														90
#define ICON_HIGH_BEAM_POS_Y 														ICON_LIGHT_POS_Y

#define ICON_FOG_LIGHT_POS_X 														30
#define ICON_FOG_LIGHT_POS_Y 														ICON_LIGHT_POS_Y

/*#define ICON_HEATER_POS_X 															ICON_HIGH_BEAM_POS_X
#define ICON_HEATER_POS_Y 															47*/

#define ICON_LEFT_TS_POS_X 															37
#define ICON_LEFT_TS_POS_Y 															0

#define ICON_RIGHT_TS_POS_X 														61
#define ICON_RIGHT_TS_POS_Y 														ICON_LEFT_TS_POS_Y

#define SERVO_BAR_POS_X 															120
#define SERVO_BAR_POS_Y 															ICON_LIGHT_POS_Y
#define SERVO_BAR_W 																4
#define SERVO_BAR_H 																40
#define SERVO_LEVEL_POS_X 															116
#define SERVO_LEVEL_W 																12
#define SERVO_LEVEL_H 																4



// ICON DESIGNS (8 PIXELS PER BYTE)
static const unsigned char PROGMEM icons[][32] = {
	// AUTO LIGHT OFF
	{
		0x03, 0x80, 0x02, 0x60, 0x9A, 0x18, 0x02, 0x04, 0x02, 0x02, 0x9A, 0x01, 0x02, 0x01, 0x02, 0x01,
		0x02, 0x01, 0x02, 0x01, 0x9A, 0x01, 0x02, 0x02, 0x02, 0x04, 0x9A, 0x18, 0x02, 0x60, 0x03, 0x80
	},

	// AUTO LIGHT ON
	{
		0x03, 0x80, 0x02, 0x60, 0xFA, 0x18, 0x02, 0x04, 0x02, 0x02, 0x9A, 0x01, 0x02, 0x01, 0x02, 0x01,
		0x02, 0x01, 0x02, 0x01, 0xFA, 0x01, 0x02, 0x02, 0x02, 0x04, 0x9A, 0x18, 0x02, 0x60, 0x03, 0x80
	},

	// BATTERY
	{
		0x38, 0x1C, 0x38, 0x1C, 0xFF, 0xFF, 0x80, 0x01, 0x80, 0x01, 0x80, 0x09, 0xB8, 0x1D, 0x80, 0x09,
		0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xFF, 0xFF
	},

	// DAILY LIGHT
	{
		0x03, 0x80, 0x82, 0x60, 0x62, 0x18, 0x1A, 0x04, 0x02, 0x02, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01,
		0xFA, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02, 0x02, 0x04, 0x1A, 0x18, 0x62, 0x60, 0x83, 0x80
	},

	// FOG LIGHT
	{
		0x01, 0xC4, 0x06, 0x44, 0x18, 0x5F, 0x20, 0x44, 0x40, 0x44, 0x80, 0x5F, 0x80, 0x44, 0x80, 0x44,
		0x80, 0x44, 0x80, 0x44, 0x80, 0x5F, 0x40, 0x44, 0x20, 0x44, 0x18, 0x5F, 0x06, 0x44, 0x01, 0xC4
	},

	// HIGH BEAM
	{
		0x03, 0x80, 0x02, 0x60, 0xFA, 0x18, 0x02, 0x04, 0x02, 0x02, 0xFA, 0x01, 0x02, 0x01, 0x02, 0x01,
		0x02, 0x01, 0x02, 0x01, 0xFA, 0x01, 0x02, 0x02, 0x02, 0x04, 0xFA, 0x18, 0x02, 0x60, 0x03, 0x80
	},

	// LIGHT
	{
		0x03, 0x80, 0x02, 0x60, 0x1A, 0x18, 0x62, 0x04, 0x82, 0x02, 0x02, 0x01, 0x02, 0x01, 0x1A, 0x01,
		0x62, 0x01, 0x82, 0x01, 0x02, 0x01, 0x02, 0x02, 0x1A, 0x04, 0x62, 0x18, 0x82, 0x60, 0x03, 0x80
	},

	// LEFT TURN SIGNAL
	{
		0x03, 0x00, 0x07, 0x00, 0x0D, 0x00, 0x19, 0x00, 0x31, 0x00, 0x61, 0xFF, 0xC0, 0x01, 0x80, 0x01,
		0x80, 0x01, 0xC0, 0x01, 0x61, 0xFF, 0x31, 0x00, 0x19, 0x00, 0x0D, 0x00, 0x07, 0x00, 0x03, 0x00
	},

 	// RIGHT TURN SIGNAL
	{
		0x00, 0xC0, 0x00, 0xE0, 0x00, 0xB0, 0x00, 0x98, 0x00, 0x8C, 0xFF, 0x86, 0x80, 0x03, 0x80, 0x01,
		0x80, 0x01, 0x80, 0x03, 0xFF, 0x86, 0x00, 0x8C, 0x00, 0x98, 0x00, 0xB0, 0x00, 0xE0, 0x00, 0xC0
	}
};

static const unsigned char PROGMEM rpmIcon[] = {
	0xFE, 0x32, 0x52, 0x9E, 0x00, 0xFE, 0x12, 0x12, 0x1E, 0x00, 0xFE, 0x04, 0x08, 0x04, 0xFE, 0x00
};

static const unsigned char PROGMEM speedIcon[][16] = {
	{ 0xFE, 0x18, 0x24, 0xC2, 0x00, 0xFE, 0x04, 0x08, 0x04, 0xFE, 0x00, 0xFE, 0x10, 0x10, 0xFE, 0x00 }, // KM/H
	{ 0xFE, 0x04, 0x08, 0x04, 0xFE, 0xFE, 0x12, 0x12, 0x12, 0x1E, 0x00, 0xFE, 0x10, 0x10, 0xFE, 0x00 } // MI/H
};

#endif // UI_DESIGN_H