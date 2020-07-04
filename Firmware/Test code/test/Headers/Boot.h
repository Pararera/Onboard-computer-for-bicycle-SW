
/*

	BOOT HEADER FILE

*/

#ifndef BOOT_H
#define BOOT_H


// LIBRARIES
#include                                            								<Arduino.h>

// MODULES
#include                                            								"Pins.h"
#include                                            								"Settings.h"



// EXPRESSIONS
#define SETUP_SCREEN 																0
#define SETTINGS_SCREEN 															1
#define RIDE_SCREEN 																2



// CLASS
class BootClass {
public:
	// (DE)CONSTRUCTOR
	BootClass();
	~BootClass();

	// FUNCTIONS
	void load(void);
	void playAnim(void) const;
	void resetDistances(void) const;
	void rideScreen(bool) const;
	void update(bool);
	
	// SETUP FUNCTIONS
	void btnSetup(void);
	void initSetup(uint8_t);
	void showSetup(void);
	void setDefaultValues(void) const;

	// SETTINGS FUNCTIONS
	void btnSettings(void);
	

	// GET/SET FUNCTIONS
	uint8_t getScreen(void) const;

	uint8_t getSetup(void) const;
	uint8_t getSettingMenu(void) const;

private:
	// VARIABLES
	uint8_t screen;
	uint8_t setup;
	uint8_t selectedItem = 0;
	uint8_t selectedMenu = 0;
	uint8_t inMenu = 0;
	bool updateRTC = false;
	bool ampm = true;
	uint8_t time[2] = { 12, 0 }; // HOUR, MINUTE
	uint8_t date[3] = { 31, 5, 19 }; // DAY, MONTH, YEAR - 2000
	uint8_t dailyDistanceDay = 1;
	uint8_t monthlyDistanceMonth = 1;
	uint8_t sensorStep = SENSOR_STEP;
	uint8_t oldUnit;

	// FUNCTIONS
	void printItem(uint8_t, char*) const;
	void generalSettings(void);
	void showSettingMenu(void);
	void showMenus(void);
	void printMenuTitle(char*) const;
	bool selected(uint8_t&, uint8_t);
	void generalSettingValue(int8_t);
	void wheeleExtentValue(int8_t) const;
	void distanceValue(int8_t) const;
	void tripTimeValue(int8_t) const;
	void topSpeedValue(int8_t) const;
	void printHLprofiles(bool);
	void batterySaverValue(int8_t) const;
	void sensorsValue(int8_t);
	void lightsValue(int8_t) const;
	void soundsValue(int8_t) const;
	void indicatorsValue(int8_t) const;
	void HLProfilesValue(int8_t) const;
	void updateLights(uint8_t) const;

	// PINS & MODES
	const uint8_t pins[22] = {
		BTN_ENTER,
		BTN_BACK,
		BTN_LIGHTS,
		BTN_HIGH_BEAM,
		BTN_LEFT_TS,
		BTN_RIGHT_TS,

		LED_HEAD,
		LED_HB_LEFT,
		LED_HB_RIGHT,
		LED_TAIL1,
		LED_TAIL2,

		IND_BATT,
		IND_LEFT_TS,
		IND_RIGHT_TS,
		IND_RPM_LED,
		IND_SPEAKER,

		SEN_SPEED,
		SEN_RPM,
		SEN_FRONT_BR,
		SEN_REAR_BR,
		SEN_LIGHT,

		CONT_SERVO
	};

	const WiringPinMode modes[22] = {
		BTN_ENTER_MODE,
		BTN_BACK_MODE,
		BTN_LIGHTS_MODE,
		BTN_HIGH_BEAM_MODE,
		BTN_LEFT_TS_MODE,
		BTN_RIGHT_TS_MODE,

		LED_HEAD_MODE,
		LED_HB_LEFT_MODE,
		LED_HB_RIGHT_MODE,
		LED_TAIL1_MODE,
		LED_TAIL2_MODE,

		IND_BATT_MODE,
		IND_LEFT_TS_MODE,
		IND_RIGHT_TS_MODE,
		IND_RPM_LED_MODE,
		IND_SPEAKER_MODE,

		SEN_SPEED_MODE,
		SEN_RPM_MODE,
		SEN_FRONT_BR_MODE,
		SEN_REAR_BR_MODE,
		SEN_LIGHT_MODE,

		CONT_SERVO_MODE
	};
};


#endif // BOOT_H