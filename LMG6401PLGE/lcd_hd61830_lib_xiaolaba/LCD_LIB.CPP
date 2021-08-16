#include <stdio.h>

//// c:\mingw\include\dos.h:54:2: warning: #warning "<dos.h> is obsolete; consider using <direct.h> instead." [-Wcpp]
//// #warning "<dos.h> is obsolete; consider using <direct.h> instead."
//#include <dos.h>
#include <direct.h>	//


#include <stdlib.h>

#include "inpout32.h"	//win32 IO access


#define DATA     0x378       // data lines address for LPT 1
#define CONTROL  0x37A       // control lines address for LPT 1

#define MAXX      240        // horizontal resolution of LCD
#define MAXY      128        // vertical resolution of LCD
//#define MAXBYTE  3840        // number of bytes in the display (MAXX*MAXY/8)
#define DIS_MAXBYTE  3840        // win32, number of bytes in the display (MAXX*MAXY/8)
#define TEXTMAXX   40        // horizontal resolution, text mode
#define TEXTMAXY   16        // vertical resolution, text mode
#define TEXTBYTE  640        // number of bytes in a text screen
#define TOTALBYTE 4192       // total RAM memory
#define DELAY    2500        // delay for instructions :
                             // 100 for Pentium 133
                             // 2500 for AMD 1800+

/*
LPT pin - LCD pin
1   - 6
17  - 4
*/

// LCD control lines definition
#define EN_0  0x01 // xxx1, sets E to 0 at LPT line
#define EN_1  0x00 // xxx0, sets E to 1 at LPT line
#define RS_0  0x08 // x1xx, sets RS to 0 at LPT line
#define RS_1  0x00 // x0xx, sets RS to 1 at at LPT line

// LCD commands definition
#define DISPLAY_LOW     0x08
#define DISPLAY_HIGH    0x09
#define CURSOR_LOW	0x0A
#define CURSOR_HIGH	0x0B
#define WRITEBYTE	0x0C
#define CLEARBIT	0x0E
#define SETBIT		0x0F

static char _copyright[]="Contact : cdragos69@yahoo.com";

static unsigned char font[2048];
static unsigned int current_x,current_y;

/*
          Return    :
          0     -   ok. Font loaded
          1     -   file not found
          2     -   invalid file
*/

unsigned int LCD_loadfont(char *name)
{
    FILE *f;

    f=fopen(name,"rb");
    if(!f)
        return 1;
    if(fread(font,1,2048,f)!=2048)            // read font information from file
        return 2;
    return 0;
}

void microdelay(unsigned int d)
{
    for(;d;d--)
        //{nop;};	//borland C syntax
		__asm__ ("nop");	// minGW g++ syntax
}

#define outportb gfpOut32

void LCD_command(unsigned char cmd, unsigned char data)
{
    outportb(DATA, cmd ); // Output the actual command
    outportb(CONTROL, (RS_1 | EN_0));   // set RS on 1 for instruction, E is kept low
    outportb(CONTROL, (RS_1 | EN_1));   // pulse on E
    outportb(CONTROL, (RS_1 | EN_0));

    outportb(DATA, data); // Output the actual data
    outportb(CONTROL, (RS_0 | EN_0));  // set RS on 0 for data, E is kept low
    outportb(CONTROL, (RS_0 | EN_1));  // pulse on E
    outportb(CONTROL, (RS_0 | EN_0));

    microdelay(DELAY);       // time for controller to finish the command
}

void LCD_init_graph()
{
    LCD_command(0x00, 0x32);	        // Display ON, master mode, graphic mode
    LCD_command(0x01, 0x77);	        // 8 bits / character
    LCD_command(0x02, 0x1D);	        // 30 horizontal bytes - 240 pixels
    LCD_command(0x03, 0x7F);	        // 1/128 duty cycle (128 lines)
    LCD_command(0x04, 0x00);	        // Cursor at bottom line
    LCD_command(DISPLAY_LOW, 0x00);	// Display start address low
    LCD_command(DISPLAY_HIGH, 0x00);	// Display start address high
    LCD_command(CURSOR_LOW, 0x00);	// Cursor address low
    LCD_command(CURSOR_HIGH, 0x00);	// Cursor address high
    current_x=0;                        // initial current position
    current_y=0;
}

