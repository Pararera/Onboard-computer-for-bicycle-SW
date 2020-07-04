
/*

	SETTINGS FUNCTIONS

*/


// MODULES
#include 																			"Headers\Settings.h"



// OBJECT
SettingsClass Settings;



// (DE)CONSTRUCTOR
SettingsClass::SettingsClass()
{

}

SettingsClass::~SettingsClass()
{

}


// FUNCTIONS
void SettingsClass::set8(uint8_t index, uint8_t value)
{
	settings8[index] = value;
}

void SettingsClass::setSetting8(int8_t value, uint8_t index, uint8_t minValue, uint8_t maxValue, bool zero)
{
	int16_t newValue = settings8[index] + (value);

	if (newValue > maxValue) settings8[index] = !zero * minValue;
	else if (newValue < minValue)
	{
		if (zero)
		{
			if (newValue < 0) settings8[index] = maxValue;
			else if (value > 0) settings8[index] = minValue;
			else settings8[index] = 0;
		}
		else settings8[index] = maxValue;
	}
	else settings8[index] = newValue;
}

uint8_t SettingsClass::getSetting8(uint8_t index) const
{
	return (settings8[index]);
}

void SettingsClass::set16(uint8_t index, uint16_t value)
{
	settings16[index] = value;
}

void SettingsClass::setSetting16(int16_t value, uint8_t index, uint16_t minValue, uint16_t maxValue, bool zero)
{
	int32_t newValue = settings16[index] + (value);

	if (newValue > maxValue) settings16[index] = !zero * minValue;
	else if (newValue < minValue)
	{
		if (zero)
		{
			if (newValue < 0) settings16[index] = maxValue;
			else if (value > 0) settings16[index] = minValue;
			else settings16[index] = 0;
		}
		else settings16[index] = maxValue;
	}
	else settings16[index] = newValue;
}

uint16_t SettingsClass::getSetting16(uint8_t index) const
{
	return (settings16[index]);
}

void SettingsClass::setHLProfile(uint8_t profileID, uint8_t angle)
{
	DEBUG_VAL("ID: ", profileID);
	DEBUG_VAL("Angle: ", angle);
	headLightProfiles[profileID] = angle;
}

uint8_t SettingsClass::getHLProfile(uint8_t profileID) const
{
	return (headLightProfiles[profileID]);
}
