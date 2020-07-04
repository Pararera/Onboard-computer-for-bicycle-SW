
/*

	USER INTERFACE HEADER FILE

*/

#ifndef UI_H
#define UI_H


// LIBRARIES
#include                                        									<Adafruit_SH1106_STM32.h>

// MODULES
#include                                        									"UIDesign.h"



// EXPRESSIONS
#define DT_TEMP 	 																false
#define DT_DATE 																	true



// ICON IDS
#define ICON_AUTO_LI_OFF 															0
#define ICON_AUTO_LI_ON 															1
#define ICON_BATTERY 																2
#define ICON_DAILY_LI 																3
#define ICON_FOG_LI 																4
#define ICON_HIGH_BEAM 																5
#define ICON_LI 																	6
#define ICON_LEFT_TS 																7
#define ICON_RIGHT_TS 																8



// CLASS
class UIClass {
public:
	// (DE)CONSTRUCTOR
	UIClass();
	~UIClass();

	// FUNCTIONS
	void leadingZero(uint32_t const, uint8_t const, bool) const;
	void icon(uint8_t, bool, bool);
	void updateTDT(void);
	void speed(uint8_t);
	void rpm(uint8_t);
	void showInfo(uint8_t);
	void refresh(void);
	void showServo(uint8_t);

private:
	// FUNCTIONS
	void showDistance(uint8_t) const;
	void showStat(uint8_t) const;
	void showSpeed(uint8_t) const;
	void showTime(uint8_t) const;

	// VARIABLES
	bool update = false;
	bool DT = DT_DATE;
	bool changeTDT = true;

	// ARRAYS
	uint8_t const iconPositions[2][9] = {
		// X AXIS
		{
			ICON_LIGHT_POS_X,
			ICON_LIGHT_POS_X,
			ICON_BATTERY_POS_X,
			ICON_LIGHT_POS_X,
			ICON_FOG_LIGHT_POS_X,
			ICON_HIGH_BEAM_POS_X,
			ICON_LIGHT_POS_X,
			ICON_LEFT_TS_POS_X,
			ICON_RIGHT_TS_POS_X
		},

		// Y AXIS
		{
			ICON_LIGHT_POS_Y,
			ICON_LIGHT_POS_Y,
			ICON_BATTERY_POS_Y,
			ICON_LIGHT_POS_Y,
			ICON_FOG_LIGHT_POS_Y,
			ICON_HIGH_BEAM_POS_Y,
			ICON_LIGHT_POS_Y,
			ICON_LEFT_TS_POS_Y,
			ICON_RIGHT_TS_POS_Y
		}
	};
};



// OBJECTS
extern Adafruit_SH1106 Screen;
extern UIClass UI;


#endif // UI_H