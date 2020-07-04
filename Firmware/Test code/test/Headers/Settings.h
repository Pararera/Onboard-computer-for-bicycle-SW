
/*

	SETTINGS HEADER FILE

*/

#ifndef SETTINGS_H
#define SETTINGS_H


// LIBRARIES
#include 																			<stdint.h>

// MODULES
#include 																			"DefaultSettings.h"



// SYSTEM SETTINGS & VALUES
#define BTN_SOUND_DURATION 															250 // IN MS
#define DARK_THRESHOLD_VALUE 														2048 // MAX 4095
#define SENSOR_STEP																	50 // MAX 255
#define SETUP_VALUE 																31 // RANDOM NUMBER BETWEEN 1 - 255
#define BRGH_STEP 																	5
#define SHIFT_STEP 																	5			

#define MIN_SCR_SAVER 																10000 // IN MS
#define MAX_SCR_SAVER 																65000 // IN MS
#define MIN_SEN_VALUE 																100
#define MAX_SEN_VALUE 																4000
#define MIN_BRGH_VALUE 																8 // MAX 250
#define MAX_BRGH_VALUE 																250 // MAX 250
#define MIN_LEAN_ANGLE 																15 // IN DEG 
#define MAX_LEAN_ANGLE 																30 // IN DEG
#define MIN_BRAKE_FLASHES 															5
#define MAX_BRAKE_FLASHES															15
#define MIN_SHIFT_IND 																60
#define MAX_SHIFT_IND  																150
#define MIN_HL_ANGLE 																75 // IN DEG   I'M NOT RESPONSIBLE FOR ANY DAMAGE
#define MAX_HL_ANGLE																105 // IN DEG   I'M NOT RESPONSIBLE FOR ANY DAMAGE

#define BRAKE_LIGHT_BRGH 	 														MAX_BRGH_VALUE
#define MAX_AMBIENT_CHECKS 															3


// SETTINGS ID
#define UNIT																		0
#define TD_FORMAT 																	1
#define BATT_SAVER 																	2
#define BATT_SAVER_LVL 																3
#define SCR_DIM 																	4
#define HL_DAILY_BRGH 																5
#define HL_BRGH 																	6
#define HL_HB_BRGH 																	7
#define TL_DAILY_BRGH 																8
#define TL_BRGH 																	9
#define TL_FOG_BRGH 																10
#define SMOOTH_LIGHTS 																11
#define CORNERING_LIGHTS 															12
#define SOUNDS 																		13
#define BTN_SOUND 																	14
#define TS_SOUND 																	15
#define ALERT_SOUND 																16
#define BRAKE_LIGHT 																17
#define TURN_SIGNALS 																18
#define SHIFT_IND 																	19

#define MAX_SETTINGS8 																20


#define SAVER_TIME 																	0
#define FRONT_BRAKE 																1
#define REAR_BRAKE 																	2
#define AUTO_LIGHT 																	3

#define MAX_SETTINGS16 																4



// CLASS
class SettingsClass {
public:
	SettingsClass();
	~SettingsClass();

	void set8(uint8_t, uint8_t);
	void setSetting8(int8_t, uint8_t, uint8_t, uint8_t, bool);
	uint8_t getSetting8(uint8_t) const;

	void set16(uint8_t, uint16_t);
	void setSetting16(int16_t, uint8_t, uint16_t, uint16_t, bool);
	uint16_t getSetting16(uint8_t) const;

	void setHLProfile(uint8_t, uint8_t);
	uint8_t getHLProfile(uint8_t) const;

private:
	uint8_t settings8[MAX_SETTINGS8] = {
		DEF_UNIT,
		DEF_TD_FORMAT,
		DEF_BATT_SAVER,
		DEF_BATT_SAVER_LVL,
		DEF_SCR_DIM,
		DEF_HL_DAILY_BRGH,
		DEF_HL_BRGH,
		DEF_HL_HB_BRGH,
		DEF_TL_DAILY_BRGH,
		DEF_TL_BRGH,
		DEF_TL_FOG_BRGH,
		DEF_SMOOTH_LIGHTS,
		DEF_CORNERING_LIGHTS,
		DEF_SOUNDS,
		DEF_BTN_SOUND,
		DEF_TS_SOUND,
		DEF_ALERT_SOUND,
		DEF_BRAKE_LIGHT,
		DEF_TURN_SIGNALS,
		DEF_SHIFT_IND
	};
	uint16_t settings16[MAX_SETTINGS16] = {
		DEF_SAVER_TIME,
		DEF_FRONT_BRAKE,
		DEF_REAR_BRAKE,
		DEF_AUTO_LIGHT
	};
	uint8_t headLightProfiles[MAX_HL_PROFILES];
};



// OBJECT
extern SettingsClass Settings; 


#endif // SETTINGS_H