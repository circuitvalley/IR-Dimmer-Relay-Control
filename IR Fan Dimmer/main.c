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

// Global includes
#include <htc.h>
#include "hardware.h"
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_ON & CPD_ON & BOREN_ON & IESO_OFF & FCMEN_ON & LVP_OFF);
__CONFIG(BOR4V_BOR40V & WRT_HALF);
/*
 * 
 */
#define _XTAL_FREQ 20000000L

//#define TICKSPERMS  1004      // tick in a milli second 
#define TICKS11ms 	13750      	// ticks in 11ms
#define TICKS5o5ms 	6875 		// ticks in 5.5ms
#define TICKS2o3ms 	2875 		// ticks in 2.3ms
#define TICKS3ms  	3750		// ticks in 3sm
#define TICKS0o2ms	250			// ticks in 0.2ms
#define TICKS8ms 	10000		// Tick

#define LED_15sec 14648
unsigned int TIMEOUT  =   TICKS11ms;       	// the pulse should occur before this time excede Otherwise it is an error 
unsigned int PREPULSE = TICKS8ms;			// the interrupt should occur after this time Otherwise it is an error

unsigned short long timer;			// varible to keep track of long timeouts  ( it can also be int if you want to save flash memory for some other purpose  ) 

unsigned char dataready;				// varible to use as flag when data is completly received and  ready it is 1 else 0 

 
unsigned char address=0,notaddress=0;	// these varible are used to store received address
unsigned char command=0,notcommand=0;	// these varible are used to store received address
void timer2Isr();
void interruptOnChangeIsr(void);  				// interrupt service routine for interrupt on change of input port for IR sensor of mcu 
void timerInterruptIsr(void);					// interrupt service rouine for timer0 
unsigned char dimmerspeed[2]={0,0};
unsigned char dimmerdelay[2]={0,0};
unsigned int display_off_timer=LED_15sec;
unsigned char scandisp_timer=0;
void scanKey(void);
#define DIMMER_MAX 5
#define RELAY_LSG_addr 0x0
#define RELAY_MSG_addr 0x1
#define DIMMER_addr 0x02
void interrupt intisr(void)
{

	if(INTCONbits.T0IF)							// check the timer0 over flow interrupt flag 
	{
	timerInterruptIsr();						// timer0 overflow interrupt has been occur call the isr
	INTCONbits.T0IF =0;							// clear the timer0 interrupt flag
	}else if(INTCONbits.INTF)
	{
		TMR2=0x0;	
		dimmerdelay[0]=(DIMMER_MAX-dimmerspeed[0])<<1;	
		dimmerdelay[1]=(DIMMER_MAX-dimmerspeed[1])<<1;
		timer2Isr();
		INTCONbits.INTF	=0;
	}else if(	PIR1bits.TMR2IF)
	{
	timer2Isr();
	PIR1bits.TMR2IF =0;
	}
	else if (INTCONbits.RBIF)					// check the interrupt on change flag
	{	
	interruptOnChangeIsr();					// interrupt on change has been detected call the isr	
	if(!(KEY_COL4 & KEY_COL3 & KEY_COL2 &KEY_COL1))
	{	
	scanKey();
	}
		INTCONbits.RBIF =0;						// clear the interrupt on chage flag 
	}

}

/* THE main source code Start here*/

void scanDisp();


void putSegment(unsigned char display,unsigned char data);

