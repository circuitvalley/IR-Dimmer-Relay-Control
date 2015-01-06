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

#ifndef 	_HARDWARE_H_
#define _HARDWARE_H_
#define RELAY1 			PORTDbits.RD1
#define RELAY2 			PORTDbits.RD0
#define RELAY3 			PORTEbits.RE2
#define RELAY4 			PORTAbits.RA3
#define RELAY5 			PORTAbits.RA5
#define RELAY6 			PORTAbits.RA4
#define RELAY7 			PORTCbits.RC1
#define RELAY8 			PORTCbits.RC0
#define RELAY9 			PORTEbits.RE0
#define RELAY10 			PORTEbits.RE1
#define RELAY11 			PORTBbits.RB7
#define RELAY12 			PORTBbits.RB6

#define TRIS_RELAY1 			TRISDbits.TRISD1
#define TRIS_RELAY2 			TRISDbits.TRISD0
#define TRIS_RELAY3 			TRISEbits.TRISE2
#define TRIS_RELAY4 			TRISAbits.TRISA3
#define TRIS_RELAY5 			TRISAbits.TRISA5
#define TRIS_RELAY6 			TRISAbits.TRISA4
#define TRIS_RELAY7 			TRISCbits.TRISC1
#define TRIS_RELAY8 			TRISCbits.TRISC0
#define TRIS_RELAY9 			TRISEbits.TRISE0
#define TRIS_RELAY10 			TRISEbits.TRISE1
#define TRIS_RELAY11 			TRISBbits.TRISB7
#define TRIS_RELAY12 			TRISBbits.TRISB6

#define SEG_A				PORTDbits.RD5
#define SEG_B				PORTDbits.RD6
#define SEG_C				PORTCbits.RC4
#define SEG_D				PORTCbits.RC5
#define SEG_E				PORTCbits.RC6
#define SEG_F				PORTDbits.RD4
#define SEG_G				PORTCbits.RC7

#define DISP_1				PORTDbits.RD7
#define DISP_2				PORTAbits.RA2

#define DIMMER_1			PORTAbits.RA1
#define DIMMER_2			PORTAbits.RA0

#define ZERO_CROSS			PORTBbits.RB0

#define KEY_COL1			PORTBbits.RB4
#define KEY_COL2			PORTBbits.RB5
#define KEY_COL3			PORTBbits.RB2
#define KEY_COL4			PORTBbits.RB1

#define KEY_ROW1			PORTCbits.RC3
#define KEY_ROW2			PORTCbits.RC2
#define KEY_ROW3			PORTDbits.RD2
#define KEY_ROW4			PORTDbits.RD3


#define TRIS_SEG_A				TRISDbits.TRISD5
#define TRIS_SEG_B				TRISDbits.TRISD6
#define TRIS_SEG_C				TRISCbits.TRISC4
#define TRIS_SEG_D				TRISCbits.TRISC5
#define TRIS_SEG_E				TRISCbits.TRISC6
#define TRIS_SEG_F				TRISDbits.TRISD4
#define TRIS_SEG_G				TRISCbits.TRISC7

#define TRIS_DISP_1				TRISDbits.TRISD7
#define TRIS_DISP_2				TRISAbits.TRISA2

#define TRIS_DIMMER_1			TRISAbits.TRISA1
#define TRIS_DIMMER_2			TRISAbits.TRISA0

#define TRIS_ZERO_CROSS			TRISBbits.TRISB0

#define TRIS_KEY_COL1			TRISBbits.TRISB4
#define TRIS_KEY_COL2			TRISBbits.TRISB5
#define TRIS_KEY_COL3			TRISBbits.TRISB2
#define TRIS_KEY_COL4			TRISBbits.TRISB1

#define TRIS_KEY_ROW1			TRISCbits.TRISC3
#define TRIS_KEY_ROW2			TRISCbits.TRISC2
#define TRIS_KEY_ROW3			TRISDbits.TRISD2
#define TRIS_KEY_ROW4			TRISDbits.TRISD3

#define IRSENSOR 				PORTBbits.RB3
#define TRIS_IRSENSOR 				TRISBbits.TRISB3


void initHardware();
#endif