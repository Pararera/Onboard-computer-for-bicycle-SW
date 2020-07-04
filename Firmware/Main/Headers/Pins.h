
/*

	PINS HEADER FILE

*/


#ifndef PINS_H
#define PINS_H


// BUTTONS
#define BTN_ENTER																	PA4
#define BTN_ENTER_MODE 																INPUT_PULLUP // INPUT_PULLUP
#define BTN_BACK																	PB4
#define BTN_BACK_MODE 																INPUT_PULLUP // INPUT_PULLUP
#define BTN_LIGHTS																	PA5
#define BTN_LIGHTS_MODE 															INPUT_PULLUP // INPUT_PULLUP
#define BTN_HIGH_BEAM																PB5
#define BTN_HIGH_BEAM_MODE 															INPUT_PULLUP // INPUT_PULLUP
#define BTN_LEFT_TS																	PA6
#define BTN_LEFT_TS_MODE 															INPUT_PULLUP // INPUT_PULLUP
#define BTN_RIGHT_TS																PA7
#define BTN_RIGHT_TS_MODE 															INPUT_PULLUP // INPUT_PULLUP

// LIGHTS
#define LED_HEAD 																	PB6
#define LED_HEAD_MODE 																PWM // PWM
#define LED_HB_LEFT 																PB7
#define LED_HB_LEFT_MODE 															PWM // PWM
#define LED_HB_RIGHT 																PB8
#define LED_HB_RIGHT_MODE 															PWM // PWM
#define LED_TAIL1 																	PB9
#define LED_TAIL1_MODE 																PWM // PWM
#define LED_TAIL2 																	PB12
#define LED_TAIL2_MODE 																OUTPUT // OUTPUT

// INDICATORS
#define IND_BATT 																	PB13
#define IND_BATT_MODE 																INPUT_PULLUP // INPUT_PULLUP
#define IND_LEFT_TS 																PA12
#define IND_LEFT_TS_MODE  															OUTPUT // OUTPUT
#define IND_RIGHT_TS 																PA11
#define IND_RIGHT_TS_MODE  															OUTPUT // OUTPUT
#define IND_RPM_LED 																PB3
#define IND_RPM_LED_MODE  															OUTPUT // OUTPUT
#define IND_SPEAKER 																PB1
#define IND_SPEAKER_MODE  															PWM // PWM

// SENSORS
#define SEN_SPEED																	PB14
#define SEN_SPEED_MODE 																INPUT // INPUT
#define SEN_RPM																		PA15
#define SEN_RPM_MODE 																INPUT // INPUT
#define SEN_FRONT_BR																PA0
#define SEN_FRONT_BR_MODE 															INPUT_ANALOG // INPUT_ANALOG
#define SEN_REAR_BR																	PA1
#define SEN_REAR_BR_MODE 															INPUT_ANALOG // INPUT_ANALOG
#define SEN_LIGHT																	PA2
#define SEN_LIGHT_MODE 																INPUT_ANALOG // INPUT_ANALOG

// CONTROLS
#define CONT_SERVO 																	PA8
#define CONT_SERVO_MODE 															PWM // PWM


#endif // PINS_H