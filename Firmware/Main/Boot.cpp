
/*

	BOOT FUNCTIONS

*/


// LIBRARIES
#include 																			<Arduino.h>
#include 																			<RTClib.h>

// MODULES
#include                                                      						"Headers\Boot.h"
#include 																			"Headers\Addresses.h"
#include 																			"Headers\Settings.h"
#include 																			"Headers\UI.h"
#include 																			"Headers\Main.h"
#include 																			"Headers\Macros.h"
#include 																			"Headers\System.h"



// (DE)CONSTRUCOTR
BootClass::BootClass()
{
	// SETS PIN MODES
	for (uint8_t i = 0; i < sizeof(pins); i++) pinMode(pins[i], modes[i]);
	noTone(IND_SPEAKER);

	// DEFAULT VALUES
	screen = RIDE_SCREEN;
	setup = 0;
}

BootClass::~BootClass()
{

}


// FUNCTIONS
void BootClass::load(void)
{
	// SCREEN MESSAGE
	CURSOR(0, 0);
	TEXT_WHITE();
	SIZE(1);
	Screen.print("LOADING...");
	Screen.display();

	setup = EEPROM.read8(EEPROM_SETUP);
	if (setup == SETUP_VALUE)
	{
		// DISTANCES
		for (uint8_t i = 0; i < MAX_DISTANCES; i++) distances[i] = EEPROM.read48(EEPROM_DISTANCES + (i * 6));

		// TIMES
		for (uint8_t i = 0; i < MAX_TIMES; i++) times[i] = EEPROM.read32(EEPROM_TIMES + (i * 4));

		// SETTINGS
		for (uint8_t i = 0; i < MAX_SETTINGS8; i++) Settings.set8(i, EEPROM.read8(EEPROM_SETTINGS8 + i));
		for (uint8_t i = 0; i < MAX_SETTINGS16; i++) Settings.set16(i, EEPROM.read16(EEPROM_SETTINGS16 + (i * 2)));

		// SPEEDS
		for (uint8_t i = 0; i < MAX_SPEEDS; i++) speeds[i] = EEPROM.read8(EEPROM_SPEEDS + i);

		// STATES
		System.setState(SCREEN_STATE, EEPROM.read8(EEPROM_STATES + SCREEN_STATE));
		System.setState(LI_STATE, EEPROM.read8(EEPROM_STATES + LI_STATE));
		System.setState(INFO_STATE, EEPROM.read8(EEPROM_STATES + INFO_STATE));

		// STATS
		for (uint8_t i = 0; i < MAX_STATS; i++) stats[i] = EEPROM.read48(EEPROM_STATS + (i * 6));

		// HEADLIGHT PROFILES
		for (uint8_t i = 0; i < MAX_HL_PROFILES; i++) Settings.setHLProfile(i, EEPROM.read8(EEPROM_HL_PROFILES + i));

		// OTHER SETTINGS & VALUES
		dailyDistanceDay = EEPROM.read8(EEPROM_DAILY_DIST_DAY);
		monthlyDistanceMonth = EEPROM.read8(EEPROM_MONTHLY_DIST_MONTH);
		wheeleExtent = EEPROM.read32(EEPROM_WHEELE_EXTENT);
	}

	Screen.clearDisplay();
}

void BootClass::playAnim(void) const
{
	Screen.clearDisplay();

    for (uint8_t i = 0; i < 65; i += BOOT_ANIM_OFFSET)
    {
    	Screen.drawRect(64 - i, 32 - (i / 2), i * 2, i, WHITE);
    	Screen.display();
    }

    for (uint8_t i = 0; i < 65; i += BOOT_ANIM_OFFSET)
    {
    	Screen.drawRect(64 - i, 32 - (i / 2), i * 2, i, BLACK);
    	Screen.display();
    }   

    Screen.clearDisplay();	
}

bool BootClass::selected(uint8_t &item, uint8_t ID)
{
	if (item == ID)
	{
		TEXT_BLACK();
		return (true);
	}
	else
	{
		TEXT_WHITE();
		return (false);
	}
}

void BootClass::resetDistances(void) const
{
	DateTime time = RTC.now();
	if (time.day() != dailyDistanceDay)
	{
		distances[DAILY_DISTANCE] = 0;
		EEPROM.write32(EEPROM_DAILY_DIST_DAY, 0);
		EEPROM.write8(EEPROM_DAILY_DIST_DAY, time.day());
		EEPROM.write48(EEPROM_DISTANCES + (6 * DAILY_DISTANCE), 0);
	}

	if (time.month() != monthlyDistanceMonth)
	{
		distances[MONTHLY_DISTANCE] = 0;
		EEPROM.write32(EEPROM_MONTHLY_DIST_MONTH, 0);
		EEPROM.write8(EEPROM_MONTHLY_DIST_MONTH, time.month());
		EEPROM.write48(EEPROM_DISTANCES + (6 * MONTHLY_DISTANCE), 0);
	}
}

void BootClass::rideScreen(bool falseIcons) const
{
	TEXT_WHITE();
	UI.updateTDT();
	UI.speed(0);
	UI_SpeedIcon(Settings.getSetting8(UNIT));
	UI.rpm(0);
	UI_RPMIcon();
	UI.showInfo(System.getState(INFO_STATE));

	if (falseIcons)
	{
		UI.showServo(Settings.getHLProfile(System.getState(HL_PROFILE)));
		UI.icon(ICON_LI, true, false);
		UI.icon(ICON_BATTERY, true, false);
		UI.icon(ICON_HIGH_BEAM, true, false);
		UI.icon(ICON_FOG_LI, true, false);
	}
	else	
	{
		if (System.getState(LI_STATE) == OFF) UI.icon(ICON_LI, false, true);
		else if (System.getState(LI_STATE) == DAILY_LI) UI.icon(ICON_DAILY_LI, true, true);
		else if (System.getState(LI_STATE) == NORMAL_LI) UI.icon(ICON_LI, true, true);
		else if (System.getState(LI_STATE) == AUTO_LI)
		{
			if (System.isDark() == YES) UI.icon(ICON_AUTO_LI_ON, true, true);
				else UI.icon(ICON_AUTO_LI_OFF, true, true);

			System.setAutoLight(System.isDark());
		}

		UI_ClearServo();
		UI.icon(ICON_HIGH_BEAM, false, true);
		UI.icon(ICON_FOG_LI, false, true);
		UI.icon(ICON_BATTERY, System_LowBattery(), true);
	}
}