void LCD_clear_memory(void)
{
    register unsigned int i;

    LCD_command(CURSOR_LOW,0);        // set page start address
    LCD_command(CURSOR_HIGH,0);
    for(i=0;i<TOTALBYTE;i++) LCD_command(WRITEBYTE,0);   // reset all memory
}

void LCD_clear_graph(void)
{
    register unsigned int i;

    LCD_command(CURSOR_LOW,0);        // set page start address
    LCD_command(CURSOR_HIGH,0);
    for(i=0;i<DIS_MAXBYTE;i++) LCD_command(WRITEBYTE,0);   // reset all graphic memory
}

void LCD_putpixel(unsigned int x,unsigned int y, unsigned char set)
{
    unsigned int byte;
    unsigned char pos;

    if(x<MAXX && y<MAXY)
    {
        byte=(y*30)+(x>>3);                   // byte address for the pixel
        pos=x&0x07;                           // pixel bit position inside byte

        LCD_command(CURSOR_LOW,byte & 0xFF);    // high part of byte address
        LCD_command(CURSOR_HIGH,byte >> 8);     // low part of byte address

        if(set)
            LCD_command(SETBIT,pos);           // set corresponding bit in the byte value
        else
            LCD_command(CLEARBIT,pos);         // reset corresponding bit in the byte value
    }
}

void LCD_line(unsigned int xi,unsigned int yi,unsigned int xf,unsigned int yf,unsigned char set)
{
    int dx,dy,s,n,x,y;
    unsigned char d=0;

    if(yf > yi)
    {
        unsigned t;
        t=xf;
        xf=xi;
        xi=t;
        t=yf;
        yf=yi;
        yi=t;
    }
    if(xf < xi)
    {
        d=1;
    }
    dy=yi-yf;
    if(d)
        dx=xi-xf;
    else
        dx=xf-xi;
    x=xi;
    y=yi;
    if(dx>dy)
    {
        n=dx+1;
        s=-dx;
        dx=2*dx;
        dy=2*dy;
        while(n!=0)
        {
            LCD_putpixel(x,y,set);
            if(d)
                x--;
            else
                x++;
            s+=dy;
            if(s>=0)
            {
                y--;
                s-=dx;
            }
            n--;
        }
    }
    else
    {
        n=dy+1;
        s=-dy;
        dx=2*dx;
        dy=2*dy;
        while(n!=0)
        {
            LCD_putpixel(x,y,set);
            y--;
            s+=dx;
            if(s>=0)
            {
                if(d)
                    x--;
                else
                    x++;
                s-=dy;
            }
            n--;
        }
    }
}

void LCD_rectangle(unsigned int lx,unsigned int ly,unsigned int rx,unsigned int ry,unsigned char set)
{
    register unsigned int i;

    if(lx > rx)
    {
        unsigned t=rx;
        rx=lx;
        lx=t;
    }
    if(ly > ry)
    {
        unsigned t=ry;
        ry=ly;
        ly=t;
    }
    for(i=lx;i<=rx;i++)
    {
        LCD_putpixel(i,ly,set);
        LCD_putpixel(i,ry,set);
    }
    for(i=ly+1;i<ry;i++)
    {
        LCD_putpixel(lx,i,set);
        LCD_putpixel(rx,i,set);
    }
}

