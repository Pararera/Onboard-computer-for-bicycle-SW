
/*

	USER INTERFACE FUNCTIONS

*/


// LIBRARIES
#include 																			<stdint.h>
#include                                        									<Adafruit_SH1106_STM32.h>

// MODULES
#include 																			"Headers\UI.h"
#include 																			"Headers\Main.h"
#include 																			"Headers\Settings.h"
#include 																			"Headers\Macros.h"



// OBJECTS
Adafruit_SH1106 Screen(4);
UIClass UI;


// (DE)CONSTRUCOTR
UIClass::UIClass()
{

}

UIClass::~UIClass()
{
	
}


// FUNCTIONS
void UIClass::updateTDT(void)
{
	DateTime time = RTC.now();

	UI_ClearTDT();

	// CLOCK
	if (Settings.getSetting8(TD_FORMAT) == TD_24CLOCK)
	{
		CURSOR(CLOCK_POS_X_24H, CLOCK_POS_Y);
		leadingZero(time.hour(), 2, true);
		Screen.print(':');
		leadingZero(time.minute(), 2, true);
	}
	else
	{
		bool ampm = false;
		uint8_t hour = time.hour();

		if (hour > 12)
		{
			hour -= 12;
			ampm = true;
		}
		else if (hour == 0)
		{
			ampm = true;
			hour = 12;
		}

		CURSOR(CLOCK_POS_X_12H, CLOCK_POS_Y);
		leadingZero(hour, 2, true);
		Screen.print(':');
		leadingZero(time.minute(), 2, true);
		if (ampm) Screen.print(" PM");
			else Screen.print(" AM");
	}

	// DATE OR TEMP
	if (changeTDT) DT = !DT;

	if (DT == DT_DATE) // SHOWS DATE
	{
        char const daysOfTheWeek[7][4] =
        {
            "Sun",
            "Mon",
            "Tue",
            "Wed",
            "Thu",
            "Fri",
            "Sat"
        };

		CURSOR(DATE_POS_X, DATETEMP_POS_Y);

        Screen.print(daysOfTheWeek[time.dayOfTheWeek()]);
        Screen.print(' ');
        leadingZero(time.day(), 2, true); 
        if (Settings.getSetting8(TD_FORMAT) == TD_24CLOCK) Screen.print('.');
	}
	else // SHOWS TEMP
	{
		int16_t temp = RTC.temp();
		char unit = 'C';
		if (Settings.getSetting8(UNIT) == IMPERIAL)
		{
			temp = cel2far(temp);
			unit = 'F';
		}

		if (temp > -10 && temp < 100) CURSOR(TEMP_POS_X_3DIG, DATETEMP_POS_Y);
		else CURSOR(TEMP_POS_X_4DIG, DATETEMP_POS_Y);

		if (temp > -1 && temp < 10) leadingZero(temp, 2, true);
			else Screen.print(temp);
		Screen.print(unit);
	}

	update = true;
}

void UIClass::icon(uint8_t iconID, bool show, bool clear)
{
	if (clear) UI_ClearIcon(iconPositions[0][iconID], iconPositions[1][iconID]);

	if (show) Screen.drawBitmap(iconPositions[0][iconID], iconPositions[1][iconID], icons[iconID], ICON_SIZE_X, ICON_SIZE_Y, WHITE);

	update = true;
}

void UIClass::speed(uint8_t value)
{
	// SPEED
	CURSOR(SPEED_POS_X, SPEED_POS_Y);
	SIZE(2);

	if (value > 99) value = 99;
	leadingZero(value, 2, true);
	SIZE(1);

	update = true;
}

void UIClass::rpm(uint8_t value)
{
	// RPM
	CURSOR(RPM_POS_X, RPM_POS_Y);
	SIZE(2);
	leadingZero(value, 3, true);
	SIZE(1);

	update = true;
}

void UIClass::showInfo(uint8_t ID)
{
	UI_ClearInfo();
	CURSOR(INFO_POS_X, INFO_POS_Y);
	if (ID < 4) showDistance(ID);
	else if (ID < 6) showStat(ID - 4);
	else if (ID < 8) showSpeed(ID - 6);
	else showTime(ID - 8);
	update = true;
}