void BootClass::update(bool afterSetup)
{
	if (afterSetup)
	{
		float tempDist = distances[TOTAL_DISTANCE] / 100.00;

		if (Settings.getSetting8(UNIT) == METRIC) distances[TOTAL_DISTANCE] = km2m(tempDist);
			else distances[TOTAL_DISTANCE] = mi2ft(tempDist);

		EEPROM.update32(EEPROM_WHEELE_EXTENT, wheeleExtent);
		EEPROM.update48(EEPROM_DISTANCES, distances[TOTAL_DISTANCE]);
		EEPROM.update8(EEPROM_SPEEDS, speeds[TOP_SPEED]);
		EEPROM.update32(EEPROM_TIMES, times[TOTAL_TIME]);
		EEPROM.update8(EEPROM_SETTINGS8 + UNIT, Settings.getSetting8(UNIT));
		EEPROM.update8(EEPROM_SETTINGS8 + TD_FORMAT, Settings.getSetting8(TD_FORMAT));
		EEPROM.write8(EEPROM_SETUP, SETUP_VALUE);

		screen = RIDE_SCREEN;
	}
	else if (screen == SETTINGS_SCREEN)
	{
		if (oldUnit != Settings.getSetting8(UNIT)) // CONVERTS DISTANCES ETC...
		{
			if (Settings.getSetting8(UNIT) == METRIC) // FROM IMPERIAL TO METRIC
			{
				for (uint8_t i = 0; i < MAX_STATS; i++) stats[i] = ft2m(stats[i]);
				for (uint8_t i = 0; i < MAX_DISTANCES; i++) distances[i] = ft2m(distances[i]);
				for (uint8_t i = 0; i < MAX_SPEEDS; i++) speeds[i] = mih2kmh(speeds[i]);
			}
			else // FROM METRIC TO IMPERIAL
			{
				for (uint8_t i = 0; i < MAX_STATS; i++) stats[i] = m2ft(stats[i]);
				for (uint8_t i = 0; i < MAX_DISTANCES; i++) distances[i] = m2ft(distances[i]);
				for (uint8_t i = 0; i < MAX_SPEEDS; i++) speeds[i] = kmh2mih(speeds[i]);				
			}

			// UPDATES VALUES IN EEPROM
			for (uint8_t i = 0; i < MAX_DISTANCES; i++) EEPROM.write48(EEPROM_DISTANCES + (i * 6), distances[i]);
			for (uint8_t i = 0; i < MAX_STATS; i++) EEPROM.write48(EEPROM_STATS + (i * 6), stats[i]);
			for (uint8_t i = 0; i < MAX_SPEEDS; i++) EEPROM.write8(EEPROM_SPEEDS + i, speeds[i]);
		}

		EEPROM.update8(EEPROM_SETTINGS8, Settings.getSetting8(0)); // FOR SOME REASON "UNIT"(INDEX 0) DOESN'T GET UPDATED CORRECTLY WITHOUT THIS LINE. SO DON'T TOUCH IT.
		for (uint8_t i = 0; i < MAX_SETTINGS8; i++) EEPROM.update8(EEPROM_SETTINGS8 + i, Settings.getSetting8(i));
		for (uint8_t i = 0; i < MAX_SETTINGS16; i++) EEPROM.update16(EEPROM_SETTINGS16 + (i * 2), Settings.getSetting16(i));
		for (uint8_t i = 0; i < MAX_HL_PROFILES; i++) EEPROM.update8(EEPROM_HL_PROFILES + i, Settings.getHLProfile(i));
	}

	// UPDATES RTC MODULE - USKORO : DODAJ PREBACIVANJE IZ 12H U 24H
	if (updateRTC) 
	{
		if (Settings.getSetting8(TD_FORMAT) == TD_AMPM && ampm == true)
		{
			/*
	
			AM
				1 -> 1
				2 -> 2
				...
				11 -> 11
				12 -> 12

			PM
				1 -> 13
				2 -> 14
				...
				10 -> 22
				11 -> 23
				12 -> 0

			*/

			time[0] += 12;
			if (time[0] == 24) time[0] = 0;
		}

		RTC.adjust(DateTime(2000 + date[2], date[1], date[0], time[0], time[1], 0));
	}
}


// SETUP FUNCTIONS
void BootClass::btnSetup(void)
{
	uint16_t duration = 0;

	for (uint8_t i = 0; i < 5; i++) // 5 BECAUSE HIGH BEAM IS NOT IN USE
	{
		if (btnPressedTick[i] < 1) continue; // NOT PRESSED
		duration = millis() - btnPressedTick[i]; // CALCULATES PRESS/HOLD DURATION 
		btnPressedTick[i] = 0;

		if (i == SETUP_BACKWARD_BTN) // GOES BACKWARD WITH SETTINGS
		{
			selectedItem--;
			if (selectedItem == 255) selectedItem = 28; // 255 BECAUSE selectedItem IS UNSIGNED 8BIT VARIABLE
			else if (selectedItem == 4 && Settings.getSetting8(TD_FORMAT) == TD_24CLOCK) selectedItem = 3; // SKIPS AM-PM 
			showSetup();
		}
		else if (i == SETUP_FORWARD_BTN) // GOES FORWARD WITH SETTINGS
		{
			selectedItem++;
			if (selectedItem > 28) selectedItem = 0;
			else if (selectedItem == 4 && Settings.getSetting8(TD_FORMAT) == TD_24CLOCK) selectedItem = 5; // SKIPS AM-PM 
			showSetup();
		}
		else // ADD OR SUBSTRACT SETTING VALUE
		{
			int value = ADD;
			if (i == SETUP_SUB_VALUE_BTN) value = SUB;

			if (selectedItem < 8) generalSettingValue(value);
			else if (selectedItem < 13) wheeleExtentValue(value);
			else if (selectedItem < 21) distanceValue(value);
			else if (selectedItem < 27) tripTimeValue(value);
			else topSpeedValue(value);	

			showSetup();		
		}
	}
}

void BootClass::initSetup(uint8_t scr)
{
	DateTime RTCtime = RTC.now();

	time[0] = RTCtime.hour();
	time[1] = RTCtime.minute();
	date[0] = RTCtime.day();
	date[1] = RTCtime.month();
	date[2] = (RTCtime.year() < 2000) ? (0) : (RTCtime.year() - 2000); 

	screen = scr;
}

void BootClass::topSpeedValue(int8_t value) const
{
	const uint8_t values[2] = {
		10,
		1
	};
	int8_t add = speeds[TOP_SPEED] + (values[selectedItem - 27] * (value));

	if (add < 0) speeds[TOP_SPEED] = 99;
	else if (add > 99) speeds[TOP_SPEED] = 0;
	else speeds[TOP_SPEED] = add;
}

void BootClass::tripTimeValue(int8_t value) const
{
	const uint32_t values[6] = {
		3600000,
		360000,
		36000,
		3600,
		600,
		60
	};
	int32_t add = times[TOTAL_TIME] + (values[selectedItem - 21] * (value));	

	if (add < 0) times[TOTAL_TIME] = 35999940; // 9999H AND 59 MIN
	if (add > 35999940) times[TOTAL_TIME] = 0;
	else times[TOTAL_TIME] = add;
}

