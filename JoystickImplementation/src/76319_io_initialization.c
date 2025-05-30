/*
 * GccApplication
 * File --> 76319_io_initialization.c
 *
 */

//  ********************************************************************
/*
 * Author: Rex.Walters
 * Purpose - Project 101202 EVi Classic
 * Firmware - For P/N 76319 EVI Classic Front Panel Interface PCBA
 *
  *----------------------------------------
 * EVI Classic Front Panel Interface Firmware Revisions
 * Revision 0
 * Created / Started March 5,2024
 * Finished (Place Date here)
 *----------------------------------------
*/


/****************************************************/
// Section - Include File Declarations
/****************************************************/
// #include "avr_compiler.h"
#include <asf.h>
// #include <avr/io.h>
#include "76319_io_initialization.h"


/****************************************************/
/***************** Function Prototypes **************/
/****************************************************/
// Declaration Section - Function prototype declarations, for use only by this C file Only
void initialize_PortA_io(void);
void initialize_PortB_io(void);
void initialize_PortC_io(void);
void initialize_PortD_io(void);
void initialize_Port_E_io(void);
void initialize_Port_F_io(void);
void initialize_Port_R_io(void);
// void 76319_initialize_io(void);


//===================================================================
// Port IO, Input / Output Setup Example
	// PORTA.DIRCLR = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);    // Example - Port pins as Input
	// PORTA.PIN2CTRL = PORT_OPC_PULLDOWN_gc;															 // Example - Port input pin declared bit 0 as pulled Down
	// PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;										 // Example - Port input pin declared bit 0 as pulled up
	// PORTA.PIN1CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;										 // Example - Port input pin declared bit 0 as pulled up and Interrupting edges
	// PORTA.INT0MASK = PIN0_bm;																		 // Example - Port input pin declares bit 0 as the Interrupt0 Mask
	// PORTA.INT1MASK = PIN1_bm;																		 // Example - Port input pin declares bit 1 as the Interrupt1 Mask
	// PORTA.INTCTRL  = (PORT_INT0LVL_MED_gc | PORT_INT1LVL_MED_gc);									 // Interrupt Control

	// PORTA.DIRSET = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);    // Example - Port pins as Output
	// PORTA.OUTSET = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);    // Example - Port pin output levels defined


//********************************************************************
//  Section - Code - C Functions
//********************************************************************

//===================================================================
void initialize_PortA_io(void)
{
    // Initializes  PORT A IO bits 7-0 to Outputs.
	//              Bits are LEDs drivers to control the front panel LEDs labeled as 8 to 1.
	//              LEDs Outputs are "On" when Low, LEDs are "Off" when High.

	// PORTA.DIRSET = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);    // Port as Output
	// PORTA.OUTSET = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);    // Turn off outputs by setting to logic 1 (LEDs )

	// Test Code
	// PORTA.OUTCLR = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);    // Turn on outputs by setting to logic 0 (LEDs )
	// PORTA.OUTSET = (PIN0_bm | PIN1_bm | PIN2_bm |PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);    // Turn off outputs by setting to logic 1 (LEDs )
}


//===================================================================
void initialize_PortB_io(void)
{
	// Initialize Horizontal Slider switches port inputs (H_Slider 9 thru 12). (See Port E for remainder of H-Slider switch Positions).
	//              Port B IO bits 3-0 to inputs with pull-ups enabled.
	// Note         Port B IO bits 5-4 are spare IO pins - Initialize to inputs with pull-ups enabled.
	// Initialize   Port B IO bit 1 drive for the Status LED. LEDs Output is "On" when Low, LEDs are "Off" when High.
	// Initialize   Port B IO bit 0 F2-F4 column output for the Keypad key-code scan signal (Front Panel Buttons).
	//              Refer to initialize_Port_F_io(void) below for other keypad Key-code signals.


	// (Input Port Pins)
	PORTB.DIRCLR = (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm);  // Declare pins as Inputs
	PORTB.PIN0CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN1CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN2CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN3CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN4CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN5CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups

	// (Output Port Pins)											             // Declare pins as Outputs
	PORTB.DIRSET = (PIN6_bm | PIN7_bm);
	PORTB.OUTSET = (PIN6_bm);													 // Set Status LED Output IO pin for LED to be "Off".
	PORTB.OUTSET = (PIN7_bm);													 // Set Output pin "F2_F4_COL" for button column to Logic High (Button Disabled).

/*
// Test Code
	PORTB.DIR = 0x0F0;												             // Set Direction Bits 4, 5, 6, 7 as output all others as input

	// Lower bits pulled up
	PORTB.PIN0CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN1CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN2CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN3CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
    // Input pins should be logic high

	// Lower bits pulled down
	PORTB.PIN0CTRL = PORT_OPC_PULLDOWN_gc;										 // Declare pins with pull downs
	PORTB.PIN1CTRL = PORT_OPC_PULLDOWN_gc;										 // Declare pins with pull downs
	PORTB.PIN2CTRL = PORT_OPC_PULLDOWN_gc;										 // Declare pins with pull downs
	PORTB.PIN3CTRL = PORT_OPC_PULLDOWN_gc;										 // Declare pins with pull downs
    // Input pins should be logic low

	// Lower bits pulled up
	PORTB.PIN0CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN1CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN2CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
	PORTB.PIN3CTRL = PORT_OPC_PULLUP_gc;										 // Declare pins with pull ups
    // Input pins should be logic high


	PORTB.OUT = 0x00;													  // Set Output pins "F2_F4_COL" & "STATUS_LED_CATHODE" to Logic Low.
	PORTB.OUT = 0x040;													  // Set Output pin "F2_F4_COL" to Logic Low & "STATUS_LED_CATHODE" to Logic High.
	PORTB.OUT = 0x080;													  // Set Output pin "F2_F4_COL" to Logic High & "STATUS_LED_CATHODE" to Logic Low.
	PORTB.OUT = 0x0C0;													  // Set Output pin "F2_F4_COL" to Logic High & "STATUS_LED_CATHODE" to Logic High.
	PORTB.OUT = 0x00;                                                     // Set Output pins "F2_F4_COL" & "STATUS_LED_CATHODE" to Logic Low.
*/
}


