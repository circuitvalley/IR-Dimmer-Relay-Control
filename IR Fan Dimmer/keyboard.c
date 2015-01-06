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

#define _XTAL_FREQ 20000000L
unsigned char KEYCODE[4][4] = {  {0xA8,0x68,0x70,0xB0},
								{0x30,0x50,0x90,0x10},
								{0x60,0xA0,0x20,0x12},
								{0x13,0x14,0xFF,0xFF}
							   };


extern unsigned char dataready;
extern unsigned char command,notcommand;
void scanKey(void)
{	
	unsigned int temp,COL=0,ROW=0;


	
	__delay_ms(7);
	
	temp = ((KEY_COL4<<3) | (KEY_COL3<<2)| (KEY_COL2<<1) |KEY_COL1 )& 0xF ;
	temp ^= 0x0F;
if(temp){
	while(temp>>=1) COL++;
	KEY_ROW4 = 1;
	KEY_ROW3 = 1;
	KEY_ROW2 = 1;
	KEY_ROW1 = 0;
	if((((KEY_COL4<<3) | (KEY_COL3<<2)| (KEY_COL2<<1) |KEY_COL1 )& 0xF)!= 0xF)
	{	
		ROW=0; 
	} 
	else
	{
	KEY_ROW4 = 1;
	KEY_ROW3 = 1;
	KEY_ROW2 = 0;
	KEY_ROW1 = 1;
	if((((KEY_COL4<<3) | (KEY_COL3<<2)| (KEY_COL2<<1) |KEY_COL1 )& 0xF)!= 0xF)
		{
		 ROW = 1; 
		}
		else
		{	
	KEY_ROW4 = 1;
	KEY_ROW3 = 0;
	KEY_ROW2 = 1;
	KEY_ROW1 = 1;
	if((((KEY_COL4<<3) | (KEY_COL3<<2)| (KEY_COL2<<1) |KEY_COL1 )& 0xF)!= 0xF)
					{
		 			ROW = 2; 
					}
					else
					{
			KEY_ROW4 = 0;
			KEY_ROW3 = 1;
			KEY_ROW2 = 1;
			KEY_ROW1 = 1;
					
							
	if((((KEY_COL4<<3) | (KEY_COL3<<2)| (KEY_COL2<<1) |KEY_COL1 )& 0xF)!= 0xF)
						{
						ROW = 3; 
						}
					
	
					}
		}
	}

			KEY_ROW4 = 0;
			KEY_ROW3 = 0;
			KEY_ROW2 = 0;
			KEY_ROW1 = 0;
		

		command=KEYCODE[ROW][COL];
	dataready=1;
}
}
		