void main() 
{
unsigned char change_dimmer=0;	

  
initHardware();

	EEADR = RELAY_LSG_addr;			// load the state of port from EEPROM 
	EECON1bits.EEPGD=0;
	EECON1bits.RD = 1;		// Start reding EEPORM

	while(EECON1bits.RD);
	RELAY1= (EEDATA & 0x01) && 0x1;
	RELAY2= (EEDATA & 0x02) && 0x1;
	RELAY3= (EEDATA & 0x04) && 0x1;
	RELAY4=(EEDATA & 0x08) && 0x1;
	RELAY5= (EEDATA & 0x10) && 0x1;
	RELAY6= (EEDATA & 0x20) && 0x1;
	RELAY7=(EEDATA & 0x40) && 0x1;
	RELAY8= (EEDATA & 0x80) && 0x1;

	EEADR = RELAY_MSG_addr;			// load the state of port from EEPROM 
	EECON1bits.EEPGD=0;
	EECON1bits.RD = 1;		// Start reding EEPORM

	while(EECON1bits.RD);
	RELAY9= (EEDATA & 0x01) && 0x1;
	RELAY10= (EEDATA & 0x02) && 0x1;
	RELAY11= (EEDATA & 0x04) && 0x1;
	RELAY12= (EEDATA & 0x08) && 0x1;

	EEADR = DIMMER_addr ;			// load the state of port from EEPROM 
	EECON1bits.EEPGD=0;
	EECON1bits.RD = 1;		// Start reding EEPORM
		
	while(EECON1bits.RD);
dimmerspeed[0]= (EEDATA) & 0x0F;
dimmerspeed[1]=(EEDATA>>4) & 0x0F;
if(dimmerspeed[0]>5)
dimmerspeed[0]=0;
if(dimmerspeed[1]>5)
dimmerspeed[1]=0;



	while(1)      			// wait forever for  the data received and ready  
	{
			
	if(dataready)			// data is received and ready to be procssed 
	{

	if(!display_off_timer)
	{
		display_off_timer=LED_15sec;
	}
	else
	{
		// key1  0x28	to	Toggle relay 1	// these are command of the IR remote control which i have 
		// key2  0xA8	to	Toggle relay 2
		// key3  0x68	to	Toggle relay 3
		// key4  0x70	to	Toggle relay 4
		// key5  0xB0	to	Turn off all the relays
		// key6  0x30	to	Turn off all the relays
		// key7  0x50	to	Turn off all the relays
		// key8  0x90	to	Turn off all the relays
		// key9  0x10	to	Turn off all the relays
		// key10  0x60	to	Turn off all the relays
		// key11  0xA0	to	Turn off all the relays
		// key12  0x20	to	Turn off all the relays
		// key13  0x80	to	Turn off all the relays
		// key14  0x00	to	Turn off all the relays
	
		switch(command)	// swich on 
		{
		case 0x28: change_dimmer++;		//change dimmer 	
				   break;
		case 0xA8: RELAY1 = !RELAY1;		//Toggle relay 1
				   break;
		case 0x68: RELAY2 = !RELAY2;		//Toggle relay 2
				   break;
		case 0x70: RELAY3 = !RELAY3;		//Toggle relay 3
				   break;
		case 0xB0: RELAY4 = !RELAY4;		//Toggle relay 4
				   break;
		case 0x30: RELAY5 = !RELAY5;		//Toggle relay 5
				   break;
		case 0x50: RELAY6 = !RELAY6;		//Toggle relay 6
				   break;
		case 0x90: RELAY7 = !RELAY7;		//Toggle relay 7
				   break;
		case 0x10: RELAY8 = !RELAY8;		//Toggle relay 8
				   break;
		case 0x60: RELAY9 = !RELAY9;		//Toggle relay 9
				   break;
		case 0xA0: RELAY10 = !RELAY10;		//Toggle relay 10
				   break;
		case 0x20: RELAY11 = !RELAY11;		//Toggle relay 11
				   break;
		case 0x12: RELAY12 = !RELAY12;		//Toggle relay 12
				   break;
		case 0x13:{ 								//keyboard key dimmer 2
					if(dimmerspeed[1]<DIMMER_MAX){
					dimmerspeed[1]++;
				}else{
					dimmerspeed[1]=0;
				}					 
				   }									
				   break;
		case 0x14: 	{								//keyboard key dimmer 1
				if(dimmerspeed[0]<DIMMER_MAX){
					dimmerspeed[0]++;
				}else{
					dimmerspeed[0]=0;
				}
				}
				   break;
	
		case 0x80:											//remote key dimmer down
					if(change_dimmer&0x01){
					if(dimmerspeed[1]){
					dimmerspeed[1]--;
					}
					}else{
					if(dimmerspeed[0]){
					dimmerspeed[0]--;
					}
					}
					break;
		case 0x00:												//remote key dimmer up
						if(change_dimmer&0x01){
					if(dimmerspeed[1]<DIMMER_MAX){
					dimmerspeed[1]++;
					}
					}else{
					if(dimmerspeed[0]<DIMMER_MAX){
					dimmerspeed[0]++;
					}
					}
					break;
		default : 	
					break;			
		}
		display_off_timer=LED_15sec;
		EEADR = RELAY_LSG_addr; 			//Write PORT status to EEPROM
		EEDAT = RELAY1 | RELAY2<<1 | RELAY3<<2 | RELAY4<<3 | RELAY5<<4 | RELAY6<<5 | RELAY7<<6 | RELAY8<<7  ;			// load the current status of GPIO to EEPROM write register
	
		EECON1bits.EEPGD =0;
		EECON1bits.WREN = 1;  	// Enable EEPROM write
		INTCONbits.GIE = 0;		//1	disable the interrupts as it may currupt the EEPROM data
		EECON2 = 0x55;			//2
		EECON2 = 0xAA;			//3	(1,2,3) require sequence 
		EECON1bits.WR = 1;  	// satart writing
		INTCONbits.GIE = 1;  	// Enable the interrupts 
		
		while(EECON1bits.WR);
		EEADR = RELAY_MSG_addr; 			//Write PORT status to EEPROM
		EEDAT =   RELAY9 | RELAY10<<1 | RELAY11<<2 | RELAY12<<3;			// load the current status of GPIO to EEPROM write register
	
		EECON1bits.EEPGD =0;
		EECON1bits.WREN = 1;  	// Enable EEPROM write
		INTCONbits.GIE = 0;		//1	disable the interrupts as it may currupt the EEPROM data
		EECON2 = 0x55;			//2
		EECON2 = 0xAA;			//3	(1,2,3) require sequence 
		EECON1bits.WR = 1;  	// satart writing
		INTCONbits.GIE = 1;  	// Enable the interrupts 

		while(EECON1bits.WR);
		EEADR = DIMMER_addr ; 			//Write PORT status to EEPROM
		EEDAT = dimmerspeed[0] | dimmerspeed[1]<<4 ;			// load the current status of GPIO to EEPROM write register
	
		EECON1bits.EEPGD =0;
		EECON1bits.WREN = 1;  	// Enable EEPROM write
		INTCONbits.GIE = 0;		//1	disable the interrupts as it may currupt the EEPROM data
		EECON2 = 0x55;			//2
		EECON2 = 0xAA;			//3	(1,2,3) require sequence 
		EECON1bits.WR = 1;  	// satart writing
		INTCONbits.GIE = 1;  	// Enable the interrupts 
	
	}
		dataready=0;			// data has been processed so clear the dataready flag 
		
	}
	
	}
	
}

