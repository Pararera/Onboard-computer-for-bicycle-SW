
/*

	DEFAULT SETTINGS HEADER FILE

*/

#ifndef DEFAULT_SETTINGS_H
#define DEFAULT_SETTINGS_H


// LIBRARIES
#include 																			<Arduino.h>

// MODULES
#include 																			"Macros.h"



// DEFAULT SETTINGS
#define DEF_UNIT 																	METRIC
#define DEF_TD_FORMAT 																TD_24CLOCK
#define DEF_BATT_SAVER 																YES
#define DEF_BATT_SAVER_LVL															HIGH
#define DEF_SCR_DIM 																AUTO_SCR_DIM
#define DEF_HL_DAILY_BRGH 															32
#define DEF_HL_BRGH 																128
#define DEF_HL_HB_BRGH 																250
#define DEF_TL_DAILY_BRGH 															32
#define DEF_TL_BRGH 																128
#define DEF_TL_FOG_BRGH 															250
#define DEF_SMOOTH_LIGHTS															YES
#define DEF_CORNERING_LIGHTS														15 // 15-30 ; NO = OFF
#define DEF_SOUNDS 																	YES
#define DEF_BTN_SOUND 																1
#define DEF_TS_SOUND 																2
#define DEF_ALERT_SOUND 															3
#define DEF_BRAKE_LIGHT 															10
#define DEF_TURN_SIGNALS 															YES
#define DEF_SHIFT_IND 																90 // 40-255 ; NO = OFF

#define DEF_SAVER_TIME 																30000 // IN MS
#define DEF_FRONT_BRAKE 															2048
#define DEF_REAR_BRAKE 																2048
#define DEF_AUTO_LIGHT 																2048

#define DEF_WHEELE_EXTENT 															23546 // 2.3546 -> 23546  IN METERS OR FEETS. MIN 10000 MAX 99999


// DEFAULT BUTTONS (NAMES FOR btnPressedTick ARRAY - MAIN.H FILE)   *** HIGH BEAM BUTTON IS NOT IN USE! ***
#define SETUP_EXIT_BTN 																BTN_LIGHTS // NAME FROM PINS.H FILE
#define SETUP_BACKWARD_BTN 															BACK
#define SETUP_FORWARD_BTN 															ENTER
#define SETUP_SUB_VALUE_BTN 														LTS
#define SETUP_ADD_VALUE_BTN 														RTS

#define SETTINGS_ENTER_BTN 															ENTER // HOLD
#define SETTINGS_ENTER_MENU_BTN 													ENTER // HOLD
#define SETTINGS_EXIT_MENU_BTN 														BACK // HOLD
#define SETTINGS_RST_TO_DEF_BTN 													ENTER // HOLD
#define SETTINGS_BACKWARD_BTN 														BACK
#define SETTINGS_FORWARD_BTN 														ENTER
#define SETTINGS_SUB_VALUE_BTN 														LTS
#define SETTINGS_ADD_VALUE_BTN 														RTS
#define SETTINGS_EXIT_BTN 															BTN_LIGHTS // NAME FROM PINS.H FILE

// TIME SETTINGS
#define SAVE_TIME 																	20000 // IN MS
#define CLOCK_UPDATE 																10000 // IN MS
#define BRAKE_FLASH_TIME 															50 // IN MS
#define AMBIENT_CHECK 																10000 // IN MS

// BUTTON DURATION TIMES
#define BTN_PRESS 																	40 // IN MS
#define BTN_HOLD 																	250 // IN MS
#define BTN_LONG 																	600 // IN MS

// STATE DEFAULT VALUES
#define DEF_SCREEN_STATE 															YES
#define DEF_LI_STATE 																NO
#define DEF_HB_STATE 																NO
#define DEF_TS_STATE 																NO
#define DEF_FL_STATE 																NO
#define DEF_HL_PROFILE 																0

// SOUNDS SETTINGS - JUST COMMENT THE LINE TO TURN OFF SOUND
#define SOUND_BTN
#define SOUND_BATT_SAVER
#define SOUND_SAVER_ON
#define SOUND_SAVER_OFF

// OTHER SETTINGS
#define MAX_HL_PROFILES 															4 // MAX 6


#endif // DEFAULT_SETTINGS_H