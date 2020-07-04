
/*

	SYSTEM HEADER FILE

*/

#ifndef SYSTEM_H
#define SYSTEM_H


// STATE ARRAY INDEXES
#define SCREEN_STATE 																0
#define LI_STATE 																	1
#define HB_STATE 																	2
#define TS_STATE 																	3
#define FL_STATE 																	4
#define INFO_STATE 																	5
#define HL_PROFILE 																	6

#define MAX_STATES 																	7	


// STATE EXPRESSIONS
#define DAILY_LI 																	1
#define NORMAL_LI 																	2
#define AUTO_LI		 																3

#define SCREEN_OFF 																	0
#define SCREEN_ON 																	1
#define SCREEN_OFF_SYS 																2



// CLASS
class SystemClass {
public:
	SystemClass();
	~SystemClass();

	// FUNCTIONS
	void init(void);
	void btnScan(void);
	void btn(void);
	uint8_t isDark(void) const;
	void dimScreen(void) const;
	void save(void);
	void updateTDT(void);
	void saver(void);
	void brakeLight(void);
	void ambient();

	// SOUNDS
	void playBtnSound(void) const;
	void playTSSound(void) const;
	void playAlertSound(void) const;

	// GET/SET FUNCTIONS
	void setState(uint8_t, uint8_t);
	uint8_t getState(uint8_t) const;
	void setAutoLight(bool);

private:
	// VARIABLES
	uint32_t lastActivity = 0;
	uint32_t lastSave = 0;
	uint32_t lastTDT = 0;
	uint32_t lastBrLiFlash = 0;
	uint32_t lastAmbientCheck = 0;

	bool brakeLightStatus = false;
	bool brakeLiFlash = false;
	bool autoLi = false;
	bool battSaver = false;
	uint8_t ambientChecks = 0;
	uint8_t brakeLightFlashes = 0;

	// FUNCTIONS
	void brakeLightFlash(void);

	// ARRAYS
	uint8_t states[MAX_STATES] = {
		DEF_SCREEN_STATE,
		DEF_LI_STATE,
		DEF_HB_STATE,
		DEF_TS_STATE,
		DEF_FL_STATE,
		0,
		DEF_HL_PROFILE
	};
};



// OBJECT
extern SystemClass System;


#endif // SYSTEM_H