void interruptOnChangeIsr(void)
{

unsigned short long tdiff;       
unsigned char pin;
static unsigned long rxbuffer;
static  unsigned char necpoj; 	 				/* (necpoj=NEC position )this varible is used to keep track of the edges of the input singal 
											as decoding of the singal is done by a state machine 
											so this varible acutalley sotores what state we currently are 
							 				and total bits 32 and 2 leading pulse */
tdiff = ((timer<<8)+TMR0) ;     // calculate how much time has been passed since last interrupt 
								// the time shold be less then time out and greater than PREPULSE 
pin = IRSENSOR;					// store the current status of Sensor 
TMR0 = 0;						// reset the timer0 to measure the next edge(interrupt) of input
timer = 0;						// reset the timer varible to




/* state machine is started here and it totally managed and keeps track of its states using the varible necpoj 
here are the details of necpoj ( NEC position ) varible 
if 
necpoj == 1       we just detected the first edge of the input singal it may also mean(if interrupt is not false) that the 9ms leading pulse started 
				  after the first edge THE next pulse is expected to arrive around 9ms so the TIMEOUT is set to 11ms and PREPULSE is set to 8ms
				

necpoj == 2 	  we just detected the second edge of the input signal and we finished the 9ms leding pulse and now 4.5ms space started 
				  after the second edge the next pulse is expected to arrive around 4.5ms so TIMEOUT is set to 5.5ms and PREPULSE is 3ms
		

necpoj == 3  	  we just detected the third edge of the input singal and we finished 4.5ms space and addres lsb is now started 
				  after the third edge the next pulse is expected to arrive around 562.5us so TIMEOUT is set to 2.3ms and PREPULSE is 0.2ms (timeout can be much less at this state but to do this i have to add one more if else statemetnt)
		
necpoj == 4 	  we just decected the forth edge and the 562.5 us burt of LSB of address has ended now a little space for '0'562.5us  or for '1' 1.6875ms   
				  after the forth edge the next pulse is expected to arrive for '0' around 562.5us  and for '1' 1.675ms so TIMEOUT is set to 2.3ms and PREPULSE is 0.2ms
		
necpoj ==5 to 66  data pulse keep comming  
					TIMOUT and PREPLUSE remain same as above.		   
			
necpoj ==67		  we just fined the command inverse MSB space not the final 562.5us burst has stated  so we fined the receiveing 
				  now we will check the address and command for being correct
*/


if ((tdiff>PREPULSE) && (tdiff<TIMEOUT) )		// the edge (interrupt) occurrence time should be less then the TIMOUT and greater then PREPULESE else it is an fake singal
{												// At the very first edge (necpoj==0)  this conditon will always false and the false block of this if will bring the state machine (necpoj) to position 1(position 1 means 9ms leading pulse has started now we have to wait for 4.5ms start pulse to occur) 
						


				
	if(necpoj==1 || necpoj==2)					// when we are hear it means 9ms leding pulse has ended and now we are necpoj=1 or necpoj=2
	{

			if((pin==1) && (necpoj==1))
			{
			necpoj++;
			TIMEOUT 	= TICKS5o5ms;  			// timeout for 3rd pulse 5.5ms	
			PREPULSE   	= TICKS3ms;				// PREPULSE for 3rd pulse 3ms
			}
			else if((pin==0)&& (necpoj ==2))
			{
			necpoj++;
		
			TIMEOUT 	= TICKS2o3ms;  			// now data starts so timeout is 2.3ms
			PREPULSE   	= TICKS0o2ms;  
	
			}
			else								// this block handle the conditon if any error occur after the completing the pre pulses 
			{
			necpoj = 0;							//reset the state machine 
			TIMEOUT 	=  	TICKS11ms;
			PREPULSE 	= 	TICKS8ms;
			}
	}
	else if(necpoj>2)							//now we are picking the data 	
	{	
				
			necpoj++;					 		//necpoj sill inrement on every edge 	

				if(necpoj&0x01)					// here we check the if necpoj is an odd number because when necpoj goes greater then 3 then 
									    		//necpoj will always be and odd value when a single bit tranmission is over  
				{
					rxbuffer=rxbuffer<<1;		//shift the buffer 
					if(tdiff>1250)				//we are here means we just recevied the edge of finished tranmission of a bit 
												// so if last edge was more than 1.24 ms then the bit which is just over is one else it is zero 
					{
					rxbuffer = rxbuffer | 0x1;
				//	GPIObits.GPIO5 = !GPIObits.GPIO5;    
					}
					else
					{
					rxbuffer = rxbuffer |0x0;
			    //	GPIObits.GPIO4 = !GPIObits.GPIO4;
					}
			
				}
		
			if(necpoj >66)						// we have reached (Leading pulse 2 +address 16+~address16+ command 16+ ~command 16+ last final burst first edge 1)=67th edge of the message frame means the date tranmission is now over 
			{
				
			address 	= (rxbuffer>>24)& 0xFF;			//extract the data from the buffer 
			notaddress 	= (rxbuffer>>16)& 0xFF;
			command 	= (rxbuffer>>8)	& 0xFF;
			notcommand 	= (rxbuffer)	& 0xFF;
			rxbuffer=0;									//clear the buffer	
				
					if((!(address & notaddress)) && (!(command & notcommand)))		// check weather the received data is vaild or not
					{
					dataready =1;
					}
					else
					{
					dataready=0;
					}
			TIMEOUT 	=  	TICKS11ms;					// weather we received the vaild data or not we have to reset the state machine 
			PREPULSE 	= 	TICKS8ms;
			necpoj=0;
			}
	}
	else 
	{	
	
	TIMEOUT 	=	TICKS11ms;							// some error occured reset state machine 
	PREPULSE 	=	TICKS8ms;
	}
	
}
else
{
	
	if(pin==0)				//we are here means that after a longtimeout or PREPULSE we just detect a pulse which may be the start of 9ms pulse 
	{
	necpoj = 1;				// yes it could be the start of 9ms pulse 
	}
	else 
	{	
	necpoj = 0;				// no it's not start of 9ms pulse 
	}
	
	address 	= 0xFF;
	notaddress 	= 0xFF;
	command 	= 0xFF;
	notcommand 	= 0xFF;
	dataready 	= 0x000;
	TIMEOUT =  TICKS11ms;		//default timing  
	PREPULSE = TICKS8ms;
}
	
}

