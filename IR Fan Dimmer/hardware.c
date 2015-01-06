/* 
 * File:   main.c
 * Author:  Gaurav Singh
 *	Email: mailchaduarygaurav@gmail.com
 * website: www.circuitvalley.com 
 * Created on April 16, 2013, 1:33 AM

 *	This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.

 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.

 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

#include <htc.h>
#include "hardware.h"

void initHardware()
{
ANSEL = 0x00; // all pin are Digital
ANSELH = 0x00;
CM1CON0bits.C1ON =0; // disable the comparator 1
CM2CON0bits.C2ON =0; // disable the comparator 2
TRIS_SEG_A =0;
TRIS_SEG_B =0;	
TRIS_SEG_C =0;		
TRIS_SEG_D =0;	
TRIS_SEG_E =0;		
TRIS_SEG_F =0;		
TRIS_SEG_G =0;	
	
  TRIS_DISP_1 =0;	 		
  TRIS_DISP_2 =0;		

  TRIS_DIMMER_1	=0;	
  TRIS_DIMMER_2	=0;

  TRIS_ZERO_CROSS =1;		

  TRIS_KEY_COL1 =1;		
  TRIS_KEY_COL2 =1;		
  TRIS_KEY_COL3 =1;		
  TRIS_KEY_COL4 =1;		

  TRIS_KEY_ROW1	=0;	
  TRIS_KEY_ROW2	=0;	
  TRIS_KEY_ROW3	=0;	
  TRIS_KEY_ROW4	=0;	

  TRIS_IRSENSOR =1;			


  TRIS_RELAY1	=0; 	
  TRIS_RELAY2	=0; 		
  TRIS_RELAY3 	=0;	
  TRIS_RELAY4 	=0;		
  TRIS_RELAY5 	=0;		
  TRIS_RELAY6 	=0;		
  TRIS_RELAY7 	=0;		
  TRIS_RELAY8 	=0;		
  TRIS_RELAY9 	=0;	
  TRIS_RELAY10	=0; 		
  TRIS_RELAY11	=0; 	
  TRIS_RELAY12	=0; 		

  RELAY1 =0;	
  RELAY2 =0;		
  RELAY3 =0;  		
  RELAY4 =0; 	
  RELAY5 =0; 	
  RELAY6 =0; 		
  RELAY7 =0; 		
  RELAY8 =0; 	
  RELAY9 =0; 		
  RELAY10 =0; 		
  RELAY11 =0; 		
  RELAY12 =0; 		

  SEG_A	=1; 		
  SEG_B	=1; 		
  SEG_C	=1; 		
  SEG_D	=1; 		
  SEG_E	=1; 		
  SEG_F	=1; 		
  SEG_G	=1; 		

  DISP_1 =1; 		
  DISP_2 =1; 			

  DIMMER_1 =0;		
  DIMMER_2 =0;		
			

 KEY_ROW1 =0; 		
 KEY_ROW2 =0;	
 KEY_ROW3 =0;	
 KEY_ROW4 =0;

  KEY_COL1 =1;		
  KEY_COL2 =1;		
  KEY_COL3 =1;		
  KEY_COL4 =1;		

    TMR0 = 0;				// clar the timer	
	OPTION_REGbits.nRBPU = 0; // pullup enabled by indivisual PORT latch value
	OPTION_REGbits.INTEDG = 1; 
	OPTION_REGbits.T0CS = 0; // Internal Instruction cycle clock (Fosc/4)
	OPTION_REGbits.PSA = 0; // Prescaller is assigned to timer 0
	OPTION_REGbits.PS= 1; //prescaller set to /4 so for fosc 20Mhz we got 1.25Mhz into timer

	IOCB = 0x3E;   			//interrupt on change B1(col1) B2(col2) B3(IR) B4(col3) B5(col4)
	INTCONbits.T0IE = 1;	// Timer0 overflow interrupt enable
	INTCONbits.T0IF = 0;    // clar the timer0 intrrupt flags
	INTCONbits.RBIE = 1;	// external interrupt GPIO on  change is enabled
	INTCONbits.RBIF = 0;	// clear the external GPIO on  change interrrupt flag
	INTCONbits.INTE	= 1;
	INTCONbits.INTF	= 0;
	T2CONbits.TOUTPS =4;   //postsacler is /10 so 204.8us * 10 to 2.048 ms

	T2CONbits.T2CKPS = 1;
	PIE1bits.TMR2IE = 1;
	PIR1bits.TMR2IF =0;
	T2CONbits.TMR2ON= 1;
	
	INTCONbits.PEIE = 1;    // peripheral intrrupt enable
	INTCONbits.GIE = 1;     // GLOBL interrupt enable

//	EEADR = 0x00;			// load the state of port from EEPROM 
//	EECON1bits.RD = 1;		// Start reding EEPORM
//	GPIO = EEDATA; 			// LOAD The readed data form EEPORM to GPIO
	


}