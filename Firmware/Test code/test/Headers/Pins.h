
/*

	PINS HEADER FILE
	LAST UPDATE: 17. 4. 2019.

*/


#ifndef _PINS_H
#define _PINS_H


// BUTTONS
#define BTN_ENTER																	PB13
#define BTN_ENTER_MODE 																INPUT_PULLUP
#define BTN_HB																		PB15
#define BTN_HB_MODE 																INPUT_PULLUP
#define BTN_LI																		PA5
#define BTN_LI_MODE 																INPUT_PULLUP
#define BTN_BACK																	PA6
#define BTN_BACK_MODE 																INPUT_PULLUP
#define BTN_LTS																		PA7
#define BTN_LTS_MODE 																INPUT_PULLUP
#define BTN_RTS																		PA8
#define BTN_RTS_MODE 																INPUT_PULLUP


// LIGHTS
#define LED_RTS 																	PA11
#define LED_RTS_MODE 																OUTPUT
#define LED_LTS 																	PA12
#define LED_LTS_MODE 																OUTPUT
#define LED_HB 																		PB6
#define LED_HB_MODE 																PWM
#define LED_HL 																		PB7
#define LED_HL_MODE 																PWM
#define LED_BR_LI 																	PB8
#define LED_BR_LI_MODE 																PWM
#define LED_TL 	 																	PB9
#define LED_TL_MODE 																PWM


// SENSORS
#define SEN_SPEED 																	PB12
#define SEN_SPEED_MODE																INPUT // INTERRUPT
#define SEN_RPM 																	PB14
#define SEN_RPM_MODE																INPUT // INTERRUPT
#define SEN_FR_BR 																	PA0
#define SEN_FR_BR_MODE																INPUT_ANALOG
#define SEN_RE_BR 																	PA1
#define SEN_RE_BR_MODE																INPUT_ANALOG
#define SEN_AMB 																	PA2
#define SEN_AMB_MODE																INPUT_ANALOG
#define SEN_BATT 																	PA4
#define SEN_BATT_MODE																INPUT_ANALOG
#define SEN_BATT_TEMP 																PA3
#define SEN_BATT_TEMP_MODE															INPUT_ANALOG
#define SEN_KS 	 																	PB4
#define SEN_KS_MODE 																INPUT


// MISC
#define BUZZER 																		PB1
#define BUZZER_MODE 																PWM



#endif // _PINS_H