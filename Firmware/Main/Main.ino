
/*

    - 	Credits: Pararera
    - 	Current version: v1.0
    - 	Last update: XX.XX.XXXX.

    - 	Default unit: Metric
    - 	Default time format: hh:mm (0-23)
    - 	Default date format: dddd.MM.

    - 	Contact e-Mail address: pararera@outlook.com
    - 	Instagram: pararera_



    ** CHANGE LOG

        v1.0:
            -	Initial release  



	** NOTES

		UNITS:
			- 	If you try to get how unit system works ask God. I really don't know anymore!
			-	One(1) meter[m]/feet[ft] is 10 000 in memory. 
			-	Maximum distance is 28 147 497km or 5 330 965mi.
			-	Within conversion from imperial to metric system, you'll lose ~6 meters/~20 feets(loss ~0.02%).
				Btw imperial system is really weird! Thanks Americans :D

		DISPLAY:
			-	Default font size: 1
			-	Default text color: WHITE text on BLACK background

*/


/*

	I2C 2
	TwoWire WIRE2 (2,I2C_FAST_MODE);
	#define Wire WIRE2

*/


// LIBRARIES
#include                                        									<Adafruit_GFX.h> 
#include                                                                       		<Servo.h>

// MODULES
#include 																			"Headers\Main.h"
#include 																			"Headers\Addresses.h"
#include 																			"Headers\Macros.h"
#include 																			"Headers\Pins.h"
#include 																			"Headers\UIDesign.h"

#include 																			"Headers\Boot.h"
#include 																			"Headers\UI.h"
#include 																			"Headers\Settings.h"
#include 																			"Headers\System.h"
#include 																			"Headers\Tour.h"



// OBJECTS
HardwareTimer timer(4);
Servo HLServo;
RTC_DS3231 RTC;



// ARRAYS
int32_t btnPressedTick[6] = {
    0, // ENTER
    0, // BACK
    0, // LTS
    0, // RTS
    0, // LI
    0 // HB - ALWAYS LAST
};

uint16_t sounds[MAX_SOUNDS] = {
	200,
	600,
	1000,
	1500,
	1900,
	2350,
	2750,
	3100,
	3600,
	3800,
	4150,
	4600,
	5000,
	5400,
	5750,
	6100
};

volatile uint64_t distances[MAX_DISTANCES] = {
	0,
	0,
	0,
	0
};

uint32_t times[MAX_TIMES] = {
	0,
	0,
	0,
	0,
	0
};

uint8_t speeds[MAX_SPEEDS] = {
	0,
	0
};

uint64_t stats[MAX_STATS] = {
	0,
	0
};



// VARIABLES
volatile uint32_t wheeleExtent 								=						DEF_WHEELE_EXTENT;



// INTERRUPT FUNCTIONS
void wheeleRev(void)
{

}


void rev(void)
{

}



