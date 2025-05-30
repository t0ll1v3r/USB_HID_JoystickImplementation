#ifndef JOYSTICK_H
#define JOYSTICK_H

// #include <asf.h>  // brings in device headers for XMEGA
// #include <avr/io.h>
#include <stdint.h>
#include "udi_hid_generic.h"


// function prototypes
void joystick(void);

int8_t jstk_readVertIndex(void);
int8_t jstk_readHoriIndex(void);
uint8_t jstk_readMask(void);

uint8_t jstk_ledMask(uint8_t percent);
uint8_t jstk_idxToAxis(int8_t idx);	// 0-11 -> 0-255

void jstk_usbTask(void);	// build and send 2 byte report

#endif // JOYSTICK_H