void timerInterruptIsr(void)
{
	if(timer<0xFFFF)			// this code is to increment the variable timer's value on every over flow but this if conditon will prevent this variable form rollover when a long timeout occurs
	{
		timer++;
	}

	scandisp_timer++;
	if(scandisp_timer==49)
	{
		if(display_off_timer)
		{
			scanDisp();
		}
		else
		{
  			DISP_1 =1; 		
  			DISP_2 =1; 
		}

	scandisp_timer =0;
	}				
}

void timer2Isr() 	// every  2.048 ms
{
	if(dimmerdelay[0])
	{
		DIMMER_1=0;	
		if(!(dimmerdelay[0]&0x80))
			dimmerdelay[0]--;
	}else
	{
		DIMMER_1=1;
		dimmerdelay[0]=0xFF;	
	}

	if(dimmerdelay[1])
	{	
		DIMMER_2=0;
		if(!(dimmerdelay[1]&0x80))
			dimmerdelay[1]--;

	}else
	{
		DIMMER_2=1;
		dimmerdelay[1]=0xFF	;
	}


	if(display_off_timer)
	{
		display_off_timer--;
	}
}



void scanDisp()
{
static unsigned char disp;
putSegment(disp&0x01,dimmerspeed[disp&0x01]);
disp++;
}		