void LCD_circle(unsigned int x,unsigned int y,unsigned int r,unsigned char set)
{
    int u,v,s;
    int x1,y1,x2,y2;

    if(r==0)
    {
        LCD_putpixel(x,y,set);
        return;
    }
    x1=x + r;y1=y;
    x2=x - r;y2=y;
    u=2*r;v=0;s=1-r;
    LCD_putpixel(x1,y1,set);
    LCD_putpixel(x2,y2,set);
round_2:
;
    y1++;y2--;v+=2;
    if(u==v) goto round_9;
round_3:
;
    LCD_putpixel(x1,y1,set);
    LCD_putpixel(x2,y2,set);
    s+=v+1;
    if(s<0) goto round_2;
    x1--;y1++;x2++;y2--;
    u-=2;v+=2;s-=u;
    if(u>v) goto round_3;
    if(u!=v) s++;
    goto round_8;
round_6:
;
    x1--;y1++;x2++;y2--;
    u-=2;v+=2;s+=v;
round_7:
;
    s-=u;
round_8:
;
    LCD_putpixel(x1,y1,set);
    LCD_putpixel(x2,y2,set);
round_9:
;
    s++;
    if(s<0) goto round_6;
    x1--;x2++;u-=2;
    if(u!=0) goto round_7;
    LCD_putpixel(x1,y1,set);
    LCD_putpixel(x2,y2,set);
    s=1-s;
round_12:
;
    x1--;x2++;u+=2;
    if(u==v) goto round_19;
round_13:
;
    LCD_putpixel(x1,y1,set);
    LCD_putpixel(x2,y2,set);
    s+=u+1;
    if(s<0) goto round_12;
    x1--;y1--;x2++;y2++;
    u+=2;v-=2;s-=v;
    if(u<v) goto round_13;
    if(u!=v) s++;
    goto round_18;
round_16:
;
    x1--;y1--;x2++;y2++;
    u+=2;v-=2;s+=u;
round_17:
;
    s-=v;
round_18:
;
    LCD_putpixel(x1,y1,set);
    LCD_putpixel(x2,y2,set);
round_19:
;
    s++;
    if(s<0) goto round_16;
    y1--;y2++;v-=2;
    if(v!=0) goto round_17;
}

void LCD_bar(unsigned int lx,unsigned int ly,unsigned int rx,unsigned int ry, unsigned char set)
{
    register unsigned int i,k;

    if(lx > rx)
    {
        unsigned int t=rx;
        rx=lx;
        lx=t;
    }
    if(ly > ry)
    {
        unsigned int t=ry;
        ry=ly;
        ly=t;
    }
    for(i=lx;i<=rx;i++)
        for(k=ly;k<=ry;k++)
            LCD_putpixel(i,k,set);
}

void LCD_moveto(unsigned int x,unsigned int y)
{
    if(x<MAXX && y<MAXY)
    {
        current_x=x;
        current_y=y;
    }
}

void LCD_lineto(unsigned int x,unsigned int y)
{
    if(x<MAXX && y<MAXY)
    {
        LCD_line(current_x,current_y,x,y,1);
        LCD_moveto(x,y);
    }
}

void LCD_outtextxy(unsigned int x,unsigned int y,unsigned char *ms)
{
    unsigned int i;
    unsigned int f_off;
    unsigned int offset;

    while(*ms)
    {
        f_off=(unsigned char)*ms;
        f_off<<=3;                         // offset for font table
        for(i=0;i<8;i++,f_off++)
        {
            offset=(y+i)*30;
            offset+=(x>>3);
            LCD_command(CURSOR_LOW,offset & 0xFF);  // set address
            LCD_command(CURSOR_HIGH,offset >> 8);
            LCD_command(WRITEBYTE,font[f_off]);   // write character line
        }
        ms++;
        x+=8;
    }
}

void LCD_outtext(unsigned char *ms)
{
    unsigned int i,k;
    unsigned int f_off;
    unsigned int x,y;

    x=current_x;
    while(*ms)
    {
        f_off=(unsigned char)*ms;
        f_off<<=3;                         // offset for font table
        y=current_y;
        for(i=0;i<8;i++,f_off++,y++)
        {
            for(k=0;k<8;k++)
            {
                if(font[f_off] & ((unsigned char)0x01 << k))
                    LCD_putpixel(x+k,y,1);
            }
        }
        ms++;                             // next character
        x+=8;
    }
}


void LCD_window(unsigned int lx,unsigned int ly,unsigned int rx,unsigned int ry, unsigned char set)
{
    register unsigned int i;

    if(lx > rx)
    {
        unsigned t=rx;
        rx=lx;
        lx=t;
    }
    if(ly > ry)
    {
        unsigned t=ry;
        ry=ly;
        ly=t;
    }
    for(i=lx+2;i<=rx-2;i++)
    {
        LCD_putpixel(i,ly,set);
        LCD_putpixel(i,ry,set);
    }
    for(i=ly+2;i<=ry-2;i++)
    {
        LCD_putpixel(lx,i,set);
        LCD_putpixel(rx,i,set);
    }

    LCD_putpixel(lx+1,ly+1,set);
    LCD_putpixel(lx+1,ry-1,set);
    LCD_putpixel(rx-1,ly+1,set);
    LCD_putpixel(rx-1,ry-1,set);
}