void BootClass::distanceValue(int8_t value) const
{
	const uint32_t values[8] = {
		10000000,
		1000000,
		100000,
		10000,
		1000,
		100,
		10,
		1
	};

	int32_t add = distances[TOTAL_DISTANCE] + (values[selectedItem - 13] * (value));

	if (add < 0) distances[TOTAL_DISTANCE] = 99999999;
	else if (add > 99999999) distances[TOTAL_DISTANCE] = 0;
	else distances[TOTAL_DISTANCE] = add;
}

void BootClass::wheeleExtentValue(int8_t value) const
{
	const uint16_t values[5] = {
		10000,	// 2
		1000,	// 3
		100,	// 5
		10,		// 6
		1		// 4
	};
	int32_t add = wheeleExtent + (values[selectedItem - 8] * (value));

	if (add < 0) wheeleExtent = 99999;
	else if (add > 99999) wheeleExtent = 0;
	else wheeleExtent = add;
}

void BootClass::generalSettingValue(int8_t value)
{
	// UNIT
	if (selectedItem == 0) Settings.set8(UNIT, !Settings.getSetting8(UNIT));

	// TIME & DATE FORMAT
	else if (selectedItem == 1)
	{
		uint8_t format = Settings.getSetting8(TD_FORMAT);
		Settings.set8(TD_FORMAT, !format);

		// CORRECTS HOURS
		if (format == TD_24CLOCK)
		{
			if (time[0] > 12) time[0] -= 12;
			else if (time[0] == 0) time[0] = 1;
		}
	}

	// TIME
	else if (selectedItem == 2) // HOURS
	{
		uint8_t min = 1;
		uint8_t max = 12;

		if (Settings.getSetting8(TD_FORMAT) == TD_24CLOCK)
		{
			min = 0;
			max = 23;
		}

		time[0] += (value);
		if (time[0] == 255 || (Settings.getSetting8(TD_FORMAT) == TD_AMPM && time[0] == 0)) time[0] = max; // 255 BECAUSE time IS UNSIGNED 8 BIT ARRAY
		else if (time[0] > max) time[0] = min;
		updateRTC = true;
	}

	else if (selectedItem == 3) // MINUTES
	{
		time[1] += (value);
		if (time[1] == 255) time[1] = 59; // 255 BECAUSE time IS UNSIGNED 8 BIT ARRAY
		else if (time[1] > 59) time[1] = 0;
		updateRTC = true;
	}

	else if (selectedItem == 4) // AM-PM
	{
		ampm = !ampm;
		if (time[0] > 12) time[0] -= 12;
		updateRTC = true;
	}
	
	// DATE
	else if (selectedItem == 5 || selectedItem == 6) // DAY/MONTH || MONTH/DAY
	{
		uint8_t max = 12;
		uint8_t index = 1; // MONTH

		if (selectedItem == 5)
		{
			if (Settings.getSetting8(TD_FORMAT) == TD_24CLOCK)
			{
				max = 31;
				index = 0; // DAY 				
			}
		}
		else if (Settings.getSetting8(TD_FORMAT) == TD_AMPM)
		{
			max = 31;
			index = 0; // DAY		
		}

		date[index] += (value);
		if (date[index] == 0) date[index] = max;
		else if (date[index] > max) date[index] = 1;
		updateRTC = true;
	}

	else if (selectedItem == 7) // YEAR
	{
		date[2] += (value); 
		updateRTC = true;
	}
}