void UIClass::showStat(uint8_t ID) const
{
	char const *title[MAX_STATS] = {
		"AVG DIST",
		"AVG SPEED"
	};
	float avg = 0.00;

	if (ID == 0)
	{
		if (Settings.getSetting8(UNIT) == METRIC) avg = (stats[0] / (float)10000000.00) / (times[0] / 3600);
			else avg = (stats[0] / (float)52800000.00) / (times[0] / 3600);
	}
	else avg = stats[1] / (times[1] + 1);

	Screen.println(title[ID]);
	Screen.print(avg, 1);
	if (Settings.getSetting8(UNIT) == METRIC) Screen.print("km/h");
		else Screen.print("mi/h");
}

void UIClass::showSpeed(uint8_t ID) const
{
	char const *title[MAX_SPEEDS] = {
		"TOP SPEED",
		"MAX SPEED"
	};

	Screen.println(title[ID]);
	Screen.print(speeds[ID]);
	if (Settings.getSetting8(UNIT) == METRIC) Screen.print(""SPEED_UNIT_METRIC"");
		else Screen.print(""SPEED_UNIT_IMPERIAL"");
}

void UIClass::showTime(uint8_t ID) const
{
	char const *title[3] = {
		"TOTAL TIME",
		"TRIP TIME",
		"WORK TIME"
	};

	uint16_t hours = times[ID + 2] / 3600;
	uint8_t minutes = (times[ID + 2] % 3600) / 60;

	Screen.println(title[ID]);
	if (hours != 0)
	{
		leadingZero(hours, 5, true);
		Screen.print("h ");
	}
	leadingZero(minutes, 2, true);
	Screen.print("min");
}

void UIClass::showDistance(uint8_t ID) const
{
	float dist = 0.00;
	char const *title[4] = {
		"TOTAL",
		"TRIP",
		"TRIP/DAY",
		"TRIP/MONTH"
	};

	if (Settings.getSetting8(UNIT) == METRIC) dist = m2km((float)distances[ID]);
		else dist = ft2mi((float)distances[ID]);

	Screen.println(title[ID]);
	leadingZero((int)dist, 6, false);
	Screen.print(dist, 2);
	if (Settings.getSetting8(UNIT) == METRIC) Screen.print("km");
		else Screen.print("mi");
}

void UIClass::showServo(uint8_t angle)
{
	angle = map(angle, MIN_HL_ANGLE, MAX_HL_ANGLE, 0, SERVO_BAR_H);
	if (angle < SERVO_LEVEL_H) angle = SERVO_LEVEL_H;
	else if (angle > SERVO_LEVEL_H && angle < (SERVO_BAR_H - SERVO_LEVEL_H)) angle += 2;

	UI_ClearServo();
	Screen.drawRect(SERVO_BAR_POS_X, SERVO_BAR_POS_Y, SERVO_BAR_W, SERVO_BAR_H, WHITE);
	Screen.fillRect(SERVO_LEVEL_POS_X, 64 - angle, SERVO_LEVEL_W, SERVO_LEVEL_H, WHITE);

	update = true;
}

void UIClass::refresh(void)
{
	if (update) Screen.display();
	update = false;
}

void UIClass::leadingZero(uint32_t const value, uint8_t const maxDigits, bool printVal) const
{
	int8_t i = 0;
	
	if (value < 10) i = maxDigits - 1;
	else if (value < 100) i = maxDigits - 2;
	else if (value < 1000) i = maxDigits - 3;
	else if (value < 10000) i = maxDigits - 4;
	else if (value < 100000) i = maxDigits - 5;
	else if (value < 1000000) i = maxDigits - 6;
	else if (value < 10000000) i = maxDigits - 7;
	else if (value < 100000000) i = maxDigits - 8;
	else if (value < 1000000000) i = maxDigits - 9;
	else if (value < 10000000000) i = maxDigits - 8;
	if (i < 0) i = 1; // JUST IN CASE

	while (i != 0)
	{
		Screen.print('0');
		i--;
	}
	if (printVal) Screen.print(value);
}