// MAIN FUNCTIONS
void setup(void)
{
	delay(100); // DON'T DELETE

	// BOOT & PIN SETUP
	BootClass Boot;
	attachInterrupt(digitalPinToInterrupt(SEN_SPEED), wheeleRev, RISING);
	attachInterrupt(digitalPinToInterrupt(SEN_RPM), rev, RISING);
	Serial.begin(9600);	

	// HARDWARE INIT (SCREEN MUST BE FIRST!!)
    Screen.begin(SH1106_SWITCHCAPVCC, SH1106);
    Screen.clearDisplay();

	HLServo.attach(CONT_SERVO);
	timer.setPrescaleFactor(1);
	timer.setOverflow(255); // SETS PWM FREQ. TO 282kHz (TIMER 4)
	Accelerometer.init();
	EEPROM.read8(0); // DON'T DELETE

	// FILLS HEADLIGHT PROFILES
	for (uint8_t i = 0; i < MAX_HL_PROFILES; i++) Settings.setHLProfile(i, 90);

	// LOADS STUFF FROM EEPROM
	Boot.load();

	// DIMS THE SCREEN
	Screen.dim(System.isDark());

	// PLAYS BOOT ANIM
	Boot.playAnim();

	// SETUP SCREEN
	if (Boot.getSetup() != SETUP_VALUE)
	{
		// WRITES DEFAULT VALUES INTO EEPROM
		Boot.setDefaultValues();

		// SHOWS SETUP
		Boot.initSetup(SETUP_SCREEN);
		Boot.showSetup();
		
		while (digitalRead(SETUP_EXIT_BTN) != LOW)
		{
			System.btnScan();
			Boot.btnSetup();

			delay(50); // DON'T DELETE
		}

		// UPDATES EEPROM WITH NEW VALUES
		Boot.update(true);
		Screen.clearDisplay();
	}
	else Boot.resetDistances(); // AUTO RESETS DAILY & MONTHLY DISTANCES

	// ABILITY TO GO INTO SETTING MENU
	uint32_t senUpdate = millis();
	uint32_t clockUpdate = millis();

	// CHECKS SCREEN DIM
	System.dimScreen();

	// DISPLAYS RIDE SCREEN
	Boot.rideScreen(true);

	// CHECKS SCREEN STATE
	if (System.getState(SCREEN_STATE) == SCREEN_OFF)
	{
		Screen.SH1106_command(SH1106_DISPLAYOFF);
		goto WITHOUT_SETTINGS;
	}
	else
	{
		System.setState(SCREEN_STATE, SCREEN_ON);
		Screen.display();
	}

	// SETTINGS LOOP
	delay(1500); // PROTECTION AGAINST SKIPPING SETTINGS
	while (digitalRead(SETTINGS_EXIT_BTN) != LOW)
	{
		System.btnScan();
		Boot.btnSettings();

		// UPDATES SENSOR VALUES
		if (Boot.getSettingMenu() == 3 && millis() - senUpdate > 250)
		{
			TEXT_WHITE();
			CURSOR(95, 30);
			Screen.print(analogRead(SEN_FRONT_BR));
			Screen.print(' ');

			CURSOR(95, 40);
			Screen.print(analogRead(SEN_REAR_BR));
			Screen.print(' ');

			CURSOR(95, 50);
			Screen.print(analogRead(SEN_LIGHT));
			Screen.print(' ');

			senUpdate = millis();
			Screen.display();
		}

		// UPDATES CLOCK, DATE AND TEMP
		if (Boot.getScreen() == RIDE_SCREEN && millis() - clockUpdate > CLOCK_UPDATE)
		{
			UI.updateTDT();
			Screen.display();
			clockUpdate = millis();
		}			

		delay(50); // DON'T DELETE
	}

	// UPDATES ALL SETTINGS & RTC MODULE (IF NEEDED)
	Boot.update(false);

	// DIMS SCREEN (AGAIN)
	System.dimScreen();

	// DISPLAYS RIDE SCREEN (AGAIN)
	WITHOUT_SETTINGS:
	Screen.clearDisplay();
	Boot.rideScreen(false);
	Screen.display();

	// SYSTEM INIT
	System.init();
}

void loop(void)
{
	// BUTTON SCAN
	System.btnScan();
	System.btn();

	// BRAKE LIGHT
	System.brakeLight();

	// CHECKS FOR BATTERY & SCREEN SAVER
	System.saver();

	// CHECKS AMBIENT LIGHT
	System.ambient();

	// UPDATES CLOCK, DATE & TEMP
	System.updateTDT();

	// SAVES VALUES IN EEPROM
	System.save();

	// REFRESHES THE SCREEN
	UI.refresh();
}



// OTHER FUNCTIONS
void number2Digits(uint64_t number, uint8_t *output, uint8_t digits)
{
	do
	{
		output[digits - 1] = number % 10;
		number /= 10;
		digits--;		
	}
	while (digits != 0);
}
