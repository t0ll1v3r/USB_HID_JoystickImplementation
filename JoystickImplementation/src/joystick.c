#include <asf.h>
#include <util/delay.h>

#include "led.h"
#include "joystick.h"
#include "udi_hid_generic.h"

#define SLIDER_COUNT   12

uint8_t jstk_mask;  // bitmask of LED's to turn on
volatile uint8_t jstk_testMode;


static int8_t jstk_scan(uint16_t jstk_bits) {
    for (int8_t i = 0; i < SLIDER_COUNT; i++)   // iterates through slider
        if ((jstk_bits & (1u << i)) == 0)       // active when low
            return i;                           // returns active pad index
    return -1;                                  // nothing being touched
}

/*
 * The sliders are really just 12 buttons which are pressed as you move your finger up/down or left/right.
 * Similar to sweeping your fingers across the keys of a piano.
 */

// vertical slider
static uint16_t jstk_readVertRaw(void) {
    uint8_t jstk_c = PORTC.IN;
    uint8_t jstk_d = PORTD.IN;
    uint16_t jstk_w = ((uint16_t)jstk_d << 8) | jstk_c; // build 16 bit word
    jstk_w >>= 2;                                       // discard C0 & C1
    return jstk_w & 0x0FFF;                             // keep only bits 0–11
}   // only return C2-C7 and D0-D5

int8_t jstk_readVertIndex(void) {
    int8_t idx = jstk_scan(jstk_readVertRaw());
    // if (idx >= 0)
    //     idx = 11 - idx;
    return idx;
}

// horizontal slider
static uint16_t jstk_readHoriRaw(void) {
    uint8_t jstk_e = PORTE.IN;
    uint8_t jstk_b = PORTB.IN;
    uint16_t jstk_w = ((uint16_t)jstk_b << 8) | jstk_e;
    return jstk_w & 0x0FFF;	// B4–B7 = bits 12-15, so they get discarded
}

int8_t jstk_readHoriIndex(void) {
    int8_t idx = jstk_scan(jstk_readHoriRaw());
    // if (idx >= 0)
    //     idx = 11 - idx;
    return idx;
}


// joystick USB stuff
static const uint8_t jstk_idx2axis[12] = {
    0,      23,     46,     69,
    92,     116,    139,    162,
    185,    208,    231,    255
};  // lookup table for the 12 discrete slider positions to avoid long division

uint8_t jstk_idxToAxis(int8_t idx) {
    if (idx < 0)
        return 128; // return to center when no contact
    return jstk_idx2axis[idx];
}   // conversion runtime is O(1)


uint8_t jstk_readMask(void)
{
    int8_t vi = jstk_readVertIndex();       // -1 to 11
    int8_t hi = jstk_readHoriIndex();       // -1 to 11

    if (vi < 0 && hi < 0)
        return 0;                           // no contact

    // decide which slider is moved furthest from center buy computing 'distance' from middle
    uint8_t dV = (vi < 0) ? 0 : (vi > 5 ? vi - 5 : 5 - vi); // vertical slider 'distance'   (dV)
    uint8_t dH = (hi < 0) ? 0 : (hi > 5 ? hi - 5 : 5 - hi); // horizontal slider 'distance' (dH)

    int8_t jstk_use = (dV >= dH) ? vi : hi; // slider with greatest distance wins

    return jstk_ledMask(jstk_use);          // convert to bits
}   // basically just prioritizes whichever axis is moving more

uint8_t jstk_ledMask(int8_t idx)
{
    if (idx < 0)    // no touch detected
        return 0;

    if (idx == 5 || idx == 6)   // center zone
        return (1u<<3) | (1u<<4);  // LED4 (bit3) + LED5 (bit4)

    uint8_t d = (idx < 5) ? (5 - idx) : (idx - 6);  // computes 'distance' from center (d)
    /*
    idx: 0 1 2 3 4 | 5 6 | 7 8 9 10 11
    d:   5 4 3 2 1 |  -  | 1 2 3  4  5
    */

    uint8_t N = (d < 2 ? 2 : (d + 1));  // decide how many LED's should activate (N)
    if (N > 4) N = 4;
    /*
    d: 1 2 3 4 5
    N: 2 3 4 4 4
    */

    uint8_t jstk_mask = 0;
    if (idx < 5)                            // down/left direction
        for (uint8_t i = 0; i < N; i++)
            jstk_mask |= (1u << (3 - i));   // LED's 3-0
    else                                    // up/right direction
        for (uint8_t i = 0; i < N; i++)
            jstk_mask |= (1u << (4 + i));   // LED's 4-7
    return jstk_mask;
}

static uint8_t jstk_usbReport[2];
static uint8_t jstk_prevReport[2] = {128, 128};

void jstk_usbTask(void)
{
    // sample current joystick/slider indices
    jstk_usbReport[0] = jstk_idxToAxis(jstk_readHoriIndex());   // x
    jstk_usbReport[1] = jstk_idxToAxis(jstk_readVertIndex());   // y

    // send if value changed & IN endpoint ready
    if ((jstk_usbReport[0] != jstk_prevReport[0]) || (jstk_usbReport[1] != jstk_prevReport[1])) { // value changed?
        if (udi_hid_generic_send_report_in(jstk_usbReport)) {   // IN endpoint ready?
            jstk_prevReport[0] = jstk_usbReport[0];
            jstk_prevReport[1] = jstk_usbReport[1];
        }
    }
}

void joystick(void) 
{
    jstk_mask = jstk_readMask();            // pick LED's
    jstk_testMode = PORTB.IN;               // checks switch for testing mode

    if ((jstk_testMode & PIN4_bm) == 0) {   // test mode
        if (jstk_mask) {
            led_allOff();
            led_on(jstk_mask);
            _delay_ms(10);
        }
    } else {                                // normal mode
        led_allOff();                       // !! probably remove this later !!
        jstk_usbTask();                     // send to USB
    }
}