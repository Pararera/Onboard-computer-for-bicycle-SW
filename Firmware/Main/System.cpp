
/*

	SYSTEM FUNCTIONS

*/


// LIBRARIES
#include 																			<Arduino.h>

// MODULES
#include 																			"Headers\Settings.h"
#include 																			"Headers\System.h"
#include 																			"Headers\Main.h"
#include 																			"Headers\Macros.h"
#include 																			"Headers\Pins.h"
#include 																			"Headers\UI.h"



// OBJECT
SystemClass System;



// (DE)CONSTRUCTOR
SystemClass::SystemClass()
{

}

SystemClass::~SystemClass()
{

}


// FUNCTIONS
void SystemClass::init(void)
{
	lastSave = lastTDT = lastActivity = millis();
	lastAmbientCheck = millis();
}

void SystemClass::btnScan(void)
{
	if (digitalRead(BTN_ENTER) == LOW)
	{
		if (btnPressedTick[ENTER] == 0)
		{
			btnPressedTick[ENTER] = millis() * (-1);
			lastActivity = millis();
			#ifdef SOUND_BTN
				playBtnSound();
			#endif
		}
	}
	else if (btnPressedTick[ENTER] < 0) btnPressedTick[ENTER] *= -1;

	if (digitalRead(BTN_BACK) == LOW)
	{
		if (btnPressedTick[BACK] == 0)
		{
			btnPressedTick[BACK] = millis() * (-1);
			lastActivity = millis();
			#ifdef SOUND_BTN
				playBtnSound();
			#endif
		}
	}
	else if (btnPressedTick[BACK] < 0) btnPressedTick[BACK] *= -1;

	if (digitalRead(BTN_LEFT_TS) == LOW)
	{
		if (btnPressedTick[LTS] == 0)
		{
			btnPressedTick[LTS] = millis() * (-1);
			lastActivity = millis();
			#ifdef SOUND_BTN
				playBtnSound();
			#endif
		}
	}
	else if (btnPressedTick[LTS] < 0) btnPressedTick[LTS] *= -1;

	if (digitalRead(BTN_RIGHT_TS) == LOW)
	{
		if (btnPressedTick[RTS] == 0)
		{
			btnPressedTick[RTS] = millis() * (-1);
			lastActivity = millis();
			#ifdef SOUND_BTN
				playBtnSound();
			#endif
		}
	}
	else if (btnPressedTick[RTS] < 0) btnPressedTick[RTS] *= -1;

	if (digitalRead(BTN_LIGHTS) == LOW)
	{
		if (btnPressedTick[LI] == 0)
		{
			btnPressedTick[LI] = millis() * (-1);
			lastActivity = millis();
			#ifdef SOUND_BTN
				playBtnSound();
			#endif
		}
	}
	else if (btnPressedTick[LI] < 0) btnPressedTick[LI] *= -1;
}

void SystemClass::btn(void)
{

}

void SystemClass::playBtnSound(void) const
{
	uint8_t soundIndex = Settings.getSetting8(BTN_SOUND);
	if (soundIndex != 0 && Settings.getSetting8(SOUNDS) == YES) tone(IND_SPEAKER, sounds[soundIndex - 1], BTN_SOUND_DURATION);
}

void SystemClass::playTSSound(void) const
{
	uint8_t soundIndex = Settings.getSetting8(TS_SOUND);
	if (soundIndex != 0 && Settings.getSetting8(SOUNDS) == YES) tone(IND_SPEAKER, sounds[soundIndex - 1], BTN_SOUND_DURATION);
}

void SystemClass::playAlertSound(void) const
{
	uint8_t soundIndex = Settings.getSetting8(ALERT_SOUND);
	if (soundIndex != 0 && Settings.getSetting8(SOUNDS) == YES) tone(IND_SPEAKER, sounds[soundIndex - 1], BTN_SOUND_DURATION);
}

uint8_t SystemClass::isDark(void) const
{
	if (analogRead(SEN_LIGHT) < DARK_THRESHOLD_VALUE) return (DARK);
		else return (LIGHT);
}

void SystemClass::dimScreen(void) const
{
	if (Settings.getSetting8(SCR_DIM) == AUTO_SCR_DIM) Screen.dim(System.isDark());
		else Screen.dim(Settings.getSetting8(SCR_DIM));
}

void SystemClass::save(void)
{
	if (millis() - lastSave > SAVE_TIME)
	{
		if (millis() - lastActivity <= Settings.getSetting16(SAVER_TIME)) // NOT IN SAVER MODE
		{
			for (uint8_t i = 0; i < MAX_DISTANCES; i++) EEPROM.write48(EEPROM_DISTANCES + (i * 6), distances[i]);
			for (uint8_t i = 0; i < MAX_STATS; i++) EEPROM.write48(EEPROM_STATS + (i * 6), stats[i]);
			for (uint8_t i = 0; i < MAX_TIMES; i++) EEPROM.write32(EEPROM_TIMES + (i * 4), times[i]);

			DEBUG("Nije u save modu");	
		}
		else
		{
			for (uint8_t i = 0; i < MAX_TIMES; i++) EEPROM.write32(EEPROM_TIMES + (i * 4), times[i]);
			DEBUG("U save modu je");
		}
	

		lastSave = millis();
	}
}

void SystemClass::updateTDT(void)
{
	if (millis() - lastTDT > CLOCK_UPDATE)
	{
		UI.updateTDT();		
		lastTDT = millis();
	}
}