void BootClass::showSetup(void)
{
	Screen.clearDisplay();

	// TITLE
	CURSOR(0, 0);
	SIZE(2);
	TEXT_WHITE();
	Screen.print("SETUP");
	SIZE(1);

	if (selectedItem < 8) generalSettings(); // FIRST PAGE
	else // SECOND PAGE (STARTS AT 8)
	{
		uint8_t wheeleExtentDigits[5] = { 0, 0, 0, 0, 0 };
		uint8_t totalDistDigits[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		uint8_t totalTimeDigitsH[4] = { 0, 0, 0, 0 };
		uint8_t totalTimeDigitsMin[2] = { 0, 0 };
		uint8_t topSpeed[2] = { 0, 0 };
		uint16_t totalTimeH = times[TOTAL_TIME] / 3600;
		uint8_t totalTimeMin = (times[TOTAL_TIME] % 3600) / 60;

		number2Digits(wheeleExtent, wheeleExtentDigits, sizeof(wheeleExtentDigits));
		number2Digits(distances[TOTAL_DISTANCE], totalDistDigits, sizeof(totalDistDigits));
		number2Digits(totalTimeH, totalTimeDigitsH, sizeof(totalTimeDigitsH));
		number2Digits(totalTimeMin, totalTimeDigitsMin, sizeof(totalTimeDigitsMin));
		number2Digits(speeds[TOP_SPEED], topSpeed, sizeof(topSpeed));

		// WHEELE EXTENT
		printItem(20, "WH EXTENT: ");
		for (uint8_t i = 0; i < sizeof(wheeleExtentDigits); i++)
		{
			selected(selectedItem, 8 + i);
			Screen.print(wheeleExtentDigits[i]);

			if (i == 0)
			{
				TEXT_WHITE();
				Screen.print('.');			
			}
		}

		TEXT_WHITE();
		if (Settings.getSetting8(UNIT) == METRIC) Screen.print('m');
			else Screen.print("ft");

		// TOTAL DISTANCE
		printItem(30, "DIST: ");
		for (uint8_t i = 0; i < sizeof(totalDistDigits); i++)
		{
			selected(selectedItem, 13 + i);
			Screen.print(totalDistDigits[i]);

			if (i == 5)
			{
				TEXT_WHITE();
				Screen.print('.');			
			}
		}

		TEXT_WHITE();
		if (Settings.getSetting8(UNIT) == METRIC) Screen.print("km");
			else Screen.print("mi");

		// TOTAL TIME
		printItem(40, "TIME: ");
		for (uint8_t i = 0; i < sizeof(totalTimeDigitsH); i++)
		{
			selected(selectedItem, 21 + i);
			Screen.print(totalTimeDigitsH[i]);
		}

		TEXT_WHITE();
		Screen.print("h ");

		for (uint8_t i = 0; i < sizeof(totalTimeDigitsMin); i++)
		{
			selected(selectedItem, 25 + i);
			Screen.print(totalTimeDigitsMin[i]);
		}

		TEXT_WHITE();
		Screen.print("min");

		// TOP SPEED
		printItem(50, "TOP SPEED: ");
		for (uint8_t i = 0; i < sizeof(topSpeed); i++)
		{
			selected(selectedItem, 27 + i);
			Screen.print(topSpeed[i]);
		}

		TEXT_WHITE();
		if (Settings.getSetting8(UNIT) == METRIC) Screen.print("km/h");
			else Screen.print("mi/h");		
	}

	Screen.display();
}

void BootClass::setDefaultValues(void) const
{
	// SCREEN MESSAGE
	Screen.clearDisplay();
	CURSOR(0, 0);
	TEXT_WHITE();
	SIZE(1);
	Screen.print("Setting things up.\nPlease wait...");
	Screen.display();

	DateTime time = RTC.now();

	// DISTANCES
	for (uint8_t i = 0; i < MAX_DISTANCES; i++) EEPROM.update48(EEPROM_DISTANCES + (i * 6), 0);

	// SETTINGS
	for (uint8_t i = 0; i < MAX_SETTINGS8; i++) EEPROM.update8(EEPROM_SETTINGS8 + i, Settings.getSetting8(i));
	for (uint8_t i = 0; i < MAX_SETTINGS16; i++) EEPROM.update16(EEPROM_SETTINGS16 + (i * 2), Settings.getSetting16(i));

	// SPEEDS
	for (uint8_t i = 0; i < MAX_SPEEDS; i++) EEPROM.update8(EEPROM_SPEEDS + i, 0);

	// TIMES
	for (uint8_t i = 0; i < MAX_TIMES; i++) EEPROM.update32(EEPROM_TIMES + (i * 4), 0);

	// STATES
	for (uint8_t i = 0; i < MAX_STATES; i++) EEPROM.update8(EEPROM_STATES + i, System.getState(i));

	// STATS
	for (uint8_t i = 0; i < MAX_STATS; i++) EEPROM.update48(EEPROM_STATS + (i * 6), 0);

	// HEADLIGHT PROFILES
	for (uint8_t i = 0; i < MAX_HL_PROFILES; i++) EEPROM.update8(EEPROM_HL_PROFILES + i, 90);

	// TOUR
		// USKOR - DODAJ ZAPIS NA DEF POSTAVKE

	// OTHER SETTINGS & VALUES
	EEPROM.update8(EEPROM_DAILY_DIST_DAY, time.day());
	EEPROM.update8(EEPROM_MONTHLY_DIST_MONTH, time.month());
	EEPROM.update32(EEPROM_WHEELE_EXTENT, DEF_WHEELE_EXTENT);

	Screen.clearDisplay();
}


// SETTINGS FUNCTIONS
void BootClass::btnSettings(void)
{
	uint16_t duration = 0;

	for (uint8_t i = 0; i < 5; i++) // 5 BECAUSE HIGH BEAM IS NOT IN USE
	{
		if (btnPressedTick[i] < 1) continue; // NOT PRESSED
		duration = millis() - btnPressedTick[i]; // CALCULATES PRESS/HOLD DURATION 
		btnPressedTick[i] = 0;

		if (duration > BTN_HOLD)
		{
			if (i == SETTINGS_ENTER_BTN && screen == RIDE_SCREEN) // ENTERS INTO SETTINGS
			{
				initSetup(SETTINGS_SCREEN);
				inMenu = 0;
				selectedItem = 0;
				selectedMenu = 0;
				oldUnit = Settings.getSetting8(UNIT);
				showMenus();

				continue;
			}
			if (i == SETTINGS_ENTER_MENU_BTN && inMenu == 0 && screen == SETTINGS_SCREEN) // ENTERS IN SETTINGS MENU
			{
				showSettingMenu();
				inMenu = selectedMenu + 1; // + 1 BECAUSE 0 IS MAIN MENU
				continue;				
			}
			if (i == SETTINGS_EXIT_MENU_BTN && inMenu != 0 && screen == SETTINGS_SCREEN) // EXITS FROM SETTINGS MENU
			{
				inMenu = 0;
				selectedItem = 0;
				showMenus();

				continue;
			}
			if (i == SETTINGS_RST_TO_DEF_BTN && inMenu != 0) // RESETS SELECTED SETTING TO DEFAULT VALUE
			{
				if (inMenu == 1) // GENERAL SETTINGS
				{
					if (selectedItem == 0) Settings.set8(UNIT, DEF_UNIT);
					else if (selectedItem == 1) Settings.set8(TD_FORMAT, DEF_TD_FORMAT);
				}
				else if (inMenu == 2) // BATTERY SAVER
				{
					if (selectedItem == 0) Settings.set8(BATT_SAVER, DEF_BATT_SAVER);
					else if (selectedItem == 1) Settings.set8(BATT_SAVER_LVL, DEF_BATT_SAVER_LVL);
					else if (selectedItem == 2) Settings.set16(SAVER_TIME, DEF_SAVER_TIME);
					else Settings.set8(SCR_DIM, DEF_SCR_DIM);
				}
				else if (inMenu == 3) // SENSORS
				{
					if (selectedItem == 0) sensorStep = SENSOR_STEP;
					else if (selectedItem == 1) Settings.set16(FRONT_BRAKE, DEF_FRONT_BRAKE);
					else if (selectedItem == 2) Settings.set16(REAR_BRAKE, DEF_REAR_BRAKE);
					else Settings.set16(AUTO_LIGHT, DEF_AUTO_LIGHT);
				}
				else if (inMenu == 4) // LIGHTS
				{
					if (selectedItem == 0) Settings.set8(HL_DAILY_BRGH, DEF_HL_DAILY_BRGH);
					else if (selectedItem == 1) Settings.set8(HL_BRGH, DEF_HL_BRGH);
					else if (selectedItem == 2) Settings.set8(HL_HB_BRGH, DEF_HL_HB_BRGH);
					else if (selectedItem == 3) Settings.set8(TL_DAILY_BRGH, DEF_TL_DAILY_BRGH);
					else if (selectedItem == 4) Settings.set8(TL_BRGH, DEF_TL_BRGH);
					else if (selectedItem == 5) Settings.set8(TL_FOG_BRGH, DEF_TL_FOG_BRGH);
					else if (selectedItem == 6) Settings.set8(SMOOTH_LIGHTS, DEF_SMOOTH_LIGHTS);
					else Settings.set8(CORNERING_LIGHTS, DEF_CORNERING_LIGHTS);
				}
				else if (inMenu == 5) // SOUNDS
				{
					if (selectedItem == 0) Settings.set8(SOUNDS, DEF_SOUNDS);
					else if (selectedItem == 1) Settings.set8(BTN_SOUND, DEF_BTN_SOUND);
					else if (selectedItem == 2) Settings.set8(TS_SOUND, DEF_TS_SOUND);
					else Settings.set8(ALERT_SOUND, DEF_ALERT_SOUND);
				}
				else if (inMenu == 6) // INDICATORS
				{
					if (selectedItem == 0) Settings.set8(BRAKE_LIGHT, DEF_BRAKE_LIGHT);
					else if (selectedItem == 1) Settings.set8(TURN_SIGNALS, DEF_TURN_SIGNALS);
					else Settings.set8(SHIFT_IND, DEF_SHIFT_IND);

				}

				else if (inMenu == 7) Settings.setHLProfile(selectedItem, 90); // HEADLIGHT PROFILES
			
				showSettingMenu();
				continue;
			}
		}
		else if (duration > BTN_PRESS && screen == SETTINGS_SCREEN) // PROTECTION AGAINST DOUBLE PRESS DETECTION
		{
			if (i == SETTINGS_BACKWARD_BTN) // GOES BACKWARD WITH MENUS AND SETTINGS
			{
				if (inMenu == 0) // IN MAIN MENU
				{
					selectedMenu--;
					if (selectedMenu == 255) selectedMenu = 7; // 255 BECAUSE selectedMenu IS UNSIGNED 8BIT VARIABLE
					showMenus();

					continue;
				}
				else // IN SELECTED MENU WITH SETTINGS
				{
					selectedItem--;
					if (selectedItem == 255) // 255 BECAUSE selectedItem IS UNSIGNED 8BIT VARIABLE
					{
						if (inMenu == 1) selectedItem = 7; // GENERAL SETTINGS
						else if (inMenu == 2) selectedItem = 3; // BATTERY SAVER
						else if (inMenu == 3) selectedItem = 3; // SENSORS
						else if (inMenu == 4) selectedItem = 7; // LIGHTS
						else if (inMenu == 5) selectedItem = 3; // SOUNDS
						else if (inMenu == 6) selectedItem = 2; // INDICATORS
						else selectedItem = MAX_HL_PROFILES - 1; // HEADLIGHT PROFILES
					}
					else if (Settings.getSetting8(TD_FORMAT) == TD_24CLOCK && selectedItem == 4 && inMenu == 1) selectedItem = 3; // SKIPS

					showSettingMenu();

					continue;
				}
			}
			if (i == SETTINGS_FORWARD_BTN) // GOES FORWARD WITH MENUS AND SETTINGS
			{
				if (inMenu == 0) // IN MAIN MENU
				{
					selectedMenu++;
					if (selectedMenu > 7) selectedMenu = 0;

					showMenus();

					continue;
				}
				else // IN SELECTED MENU WITH SETTINGS
				{
					uint8_t max = 1;
					selectedItem++;

					if (inMenu == 1) max = 7; // GENERAL SETTINGS
					else if (inMenu == 2) max = 3; // BATTERY SAVER
					else if (inMenu == 3) max = 3; // SENSORS
					else if (inMenu == 4) max = 7; // LIGHTS
					else if (inMenu == 5) max = 3; // SOUNDS
					else if (inMenu == 6) max = 2; // INDICATORS
					else max = MAX_HL_PROFILES - 1; // HEADLIGHT PROFILES

					if (selectedItem > max) selectedItem = 0;
					else if (Settings.getSetting8(TD_FORMAT) == TD_24CLOCK && selectedItem == 4 && inMenu == 1) selectedItem = 5; // SKIPS

					showSettingMenu();

					continue;
				}
			}
			if (i == SETTINGS_SUB_VALUE_BTN || i == SETTINGS_ADD_VALUE_BTN) // DECREASING/INCREASING VALUE OF SETTING
			{
				int8_t value = ADD;
				if (i == SETTINGS_SUB_VALUE_BTN) value = SUB;

				if (inMenu == 1) generalSettingValue(value);
				else if (inMenu == 2) batterySaverValue(value);
				else if (inMenu == 3) sensorsValue(value);
				else if (inMenu == 4) lightsValue(value);
				else if (inMenu == 5) soundsValue(value);
				else if (inMenu == 6) indicatorsValue(value);
				else HLProfilesValue(value);

				showSettingMenu();
			} 
		}
	}
}

void BootClass::showMenus(void)
{
	Screen.clearDisplay();

	// TITLE
	TEXT_WHITE();
	CURSOR(0, 0);
	SIZE(2);
	Screen.print("SETTINGS");
	SIZE(1);

	if (selectedMenu < 4) // FIRST PAGE
	{
		CURSOR(0, 20);
		if (selectedMenu == 0)
		{
			#if SETTING_IND == LEFT
				Screen.print("> GENERAL");
			#else
				Screen.print(" GENERAL <");
			#endif 
		}
		else Screen.print(" GENERAL");

		CURSOR(0, 30);
		if (selectedMenu == 1)
		{
			#if SETTING_IND == LEFT
				Screen.print("> BATT SAVER");
			#else
				Screen.print(" BATT SAVER <");
			#endif 
		}
		else Screen.print(" BATT SAVER");

		CURSOR(0, 40);
		if (selectedMenu == 2)
		{
			#if SETTING_IND == LEFT
				Screen.print("> SENSORS");
			#else
				Screen.print(" SENSORS <");
			#endif 
		}
		else Screen.print(" SENSORS");

		CURSOR(0, 50);
		if (selectedMenu == 3)
		{
			#if SETTING_IND == LEFT
				Screen.print("> LIGHTS");
			#else
				Screen.print(" LIGHTS <");
			#endif 
		}
		else Screen.print(" LIGHTS");
	}
	else // SECOND PAGE
	{
		CURSOR(0, 20);
		if (selectedMenu == 4)
		{
			#if SETTING_IND == LEFT
				Screen.print("> SOUNDS");
			#else
				Screen.print(" SOUNDS <");
			#endif 
		}
		else Screen.print(" SOUNDS");

		CURSOR(0, 30);
		if (selectedMenu == 5)
		{
			#if SETTING_IND == LEFT
				Screen.print("> INDICATORS");
			#else
				Screen.print(" INDICATORS <");
			#endif 
		}
		else Screen.print(" INDICATORS");

		CURSOR(0, 40);
		if (selectedMenu == 6)
		{
			#if SETTING_IND == LEFT
				Screen.print("> HL PROFILES");
			#else
				Screen.print(" HL PROFILES <");
			#endif 
		}
		else Screen.print(" HL PROFILES");

		CURSOR(0, 50);
		if (selectedMenu == 7)
		{
			#if SETTING_IND == LEFT
				Screen.print("> RST 2 DEF");
			#else
				Screen.print(" RST 2 DEF <");
			#endif 
		}
		else Screen.print(" RST 2 DEF"); 
	}

	TEXT_WHITE(); // JUST IN CASE
	Screen.display();
}

void BootClass::showSettingMenu(void)
{
	if (selectedMenu != 2) Screen.clearDisplay();

	if (selectedMenu == 0) // GENERAL
	{
		printMenuTitle(" >> GENERAL ");
		generalSettings();
	}

	else if (selectedMenu == 1) // BATTERY SAVER
	{
		printMenuTitle(" >> BATT SAVER ");

		// STATUS
		printItem(20, "STATUS: ");
		selected(selectedItem, 0);
		if (Settings.getSetting8(BATT_SAVER) == OFF) Screen.print("OFF");
			else Screen.print("AUTO ON");

		// SAVER LEVEL
		printItem(30, "SAVER LEVEL: ");
		selected(selectedItem, 1);
		if (Settings.getSetting8(BATT_SAVER_LVL) == LOW) Screen.print("LOW");
			else Screen.print("HIGH");

		// SAVER LEVEL
		printItem(40, "SAVER: ");
		selected(selectedItem, 2);
		if (Settings.getSetting16(SAVER_TIME) == 0) Screen.print("OFF");
		else
		{
			Screen.print(Settings.getSetting16(SAVER_TIME) / 1000);
			Screen.print('s');
		}

		// SCREEN DIM
		printItem(50, "SCR DIM: ");
		selected(selectedItem, 3);
		if (Settings.getSetting8(SCR_DIM) == OFF) Screen.print("OFF");
		else if (Settings.getSetting8(SCR_DIM) == YES) Screen.print("ALWAYS");
		else Screen.print("AUTO");
	}

	else if (selectedMenu == 2) // SENSORS
	{
		printMenuTitle(" >> SENSORS ");

		// STEP
		printItem(20, "STEP: ");
		selected(selectedItem, 0);
		Screen.print(sensorStep);
		Screen.print(' ');

		// FRONT BRAKE
		printItem(30, "FRONT BR: ");
		selected(selectedItem, 1);
		Screen.print(Settings.getSetting16(FRONT_BRAKE));
		Screen.print(' ');	

		// REAR BRAKE
		printItem(40, "REAR BR: ");
		selected(selectedItem, 2);
		Screen.print(Settings.getSetting16(REAR_BRAKE));
		Screen.print(' ');	

		// AUTO LIGHT
		printItem(50, "AUTO LI: ");
		selected(selectedItem, 3);
		Screen.print(Settings.getSetting16(AUTO_LIGHT));
		Screen.print(' ');
	}

	else if (selectedMenu == 3) // LIGHTS
	{
		printMenuTitle(" >> LIGHTS ");

		// HEADLIGHT
		printItem(20, "HEADLIGHT: ");

		if (selected(selectedItem, 0)) updateLights(selectedItem);
		Screen.print(Settings.getSetting8(HL_DAILY_BRGH));

		TEXT_WHITE();
		Screen.print('/');	

		if (selected(selectedItem, 1)) updateLights(selectedItem);
		Screen.print(Settings.getSetting8(HL_BRGH));		

		TEXT_WHITE();
		Screen.print('/');	

		if (selected(selectedItem, 2)) updateLights(selectedItem);
		Screen.print(Settings.getSetting8(HL_HB_BRGH));	

		// TAILLIGHT
		printItem(30, "TAILLIGHT: ");

		if (selected(selectedItem, 3)) updateLights(selectedItem);
		Screen.print(Settings.getSetting8(TL_DAILY_BRGH));

		TEXT_WHITE();
		Screen.print('/');	

		if (selected(selectedItem, 4)) updateLights(selectedItem);
		Screen.print(Settings.getSetting8(TL_BRGH));		

		TEXT_WHITE();
		Screen.print('/');	

		if (selected(selectedItem, 5)) updateLights(selectedItem);
		Screen.print(Settings.getSetting8(TL_FOG_BRGH));

		// SMOOTH LIGHT
		printItem(40, "SMOOTH LI: ");
		selected(selectedItem, 6);
		if (Settings.getSetting8(SMOOTH_LIGHTS) == OFF) Screen.print("OFF");
			else Screen.print("ON");

		// CORNERING LIGHT
		printItem(50, "CORNER LI: ");
		selected(selectedItem, 7);
		if (Settings.getSetting8(CORNERING_LIGHTS) == OFF) Screen.print("OFF");
		else
		{
			Screen.print("+/-");
			Screen.print(Settings.getSetting8(CORNERING_LIGHTS));
			Screen.print("deg");
		}

		if (selectedItem > 5) updateLights(10); // RANDOM VALUE BETWEEN 6 - 255
	}

	else if (selectedMenu == 4) // SOUNDS
	{
		printMenuTitle(" >> SOUNDS ");

		// GENERAL
		printItem(20, "GENERAL: ");
		selected(selectedItem, 0);
		if (Settings.getSetting8(SOUNDS) == OFF) Screen.print("OFF");
			else Screen.print("ON");

		// BUTTON SOUND
		printItem(30, "BUTTON: ");
		selected(selectedItem, 1);
		if (Settings.getSetting8(BTN_SOUND) == OFF) Screen.print("OFF");
		else
		{
			Screen.print("Sound");
			Screen.print(Settings.getSetting8(BTN_SOUND));
			Screen.print(' ');
		}

		// TURN SIGNAL SOUND
		printItem(40, "TURN SIGNAL: ");
		selected(selectedItem, 2);
		if (Settings.getSetting8(TS_SOUND) == OFF) Screen.print("OFF");
		else
		{
			Screen.print("Sound");
			Screen.print(Settings.getSetting8(TS_SOUND));
			Screen.print(' ');
		}

		// ALERT SOUND
		printItem(50, "ALERT: ");
		selected(selectedItem, 3);
		if (Settings.getSetting8(ALERT_SOUND) == OFF) Screen.print("OFF");
		else
		{
			Screen.print("Sound");
			Screen.print(Settings.getSetting8(ALERT_SOUND));
			Screen.print(' ');
		}	
	}

	else if (selectedMenu == 5) // INDICATORS
	{
		printMenuTitle(" >> INDICATORS ");

		// BRAKE LIGHT
		printItem(20, "BRAKE LI: ");
		selected(selectedItem, 0);
		if (Settings.getSetting8(BRAKE_LIGHT) == OFF) Screen.print("OFF");
		else
		{
			Screen.print("FLASH ");
			Screen.print(Settings.getSetting8(BRAKE_LIGHT));
			Screen.print('x');
		}	

		// TURN SIGNALS
		printItem(30, "TURN SIGNALS: ");
		selected(selectedItem, 1);
		if (Settings.getSetting8(TURN_SIGNALS) == OFF) Screen.print("OFF");
			else Screen.print("ON");

		// SHIFT INDICATOR
		printItem(40, "SHIFT IND: ");
		selected(selectedItem, 2);
		if (Settings.getSetting8(SHIFT_IND) == OFF) Screen.print("OFF");
		else
		{
			Screen.print(Settings.getSetting8(SHIFT_IND));
			Screen.print("rpm");
		}
	}

	else if (selectedMenu == 6) // HEADLIGHT PROFILES
	{
		uint8_t offset = 0;	// FIRST PAGE
		uint8_t max = 4; // FIRST PAGE	
		printMenuTitle(" >> HL PROFILES ");

		if (selectedItem > 3) // SECOND PAGE
		{
			offset = 4;
			max = 2;
		}

		for (uint8_t i = 0; i < max; i++)
		{
			printItem(20 + (i * 10), "PROFILE");
			Screen.print(i + 1 + offset);
			Screen.print(": ");
			selected(selectedItem, i + offset);
			if (Settings.getHLProfile(i + offset) != 0)
			{
				Screen.print(Settings.getHLProfile(i + offset));
				Screen.print("deg");
			}
			else Screen.print("OFF");	
		}
	}
	else // RESET 2 DEFAULT
	{
		bool reset = false;
		uint32_t hbPress = 0;

		printMenuTitle(" >> RST 2 DEF ");
		CURSOR(0, 10);
		SIZE(1);
		TEXT_WHITE();
		Screen.print("If You want cancle\nthis action, please\nPRESS High Beam\nbutton. If You want\nto continue, please\nHOLD High Beam button");
		Screen.display();
			
		do
		{
			if (digitalRead(BTN_HIGH_BEAM) == LOW)
			{
				if (hbPress == 0) hbPress = millis();
			}
			else if (hbPress != 0)
			{
				if (millis() - hbPress > BTN_HOLD) reset = true;
				break;
			}

			delay(50);
		}
		while (true);

		if (reset)
		{
			// GENERAL SETTINGS
			Settings.set8(UNIT, DEF_UNIT);
			Settings.set8(TD_FORMAT, DEF_TD_FORMAT);

			// BATTERY SAVER
			Settings.set8(BATT_SAVER, DEF_BATT_SAVER);
			Settings.set8(BATT_SAVER_LVL, DEF_BATT_SAVER_LVL);
			Settings.set16(SAVER_TIME, DEF_SAVER_TIME);
			Settings.set8(SCR_DIM, DEF_SCR_DIM);

			// SENSORS
			Settings.set16(FRONT_BRAKE, DEF_FRONT_BRAKE);
			Settings.set16(REAR_BRAKE, DEF_REAR_BRAKE);
			Settings.set16(AUTO_LIGHT, DEF_AUTO_LIGHT);

			// LIGHTS
			Settings.set8(HL_DAILY_BRGH, DEF_HL_DAILY_BRGH);
			Settings.set8(HL_BRGH, DEF_HL_BRGH);
			Settings.set8(HL_HB_BRGH, DEF_HL_HB_BRGH);
			Settings.set8(TL_DAILY_BRGH, DEF_TL_DAILY_BRGH);
			Settings.set8(TL_BRGH, DEF_TL_BRGH);
			Settings.set8(TL_FOG_BRGH, DEF_TL_FOG_BRGH);
			Settings.set8(SMOOTH_LIGHTS, DEF_SMOOTH_LIGHTS);
			Settings.set8(CORNERING_LIGHTS, DEF_CORNERING_LIGHTS);
			
			// SOUNDS
			Settings.set8(SOUNDS, DEF_SOUNDS);
			Settings.set8(BTN_SOUND, DEF_BTN_SOUND);
			Settings.set8(TS_SOUND, DEF_TS_SOUND);
			Settings.set8(ALERT_SOUND, DEF_ALERT_SOUND);

			// INDICATORS
			Settings.set8(BRAKE_LIGHT, DEF_BRAKE_LIGHT);
			Settings.set8(TURN_SIGNALS, DEF_TURN_SIGNALS);
			Settings.set8(SHIFT_IND, DEF_SHIFT_IND);

			// HEADLIGHT PROFILES
			for (uint8_t i = 0; i < MAX_HL_PROFILES; i++) Settings.setHLProfile(i, 90);
		}

		// EXITS FROM RESET TO DEFAULT MENU
		selectedItem = 0;
		inMenu = 0;
		showMenus();
	}

	Screen.display();
}

void BootClass::printHLprofiles(bool page)
{
	uint8_t offset = 0;
	uint8_t max = 4;
	if (page)
	{
		offset = 4;
		max = 2;
	}

	for (uint8_t i = 0; i < max; i++)
	{
		printItem(20 + (i * 10), "PROFILE");
		Screen.print(i + 1 + offset);
		Screen.print(": ");
		selected(selectedItem, i + offset);
		Screen.print(Settings.getHLProfile(i + offset));
		Screen.print("deg");			
	}
}

void BootClass::generalSettings(void)
{
	// UPDATES DATE & TIME VALUES
	if (!updateRTC && screen != SETUP_SCREEN)
	{
		DateTime clock = RTC.now();

		time[0] = clock.hour();
		time[1] = clock.minute();
		date[0] = clock.day();
		date[1] = clock.month();
		date[2] = clock.year() - 2000;
	}

	// UNIT
	printItem(20, "UNIT: ");
	selected(selectedItem, 0);
	if (Settings.getSetting8(UNIT) == METRIC) Screen.print("Metric");
		else Screen.print("Imperial");

	// TIME & DATE FORMAT
	printItem(30, "TD FORMAT: ");
	selected(selectedItem, 1);
	if (Settings.getSetting8(TD_FORMAT) == TD_24CLOCK) Screen.print("24h d.m");
		else Screen.print("12h m/d");

	// TIME
	printItem(40, "TIME: ");

	selected(selectedItem, 2);
	UI.leadingZero(time[0], 2, true);

	TEXT_WHITE();
	Screen.print(':');

	selected(selectedItem, 3);
	UI.leadingZero(time[1], 2, true);

	if (Settings.getSetting8(TD_FORMAT) == TD_24CLOCK)
	{
		// DATE
		printItem(50, "DATE: ");

		selected(selectedItem, 5); // STARTS AT FIVE
		UI.leadingZero(date[0], 2, true);

		TEXT_WHITE();
		Screen.print('.');

		selected(selectedItem, 6);
		UI.leadingZero(date[1], 2, true);

		TEXT_WHITE();
		Screen.print('.');

		selected(selectedItem, 7);
		Screen.print(date[2] + 2000);	

		TEXT_WHITE();
		Screen.print('.');		
	}
	else
	{
		// AM-PM
		TEXT_WHITE();
		Screen.print(' ');

		selected(selectedItem, 4);
		if (ampm) Screen.print("PM");
			else Screen.print("AM");

		// DATE
		printItem(50, "DATE: ");

		selected(selectedItem, 5);
		UI.leadingZero(date[1], 2, true);

		TEXT_WHITE();
		Screen.print('/');

		selected(selectedItem, 6);
		UI.leadingZero(date[0], 2, true);

		TEXT_WHITE();
		Screen.print('/');

		selected(selectedItem, 7);
		Screen.print(date[2] + 2000);	
	}
}

void BootClass::batterySaverValue(int8_t value) const
{
	// STATUS
	if (selectedItem == 0) Settings.set8(BATT_SAVER, !Settings.getSetting8(BATT_SAVER));

	// LEVEL
	else if (selectedItem == 1) Settings.set8(BATT_SAVER_LVL, !Settings.getSetting8(BATT_SAVER_LVL));

	// SCREEN SAVER
	else if (selectedItem == 2) Settings.setSetting16(1000 * (value), SAVER_TIME, MIN_SCR_SAVER, MAX_SCR_SAVER, true);

	// SCREEN DIM
	else Settings.setSetting8(value, SCR_DIM, 0, 2, false);
}

void BootClass::sensorsValue(int8_t value)
{
	// STEP VALUE
	if (selectedItem == 0)
	{
		int16_t newValue = sensorStep + (10 * (value));

		if (newValue > 250) sensorStep = 20;
		else if (newValue < 20) sensorStep = 250;
		else sensorStep = newValue;
	}

	// FRONT BRAKE
	else if (selectedItem == 1) Settings.setSetting16(sensorStep * (value), FRONT_BRAKE, MIN_SEN_VALUE, MAX_SEN_VALUE, false);

	// REAR BRAKE
	else if (selectedItem == 2) Settings.setSetting16(sensorStep * (value), REAR_BRAKE, MIN_SEN_VALUE, MAX_SEN_VALUE, false);

	// AUTO LIGHT
	else Settings.setSetting16(sensorStep * (value), AUTO_LIGHT, MIN_SEN_VALUE, MAX_SEN_VALUE, false);
}

void BootClass::lightsValue(int8_t value) const
{
	// HEADLIGHT BRIGHTNESS
	if (selectedItem < 3) Settings.setSetting8(BRGH_STEP * (value), HL_DAILY_BRGH + selectedItem, MIN_BRGH_VALUE, MAX_BRGH_VALUE, false);

	// TAILLIGHT BRIGHTNESS
	else if (selectedItem < 6) Settings.setSetting8(BRGH_STEP * (value), HL_DAILY_BRGH + selectedItem, MIN_BRGH_VALUE, MAX_BRGH_VALUE, false);

	// SMOOTH LIGHTS
	else if (selectedItem == 6) Settings.set8(SMOOTH_LIGHTS, !Settings.getSetting8(SMOOTH_LIGHTS));

	// CORNERING LIGHTS
	else Settings.setSetting8(value, CORNERING_LIGHTS, MIN_LEAN_ANGLE, MAX_LEAN_ANGLE, true);
}

void BootClass::soundsValue(int8_t value) const
{
	// GENERAL
	if (selectedItem == 0) Settings.set8(SOUNDS, !Settings.getSetting8(SOUNDS));

	// BUTTON SOUND
	else if (selectedItem == 1)
	{
		Settings.setSetting8(value, BTN_SOUND, 0, MAX_SOUNDS - 1, false);
		delay(1000);
		System.playBtnSound();
	}

	// TURN SIGNAL SOUND
	else if (selectedItem == 2)
	{
		Settings.setSetting8(value, TS_SOUND, 0, MAX_SOUNDS - 1, false);
		delay(1000);
		System.playTSSound();
	}

	// ALERT SOUND
	else
	{
		Settings.setSetting8(value, ALERT_SOUND, 0, MAX_SOUNDS - 1, false);
		delay(1000);
		System.playAlertSound();
	}
}

void BootClass::indicatorsValue(int8_t value) const
{
	// BRAKE LIGHT
	if (selectedItem == 0) Settings.setSetting8(value, BRAKE_LIGHT, MIN_BRAKE_FLASHES, MAX_BRAKE_FLASHES, true);

	// TURN SIGNALS
	else if (selectedItem == 1) Settings.set8(TURN_SIGNALS, !Settings.getSetting8(TURN_SIGNALS));

	// SHIFT INDICATOR
	else Settings.setSetting8(SHIFT_STEP * (value), SHIFT_IND, MIN_SHIFT_IND, MAX_SHIFT_IND, true);
}

void BootClass::HLProfilesValue(int8_t value) const
{
	int16_t newAngle = Settings.getHLProfile(selectedItem) + (value);

	if (newAngle > MAX_HL_ANGLE) Settings.setHLProfile(selectedItem, 0);
	else if (newAngle < MIN_HL_ANGLE)
	{
		if (newAngle < 0) Settings.setHLProfile(selectedItem, MAX_HL_ANGLE);
		else if (value > 0) Settings.setHLProfile(selectedItem, MIN_HL_ANGLE);
		else Settings.setHLProfile(selectedItem, 0);
	}
	else Settings.setHLProfile(selectedItem, newAngle);
}

void BootClass::printItem(uint8_t y, char *text) const
{
	CURSOR(0, y);
	TEXT_WHITE();
	Screen.print(text);	
}

void BootClass::printMenuTitle(char *text) const
{
	CURSOR(0, 0);
	SIZE(1);
	TEXT_BLACK();
	Screen.print(text);
	TEXT_WHITE();
}

void BootClass::updateLights(uint8_t index) const
{
	pwmWrite(LED_HEAD, 0);
	pwmWrite(LED_HB_LEFT, 0);
	pwmWrite(LED_HB_RIGHT, 0);
	pwmWrite(LED_TAIL1, 0);
	digitalWrite(LED_TAIL2, LOW);

	// HEADLIGHT
	if (index == 0) pwmWrite(LED_HEAD, Settings.getSetting8(HL_DAILY_BRGH)); // DAILY LIGHT
	else if (index == 1) pwmWrite(LED_HEAD, Settings.getSetting8(HL_BRGH)); // NORMAL LIGHT
	else if (index == 2) // HIGH BEAM
	{
		pwmWrite(LED_HEAD, Settings.getSetting8(HL_HB_BRGH));
		pwmWrite(LED_HB_LEFT, Settings.getSetting8(HL_HB_BRGH));
		pwmWrite(LED_HB_RIGHT, Settings.getSetting8(HL_HB_BRGH));
	}

	// TAILLIGHT
	else if (index == 3) pwmWrite(LED_TAIL1, Settings.getSetting8(TL_DAILY_BRGH)); // DAILY LIGHT
	else if (index == 4) pwmWrite(LED_TAIL1, Settings.getSetting8(TL_BRGH)); // NORMAL LIGHT
	else if (index == 5) // FOG LIGHT
	{
		pwmWrite(LED_TAIL1, Settings.getSetting8(TL_FOG_BRGH));
		digitalWrite(LED_TAIL2, HIGH);
	}
}


// GET/SET FUNCTIONS
uint8_t BootClass::getScreen(void) const
{
	return (screen);
}

uint8_t BootClass::getSetup(void) const
{
	return (setup);
}

uint8_t BootClass::getSettingMenu(void) const
{
	return (inMenu);
}