void LCD_init_text()
{
    LCD_command(0x00, 0x38);	        // Display ON, master mode, cursor on/blink, text mode, internal CG-ROM
    LCD_command(0x01, 0x75);	        // 6x8 font
    LCD_command(0x02, 0x27);	        // 40 horizontal characters
    LCD_command(0x03, 0x7F);	        // 1/128 duty cycle (128 lines)
    LCD_command(0x04, 0x07);            // 8-lines cursor
    LCD_command(DISPLAY_LOW, 0x00);     // Display start address low
    LCD_command(DISPLAY_HIGH, 0x00);    // Display start address high
    LCD_command(CURSOR_LOW, 0x00);      // Cursor address low
    LCD_command(CURSOR_HIGH, 0x00);     // Cursor address high
}

void LCD_gotoxy(unsigned int x, unsigned int y)
{
    x+=y*TEXTMAXX;                    // absolute byte address
    LCD_command(CURSOR_LOW,x & 0xFF);        // set page start address
    LCD_command(CURSOR_HIGH,x >> 8);

}

void LCD_putch(unsigned char c)
{
    LCD_command(WRITEBYTE,c);        // output the character

}

void LCD_clear_text(void)
{
    register unsigned int i;

    LCD_gotoxy(0,0);                  // set page start address
    for(i=0;i<TEXTBYTE;i++) LCD_command(WRITEBYTE,0);   // reset all text memory
    LCD_gotoxy(0,0);
}








void Beep(unsigned int freq)
{
	gfpOut32(0x43, 0xB6);
	gfpOut32(0x42, (freq & 0xFF));
	gfpOut32(0x42, (freq >> 9));
	Sleep(10);
	gfpOut32(0x61, gfpInp32(0x61) | 0x03);
}

void StopBeep()
{
	gfpOut32(0x61, (gfpInp32(0x61) & 0xFC));
}



int win32_open_port(void)
{
	//Dynamically load the DLL at runtime (not linked at compile time)
	HINSTANCE hInpOutDll ;
	hInpOutDll = LoadLibrary ( "InpOut32.DLL" ) ;	//The 32bit DLL. If we are building x64 C++ 
													//applicaiton then use InpOutx64.dll
	if ( hInpOutDll != NULL )
	{
		gfpOut32 = (lpOut32)GetProcAddress(hInpOutDll, "Out32");
		gfpInp32 = (lpInp32)GetProcAddress(hInpOutDll, "Inp32");
		gfpIsInpOutDriverOpen = (lpIsInpOutDriverOpen)GetProcAddress(hInpOutDll, "IsInpOutDriverOpen");
		gfpIsXP64Bit = (lpIsXP64Bit)GetProcAddress(hInpOutDll, "IsXP64Bit");

		if (gfpIsInpOutDriverOpen())
		{
			//Make some noise through the PC Speaker - hey it can do more that a single beep using InpOut32
			Beep(2000);
			Sleep(200);
			Beep(1000);
			Sleep(300);
			Beep(2000);
			Sleep(250);
			StopBeep();

//			if(!strcmp(argv[1],"read"))
//			{
//				short iPort = atoi(argv[2]);
				short iPort = 0x378;
				WORD wData = gfpInp32(iPort);	//Read the port
				printf("Data read from address %s is %d \n\n\n\n", 0x378, wData);
//				printf("Data read from address %s is %d \n\n\n\n", argv[2], wData);
//			}
//			else if(!strcmp(argv[1],"write"))
//			{
//				if(argc<4)
//				{
//					printf("Error in arguments supplied");
//					printf("\n***** Usage *****\n\nInpoutTest read <ADDRESS> \nor \nInpoutTest write <ADDRESS> <DATA>\n\n\n\n\n");
//				}
//				else
//				{
//					short iPort = atoi(argv[2]);
					iPort = 0x378;
//					WORD wData = atoi(argv[3]);
					wData = 0x1234;
					gfpOut32(iPort, wData);
					gfpOut32(iPort, wData);
//					printf("data written to %s\n\n\n", argv[2]);
					printf("data written to %s\n\n\n", 0x1234);

//				}
//			}
		}
		else
		{
			printf("Unable to open InpOut32 Driver!\n");
		}

		//All done
		FreeLibrary ( hInpOutDll ) ;
		return 0;
	}
	else
	{
		printf("Unable to load InpOut32 DLL!\n");
		return -1;
	}
}