void putSegment(unsigned char display,unsigned char data)
{

  DISP_1 =1; 		
  DISP_2 =1; 

switch(data & 0x0F)
{
case 0:
	{
  SEG_A	=0; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=0; 		
  SEG_E	=0; 		
  SEG_F	=0; 		
  SEG_G	=1; 
	}
	  break;
case 1:
	{
  SEG_A	=1; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=1; 		
  SEG_E	=1; 		
  SEG_F	=1; 		
  SEG_G	=1; 
	}
	  break;
case 2:
	{
  SEG_A	=0; 		
  SEG_B	=0; 		
  SEG_C	=1; 		
  SEG_D	=0; 		
  SEG_E	=0; 		
  SEG_F	=1; 		
  SEG_G	=0; 
	}
	  break;
case 3:
	{
  SEG_A	=0; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=0; 		
  SEG_E	=1; 		
  SEG_F	=1; 		
  SEG_G	=0; 
	}
	  break;
case 4:
	{
  SEG_A	=1; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=1; 		
  SEG_E	=1; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
case 5:
	{
  SEG_A	=0; 		
  SEG_B	=1; 		
  SEG_C	=0; 		
  SEG_D	=0; 		
  SEG_E	=1; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
case 6:
	{
  SEG_A	=0; 		
  SEG_B	=1; 		
  SEG_C	=0; 		
  SEG_D	=0; 		
  SEG_E	=0; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
case 7:
	{ 
  SEG_A	=0; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=1; 		
  SEG_E	=1; 		
  SEG_F	=1; 		
  SEG_G	=1; 
	}
	  break;
case 8:
	{
  SEG_A	=0; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=0; 		
  SEG_E	=0; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
case 9:
	{  
  SEG_A	=0; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=0; 		
  SEG_E	=1; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
case 0xA:
	{
  SEG_A	=0; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=1; 		
  SEG_E	=0; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
case 0xB:
	{
  SEG_A	=1; 		
  SEG_B	=1; 		
  SEG_C	=0; 		
  SEG_D	=0; 		
  SEG_E	=0; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
case 0xC:
	{
  SEG_A	=0; 		
  SEG_B	=1; 		
  SEG_C	=1; 		
  SEG_D	=0; 		
  SEG_E	=0; 		
  SEG_F	=0; 		
  SEG_G	=1; 
	}
	  break;
case 0xD:
	{
  SEG_A	=1; 		
  SEG_B	=0; 		
  SEG_C	=0; 		
  SEG_D	=0; 		
  SEG_E	=0; 		
  SEG_F	=1; 		
  SEG_G	=0; 
	}
	  break;
case 0xE:
	{
  SEG_A	=0; 		
  SEG_B	=1; 		
  SEG_C	=1; 		
  SEG_D	=0; 		
  SEG_E	=0; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
case 0xF:
	{
  SEG_A	=0; 		
  SEG_B	=1; 		
  SEG_C	=1; 		
  SEG_D	=1; 		
  SEG_E	=0; 		
  SEG_F	=0; 		
  SEG_G	=0; 
	}
	  break;
default:
  SEG_A	=1; 		
  SEG_B	=1; 		
  SEG_C	=1; 		
  SEG_D	=1; 		
  SEG_E	=1; 		
  SEG_F	=1; 		
  SEG_G	=0; 
	
}

if(display)
{
  DISP_1 =0; 		
  DISP_2 =1; 
}
else
{
  DISP_1 =1; 		
  DISP_2 =0; 
}

}