//  ********************************************************************
/*
 * Author: Rex.Walters
 * Purpose - Project 101202 EVi Classic
 * Firmware - For P/N 76319 EVI Classic Front Panel Interface PCBA
 * File 76319_ui.h
 *
 *----------------------------------------
 * 76319 Firmware Common Definitions -  Revisions
 * Revision 0
 * Created / Started March 7, 2024
 * Finished (Place Date here)
 *----------------------------------------
*/


/****************************************************/
/***************** Function Prototypes **************/
/****************************************************/
void BD76319_KeyToReport(uint16_t pui_KeyStatus, uint16_t pui_KeyValue);
void BD76319_ui_process(uint16_t pui_framenumber);
// void BD76319_ui_led_change(uint8_t *report);
void BD76319_ui_kbd_led(uint8_t value);


//  *************************************************/
//  Definitions Section
//  *************************************************/
//  No Definitions defined


//  *************************************************/
// Variables Section (externals)
//  *************************************************/
// No External Global Variables
