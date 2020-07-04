
#include 																			<Wire.h>
#include 																			<RTClib.h>
#include                                        									<Adafruit_GFX.h> 
#include                                        									<Adafruit_SH1106_STM32.h>
#include 																			<AT24CXX_STM32.h>
#include 																			<string.h>

#include 																			"Headers\Addresses.h"
#include 																			"Headers\Pins.h"
#include 																			"Headers\Macros.h"


HardwareTimer timer(4);
Adafruit_SH1106 Screen(4);
RTC_DS3231 RTC;

volatile bool rpmSen = false;
volatile bool speedSen = false;

#define SERIAL_BUFFER 128
char serialData[128];


const uint8_t pins[21] = {
	BTN_ENTER,
	BTN_HB,
	BTN_LI,
	BTN_BACK,
	BTN_LTS,
	BTN_RTS,

	LED_RTS,
	LED_LTS,
	LED_HB,
	LED_HL,
	LED_BR_LI,
	LED_TL,

	SEN_SPEED,
	SEN_RPM,
	SEN_FR_BR,
	SEN_RE_BR,
	SEN_AMB,
	SEN_BATT,
	SEN_BATT_TEMP,
	SEN_KS,

	BUZZER
};

const WiringPinMode modes[21] = {
	BTN_ENTER_MODE,
	BTN_HB_MODE,
	BTN_LI_MODE,
	BTN_BACK_MODE,
	BTN_LTS_MODE,
	BTN_RTS_MODE,

	LED_RTS_MODE,
	LED_LTS_MODE,
	LED_HB_MODE,
	LED_HL_MODE,
	LED_BR_LI_MODE,
	LED_TL_MODE,

	SEN_SPEED_MODE,
	SEN_RPM_MODE,
	SEN_FR_BR_MODE,
	SEN_RE_BR_MODE,
	SEN_AMB_MODE,
	SEN_BATT_MODE,
	SEN_BATT_TEMP_MODE,
	SEN_KS_MODE,

	BUZZER_MODE
};


void process(char *data)
{
	uint64_t cmdValue = 0;
	char *cmdType;

	cmdType = strtok(data, "=");
	cmdValue = atoi(strtok(NULL, "="));

	Serial.println(cmdType);
	Serial.println(cmdValue, DEC);

	Screen.clearDisplay();
	Screen.setCursor(0, 0);
	Screen.print("Command: ");
	Screen.println(cmdType);
	Screen.print("Value: ");
	Screen.println(cmdValue);
	Screen.display();
} 

void buff(const uint8_t buffByte)
{
	static char dataBuff[SERIAL_BUFFER];
	static uint8_t buffIdx = 0;

	switch (buffByte)
	{
		case '\n':
		{
			dataBuff[buffIdx] = 0;
			buffIdx = 0;

			uint32_t tick = millis();
			process(dataBuff); // CCA 1ms WITHOUT SERIAL AND SCREEN. 100 TIMES CALLED.
			Serial.println(millis() - tick, DEC);
			break;
		}

		case '\r': break;

		default:
		{
			if (buffIdx < (SERIAL_BUFFER - 1)) dataBuff[buffIdx++] = buffByte;
			break;
		}
	}
}

void wheeleRev()
{
	speedSen = true;
}

void rev()
{
	rpmSen = true;
}

void setup()
{
	delay(100); // DO NOT DELETE
	for (uint8_t i = 0; i < 21; i++) pinMode(pins[i], modes[i]);
	attachInterrupt(SEN_SPEED, wheeleRev, RISING);
	attachInterrupt(SEN_RPM, rev, RISING);
	Serial.begin(115200);

	Serial.println("Hello, World!");

	noTone(BUZZER);
	Wire.begin();

	Serial.println("WIRED!");

	EEPROM.init(AT24CXX, 32, 5);

	Serial.println(RTC.temp(), DEC);
	Serial.println(EEPROM.read(10, 8), DEC);

	Screen.begin(SH1106_SWITCHCAPVCC, SH1106);
	Screen.clearDisplay();
	timer.setPrescaleFactor(1);
	timer.setOverflow(255); // SETS PWM FREQ. TO 282kHz (TIMER 4)

	// OLED SCREEN TEST
	Screen.setCursor(0, 0);
	Screen.setTextColor(WHITE, BLACK);
	Screen.println("Hello, World!");
	Screen.display();

	// EEPROM TEST
/*	uint64_t testBytes[10] = { 100, 1000, 20000, 300000, 40000, 5000000, 6000000, 7000000, 80000000, 90000000 }; // 80 BYTES IN TOTAL

	Serial.println("Page write");
	EEPROM.writePage(10, reinterpret_cast<char*>(&testBytes[0]), sizeof(testBytes));

	Serial.println("\nRead");
	for (uint8_t i = 0; i < 10; i++) Serial.println(EEPROM.read(10 + (i * 8), 8), DEC);*/
}

void loop()
{
	while (Serial.available()) buff(Serial.read());
}