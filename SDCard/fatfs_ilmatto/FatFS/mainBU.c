/*
 * FatFS.c
 *
 * Created: 24/10/2012 13:02:51
 *  Author: hslovett
 */ 

//#define F_CPU 12000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "diskio.h"
#include "ff.h"
#include "__UART.h"

char Line[100];				/* Console input buffer */
FATFS Fatfs[_VOLUMES];		/* File system object for each logical drive */
FIL File[2];				/* File object */

BYTE Buff[2048];			/* Working buffer */

ISR(TIMER0_COMPA_vect)
{
	disk_timerproc();
	
}
int main(void)
{
	
	char *ptr, *ptr2;
	long p1, p2, p3;
	BYTE res, b1, *bp;
	UINT s1, s2, cnt;
	DWORD ofs, sect = 0;
	FATFS *fs;
	DIR dir;
	FILINFO Finfo;
	
	USART0_Init(MYUBBR0);
	DDRB = 0x01;
	//initialise timer 0 to interrupt every 10 ms
	TIMSK0 |= (1 << OCIE0A);
	TCCR0A |= (1 << WGM01);
	OCR0A = 117; //10ms interrupt at 12MHz
	TCCR0B |= (1 << CS02) | (1 << CS00);
	
	sei();
	USART0_SendString("\n\rReady.");
	
	
	while(1)
	{
		USART0_GetLine(Line, sizeof(Line));
		ptr = (char*)Line;
		switch (*ptr++) 
		{
			case 'a':
				USART0_SendString("Test");
				break;
				
			case 'd' :
			switch (*ptr++) 
			{
				case 'i' :	/* di <phy_drv#> - Initialize disk */
	// 				if (!xatoi(&ptr, &p1)) 
	// 					break;
					while (*ptr == ' ') ptr++;
					p1 = atoi(*ptr);
					USART0_SendString("Disk ");
					USART0_SendInt(p1);
					USART0_SendString(" Initialise:");
					USART0_SendInt(disk_initialize((BYTE)p1));
					break;//break di


			}
			break; //break d
					
			case 'f' :
				switch (*ptr++)
				{

					case 'i' :	/* fi <log drv#> - Initialize logical drive */
						//if (!xatoi(&ptr, &p1)) break;
						//put_rc(f_mount((BYTE)p1, &Fatfs[p1]));
						while (*ptr == ' ') ptr++;
						p1 = atoi(*ptr);
						USART0_SendString("Mounting drive ");
						USART0_SendInt(p1);
						USART0_SendHex(f_mount(p1, &Fatfs[p1]));
						break;
						
					
					case 'l' :	/* fl [<path>] - Directory listing */
						while (*ptr == ' ') ptr++;
						res = f_opendir(&dir, ptr);
						if (res) 
						{ 
							USART0_SendString("Open Dir Fail: ");
							USART0_SendHex(res); 
							break; 
						}
						p1 = s1 = s2 = 0;
						for(;;) 
						{
							res = f_readdir(&dir, &Finfo);
							if ((res != FR_OK) || !Finfo.fname[0]) 
								break;
							if (Finfo.fattrib & AM_DIR) 
							{
								s2++;
							} 
							else 
							{
								s1++; 
								p1 += Finfo.fsize;
							}
							sprintf(Line, "%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s", 
								(Finfo.fattrib & AM_DIR) ? 'D' : '-',
								(Finfo.fattrib & AM_RDO) ? 'R' : '-',
								(Finfo.fattrib & AM_HID) ? 'H' : '-',
								(Finfo.fattrib & AM_SYS) ? 'S' : '-',
								(Finfo.fattrib & AM_ARC) ? 'A' : '-',
								(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
								(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
								Finfo.fsize, &(Finfo.fname[0]));
							USART0_SendString(Line);
							USART0_SendString("\n\r");
// #if _USE_LFN
// 					for (p2 = strlen(Finfo.fname); p2 < 14; p2++)
// 						xputc(' ');
// 					xprintf(PSTR("%s\n"), Lfname);
// #else
					USART0_SendChar('\n');
/*#endif*/
				}
				sprintf(Line, "%4u File(s),%10lu bytes total\n%4u Dir(s)", s1, p1, s2);
				USART0_SendString(Line);
				USART0_SendString("\n\r");
				if (f_getfree(ptr, (DWORD*)&p1, &fs) == FR_OK)
				{
					sprintf(Line, ", %10luK bytes free\n", p1 * fs->csize / 2);
					USART0_SendString(Line);
					USART0_SendString("\n\r");
				}					
				break;
				}
				break;


		
		}
		
    }
}