void SystemClass::saver(void)
{
	// BATTERY SAVER
	if (System_LowBattery()) // BATTERY IS LOW
	{
		UI.icon(ICON_BATTERY, true, true);

		#ifdef SOUND_BATT_SAVER
			playAlertSound();
		#endif

		if (Settings.getSetting8(BATT_SAVER) == ON)
		{
			// LOW SAVER LEVEL ACTIONS

			if (Settings.getSetting8(BATT_SAVER_LVL) == HIGH)
			{
				// HIGH SAVER LEVEL ACTIONS
			}

			battSaver = true;			
		}
	}

	// SCREEN SAVER
	if (Settings.getSetting8(BATT_SAVER) == ON)
	{
		if (millis() - lastActivity > Settings.getSetting16(SAVER_TIME) && Settings.getSetting16(SAVER_TIME) != 0) // IN SAVER MODE & SAVER IS ON
		{
			if (states[SCREEN_STATE] == SCREEN_ON)
			{
				states[SCREEN_STATE] = SCREEN_OFF_SYS;
				Screen.SH1106_command(SH1106_DISPLAYOFF);

				#ifdef SOUND_SAVER_ON
					playAlertSound();
				#endif
			}
		}
		else if (states[SCREEN_STATE] == SCREEN_OFF_SYS && !battSaver)
		{
			states[SCREEN_STATE] = SCREEN_ON;
			Screen.SH1106_command(SH1106_DISPLAYON);

			#ifdef SOUND_SAVER_OFF
				playAlertSound();
			#endif	
		}		
	}
}

void SystemClass::brakeLight(void)
{
	if (analogRead(SEN_FRONT_BR) > Settings.getSetting16(FRONT_BRAKE) || analogRead(REAR_BRAKE) > Settings.getSetting16(REAR_BRAKE)) // BRAKE IS PRESSED
	{
		if (!brakeLightStatus) // FIRST BRAKE FLASH
		{
			brakeLightStatus = true;
			brakeLiFlash = true;
			brakeLightFlash();
		}
		else if (states[FL_STATE] == LOW) // FOG LIGHT IS OFF
		{
			if (millis() - lastBrLiFlash > BRAKE_FLASH_TIME) // NEW FLASH
			{
				if (Settings.getSetting8(BRAKE_LIGHT) != 0) // BRAKE LIGHT WITH FLASHES
				{
					if (brakeLightFlashes < Settings.getSetting8(BRAKE_LIGHT)) // FLASH BRAKE LIGHT
					{
						brakeLiFlash = !brakeLiFlash;
						brakeLightFlash();
					}
					else // HOLD BRAKE LIGHT
					{
						brakeLiFlash = true;
						brakeLightFlash();
						brakeLightFlashes = 255; // JUST IN CASE OF LONG BRAKING TIME
					}
				}
			}
		}
		else if (millis() - lastBrLiFlash > BRAKE_FLASH_TIME) // FOG LIGHT IS ON & IT'S TIME FOR NEW FLASH
		{
			brakeLiFlash = !brakeLiFlash;
			brakeLightFlash();
		}
	}
	else if (brakeLightStatus) // BRAKE LIGHT GOES OFF
	{
		brakeLightFlashes = 0;
		brakeLightStatus = brakeLiFlash = false;
		brakeLightFlash();
	}
}

void SystemClass::brakeLightFlash(void)
{
	pwmWrite(LED_TAIL1, brakeLiFlash * BRAKE_LIGHT_BRGH);
	digitalWrite(LED_TAIL2, brakeLiFlash);
	lastBrLiFlash = millis();

	if (brakeLiFlash) brakeLightFlashes++; // IT'S ON
	else // IT'S OFF
	{
		// CORRECTIONS
		if (states[LI_STATE] == DAILY_LI) pwmWrite(LED_TAIL1, Settings.getSetting8(TL_DAILY_BRGH));
		else if (states[LI_STATE] == NORMAL_LI) pwmWrite(LED_TAIL1, Settings.getSetting8(TL_BRGH));
		else if (states[LI_STATE] == AUTO_LI && autoLi) pwmWrite(LED_TAIL1, Settings.getSetting8(TL_BRGH));

		if (states[FL_STATE] == HIGH) pwmWrite(LED_TAIL1, Settings.getSetting8(TL_FOG_BRGH));
	}
}

void SystemClass::ambient(void)
{
	if (millis() - lastAmbientCheck > AMBIENT_CHECK)
	{
		if (isDark())
		{
			ambientChecks++;
			if (ambientChecks == MAX_AMBIENT_CHECKS) // YES, IT'S DARK FOR SURE
			{
				// SCREEN DIM
				if (Settings.getSetting8(SCR_DIM) == AUTO_SCR_DIM) Screen.dim(true);

				// AUTO LIGHT
				if (states[LI_STATE] == AUTO_LI)
				{
					autoLi = true;
					pwmWrite(LED_HEAD, Settings.getSetting8(HL_BRGH));
					UI.icon(ICON_AUTO_LI_ON, true, true);
				}
			}
		}
		else
		{
			ambientChecks--;
			if (ambientChecks == 0) // YES, IT'S NOT DARK ANYMORE
			{
				// SCREEN DIM
				if (Settings.getSetting8(SCR_DIM) == AUTO_SCR_DIM) Screen.dim(false);

				// AUTO LIGHT
				if (states[LI_STATE] == AUTO_LI)
				{
					autoLi = true;
					pwmWrite(LED_HEAD, Settings.getSetting8(HL_BRGH));
					UI.icon(ICON_AUTO_LI_OFF, true, true);
				}
			}
		}

		lastAmbientCheck = millis();
	}
}


// GET/SET FUNCTIONS
void SystemClass::setAutoLight(bool value)
{
	autoLi = value;
}

void SystemClass::setState(uint8_t index, uint8_t value)
{
	states[index] = value;
}

uint8_t SystemClass::getState(uint8_t index) const
{
	return (states[index]);
}