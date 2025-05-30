#include <asf.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
// #include "avr_compiler.h"

#include "led.h"
#include "joystick.h"
// #include "conf_usb.h"
#include "udi_hid_generic.h"
#include "76319_io_initialization.h"

#define SLIDER_COUNT   12

uint8_t jstk_mask;
volatile uint8_t jstk_testMode;


static int8_t jstk_scan(uint16_t jstk_bits) {
    for (int8_t i = 0; i < SLIDER_COUNT; i++)
        if ((jstk_bits & (1u << i)) == 0)
            return i;
    return -1;
}


// vertical slider
static uint16_t jstk_readVertRaw(void) { // mostly copied from 76319_keyboard.c
    uint8_t jstk_c = PORTC.IN;
    uint8_t jstk_d = PORTD.IN;
    uint16_t jstk_w = ((uint16_t)jstk_d << 8) | jstk_c;
    jstk_w >>= 2;            // discard C0 & C1
    return jstk_w & 0x0FFF;  // keep only bits 0–11
}

int8_t jstk_readVertIndex(void) {
    int8_t idx = jstk_scan(jstk_readVertRaw());
    if (idx >= 0)
        idx = 11 - idx;
    return idx;
}

// horizontal slider
static uint16_t jstk_readHoriRaw(void) { // mostly copied from 76319_keyboard.c
    uint8_t jstk_e = PORTE.IN;
    uint8_t jstk_b = PORTB.IN;
    uint16_t jstk_w = ((uint16_t)jstk_b << 8) | jstk_e;
    return jstk_w & 0x0FFF;	// B4–B7 land above bit11 and get masked off
}

int8_t jstk_readHoriIndex(void) {
    int8_t idx = jstk_scan(jstk_readHoriRaw());
    if (idx >= 0)
        idx = 11 - idx;
    return idx;
}


// joystick USB stuff
static const uint8_t jstk_idx2axis[12] =
{
    0, 23, 46, 69,
    92, 116, 139, 162,
    185, 208, 231, 255
};

uint8_t jstk_idxToAxis(int8_t idx) {
    if (idx < 0)
        return 128;
    return jstk_idx2axis[(uint8_t)idx];
}


uint8_t jstk_readMask(void)
{
    int8_t vi = jstk_readVertIndex();
    int8_t hi = jstk_readHoriIndex();

    if (vi < 0 && hi < 0)
        return 0;

    uint8_t dV = (vi < 0) ? 0 : (vi > 5 ? vi - 5 : 5 - vi);
    uint8_t dH = (hi < 0) ? 0 : (hi > 5 ? hi - 5 : 5 - hi);

    int8_t jstk_use = (dV >= dH) ? vi : hi;

    return jstk_ledMask(jstk_use);
}

uint8_t jstk_ledMask(uint8_t idx)
{
    if (idx < 0)    // no touch detected
        return 0;

    if (idx == 5 || idx == 6)   // center dead‑zone
        return (1u<<3) | (1u<<4);  // LED4 (bit3) + LED5 (bit4)

    // computes 'distance' from center [5,6]
    uint8_t d = (idx < 5) ? (5 - idx) : (idx - 6);

    // controls how many LED's should activate
    uint8_t N = (d < 2 ? 2 : (d + 1));
    if (N > 4) N = 4;

    uint8_t jstk_mask = 0;
    if (idx < 5)    // down/left LED joystick
        for (uint8_t i = 0; i < N; i++)
            jstk_mask |= (1u << (3 - i));
    else    // up/right LED joystick
        for (uint8_t i = 0; i < N; i++)
            jstk_mask |= (1u << (4 + i));
    return jstk_mask;
}

static uint8_t jstk_usbReport[2];
static uint8_t jstk_prevReport[2] = {128, 128};

void jstk_usbTask(void)
{
    // sample current joystick/slider indices
    jstk_usbReport[0] = jstk_idxToAxis(jstk_readHoriIndex());
    jstk_usbReport[1] = jstk_idxToAxis(jstk_readVertIndex());

    // send if value changed & endpoint ready
    if ((jstk_usbReport[0] != jstk_prevReport[0]) || (jstk_usbReport[1] != jstk_prevReport[1])) {
        if (udi_hid_generic_send_report_in(jstk_usbReport)) {
            jstk_prevReport[0] = jstk_usbReport[0];
            jstk_prevReport[1] = jstk_usbReport[1];
        }
    }
}

void joystick(void) 
{
    jstk_mask = jstk_readMask();
    jstk_testMode = PORTB.IN;

    if ((jstk_testMode & PIN4_bm) == 0) {
        if (jstk_mask) {
            led_allOff();
            led_on(jstk_mask);
            _delay_ms(10);
        }
    }

    jstk_usbTask();
}