//===================================================================
void initialize_PortC_io(void)
{
	// Initializes   PORTC IO bits 1-0 as Outputs with pull-ups enabled. Reserved IO pins for other IO later such as I2C link.
	// Initializes   Vertical Slider Switches port inputs (V_Slider 1 thru 6). (See Port D for remainder of H-Slider switch Positions).
	// Initializes   PORTC IO bits 7-2 to Inputs with pull-ups enabled

	// (Input Port Pins)
	PORTC.DIRCLR = (PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);	  // Declare pins as Inputs
	PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTC.PIN4CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTC.PIN5CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTC.PIN7CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups

	// (Output Port Pins)
	PORTC.DIRSET = (PIN0_bm | PIN1_bm);											  // Declare pins as Outputs
	// PORTC.OUTCLR = (PIN0_bm | PIN1_bm);											  // Declare pins Output levels
	PORTC.OUTSET = (PIN0_bm | PIN1_bm);											  // Declare pins Output levels
}

//===================================================================
void initialize_PortD_io(void)
{
	// Initializes 6 bits of PortD IO as inputs
	// Initializes   Horizontal Slider Switches port inputs (H_Slider 1 thru 8). (See Port B for remainder of H-Slider switch Positions).
	// Initializes   PORTD IO bits 5-0 to Inputs with pull-ups enabled
	// Initializes   PORTD IO bits 7-6 to Outputs. Bits reserved for USB communication

	// (Input Port Pins)
	PORTD.DIRCLR = (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm);	  // Declare pins as Inputs
	PORTD.PIN0CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTD.PIN1CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTD.PIN3CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTD.PIN4CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTD.PIN5CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	// PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	// PORTD.PIN7CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups

	// (Output Port Pins)
	PORTD.DIRSET = (PIN6_bm | PIN7_bm);											  // Declare pins as Outputs - Declare for USB
	PORTD.OUTCLR = (PIN6_bm | PIN7_bm);											  // Declare pins Output levels
}



//===================================================================
void initialize_Port_E_io(void)
{
	// Initializes all 8 bits of PortE IO as inputs
	// Initializes   Horizontal Slider Switches port inputs (H_Slider 1 thru 8). (See Port B for remainder of H-Slider switch Positions).
	// Initializes   PORTE IO bits 7-0 to Inputs with pull-ups enabled

	// (Input Port Pins)
	PORTE.DIRCLR = (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);
	PORTE.PIN0CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTE.PIN1CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTE.PIN2CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTE.PIN3CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTE.PIN4CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTE.PIN5CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTE.PIN6CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTE.PIN7CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
}


//===================================================================
void initialize_Port_F_io(void)
{
	// Initializes PortF IO - 4 bits are inputs and 4 bits are output for keypad key-code scanning.
	// One additional keypad key-code scan signal (F2-F4 Column) is also initialized on Port B bit 7. See Inititalize_Port_B_io() above.
	// Bits 3-0 (Keypad Column Signals) are set for output, Bits 7-4 (Keypad Row Pins) are set for input

	// (Input Port Pins)
	PORTF.DIRCLR = (PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);						  // Keypad Row Pins
	PORTF.PIN4CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTF.PIN5CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTF.PIN6CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups
	PORTF.PIN7CTRL = PORT_OPC_PULLUP_gc;										  // Declare pins with pull ups


	// (Output Port Pins)
	PORTF.DIRSET = (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm);						  // Keypad Column Pins
	PORTF.OUTSET = (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm);						  // Set pins to Logic High (Buttons Disabled)
	// PORTF.OUTCLR = (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm);						  // Set pins to Logic Low (Buttons Disabled)
}


//===================================================================
/* void initialize_Port_R_io(void)
{
	// Ports R PR0 & PR1 bits are unused (spare) IO Ports - Reserved for a Crystal connection
	// NOTES
	// PORT R Pins
	// 56, Pin Name - PDI     (TCF0 -> PDI_DATA)
	// 57, Pin Name - RESET\  (TCF0 -> PDI_CLOCK)
	// 58, Pin Name - PR0     (INTERRUPT -> SYNC), (USARTF0 -> XTAL2)
	// 59, Pin Name - PR1     (INTERRUPT -> SYNC), (USARTF0 -> XTAL1)


	// PORTR.DIRSET = (PIN0_bm | PIN1_bm);
	// PORTR.OUTCLR = (PIN0_bm | PIN1_bm);
}
*/



//===================================================================
void _76319_initialize_io(void)
// void initialize_io(void)
{
	// initialize_PortA_io();		// (Alarm LED Signals)
	initialize_PortB_io();		// (Horizontal Slider Switch signals), (Status LED Signal), (F2-F4_COL Keypad Scan Code Signal), (Spare IO)
	initialize_PortC_io();		// (Vertical Slider Switch signals), (I2C signals)
	initialize_PortD_io();		// (Vertical Slider Switch signals), (USB signals)
	initialize_Port_E_io();		// (Horizontal Slider Switch signals)
	initialize_Port_F_io();		// (COLUMN & ROW Keypad Scan Code signals)
	// initialize_Ports_R_io();	// (Unused Port)
}
