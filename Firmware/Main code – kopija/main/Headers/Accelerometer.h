
/*

	ACCELEROMETER HEADER FILE

*/


#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H


// LIBRARIES
#include 																			<stdint.h>
#include 																			<Wire.h>

// MODULES
#include 																			"Addresses.h"



// CLASS
class AccelClass {
public:
	float leanAngle(void);
	void init(void);

private:
	void read(uint8_t,  uint8_t,  uint8_t,  uint8_t*);
	void write(uint8_t,  uint8_t,  uint8_t);
};



// OBJECT
extern AccelClass Accelerometer;


#endif // ACCELEROMETER_H