#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

//// c:\mingw\include\dos.h:54:2: warning: #warning "<dos.h> is obsolete; consider using <direct.h> instead." [-Wcpp]
//// #warning "<dos.h> is obsolete; consider using <direct.h> instead."
//#include <dos.h>
//#include <direct.h>	//
#include <windows.h>


#include "lcdlib.h"

#define NRVAL   100
#define XSTART   30
#define YMIN     10
#define YLIMIT  100

#define ZMAX 400
#define ZMIN (-200)
#define ZDEPTH (ZMAX-ZMIN)
#define ZSMALL (ZMIN+1.0)

#define SCOUNT  30                    // number of stars
#define SW 240                        // screen width
#define SH 128                        // screen height
#define TOTAL_FRAMES  60              // number of frames
#define CENX  120                     // X center
#define CENY  64                      // Y center
#define DIST  ZDEPTH/TOTAL_FRAMES     // distance to move a star
#define CR  1                         // circle (star) radius

int *x, *y, *z;

static unsigned char val[NRVAL]={\
        40, 39, 38, 37, 37, 38, 37, 38, 39, 40,
        41, 42, 42, 41, 42, 43, 43, 43, 44, 43,
        42, 42, 41, 41, 42, 43, 43, 43, 44, 45,
        46, 47, 46, 46, 47, 48, 48, 48, 48, 48,
        47, 47, 48, 47, 46, 45, 45, 45, 45, 44,
        45, 45, 45, 45, 45, 44, 44, 43, 44, 45,
        46, 47, 47, 47, 46, 45, 44, 43, 42, 41,
        41, 42, 42, 41, 42, 42, 42, 41, 42, 42,
        41, 42, 43, 43, 42, 43, 44, 43, 44, 44,
        45, 44, 44, 45, 45, 46, 46, 46, 47, 47};


void test1()
{
    unsigned int i;

    LCD_init_text();
    LCD_clear_text();
    for(i=0;i<16;i++)
    {
      LCD_gotoxy(i,i);
      LCD_putch('0'+i);
    }
}

void test2()
{
    unsigned int i;

    LCD_init_graph();
    LCD_clear_graph();


    LCD_rectangle(0,0,239,127,1);
    LCD_rectangle(XSTART-1,YMIN-1,XSTART+NRVAL+1,YLIMIT+1,1);

    for(i=XSTART;i<(XSTART+NRVAL);i++)
        LCD_putpixel(i,val[i-XSTART],1);

    for(i=XSTART;i<(XSTART+NRVAL);i++)
    {
        LCD_line(i,YLIMIT,i,val[i-XSTART]+1,1);
        //delay(100);	// dos.h
        Sleep(100);	// win32.h		
        LCD_line(i,YLIMIT,i,val[i-XSTART]+1,0);
    }

    LCD_line(5,5,200,30,1);
    LCD_circle(120,64,40,1);

    LCD_bar(20,100,200,120,1);
    LCD_bar(150,80,200,110,0);
}

void see_stars()                       //  Draw all the stars
{
    int i;
    double zscale,zz;

    for (i=0; i<SCOUNT; i++)
    {
	zz = z[i];                      // get Z coordinate
	if (zz > ZSMALL)
	{
            zscale = -ZMIN/(zz-ZMIN);   // find perspective shrink factor
            LCD_circle(CENX+x[i]*zscale,CENY+y[i]*zscale,CR,1);
//            LCD_putpixel(CENX+x[i]*zscale,CENY+y[i]*zscale,1);
	}
    }
}

void move_star(int starnr)        // Move a star towards viewer
{
    double zscale,zz;

    zz = z[starnr];             // deletes the star
    if (zz > ZSMALL)
    {
        zscale = -ZMIN/(zz-ZMIN);
        LCD_circle(CENX+x[starnr]*zscale,CENY+y[starnr]*zscale,CR,0);
//        LCD_putpixel(CENX+x[starnr]*zscale,CENY+y[starnr]*zscale,0);
    }
    if ((z[starnr] -= DIST) < ZMIN)
        z[starnr] += ZDEPTH;
    zz = z[starnr];             // and then redraws it
    if (zz > ZSMALL)
    {
        zscale = -ZMIN/(zz-ZMIN);
        LCD_circle(CENX+x[starnr]*zscale,CENY+y[starnr]*zscale,CR,1);
//        LCD_putpixel(CENX+x[starnr]*zscale,CENY+y[starnr]*zscale,1);
    }
}

void random_star(int starix)
/* Initialize the data associated with a star index to a random value */
{
    x[starix] = random(SW)-CENX;
    y[starix] = random(SH)-CENY;
    z[starix] = random(ZDEPTH+100)-100;
}

int init_stars(int count)
/* Allocate star arrays and set all stars to random positions */
{
    unsigned int i;

    if ((x = (int *)malloc(count * sizeof(int))) == NULL)
        return 0;
    if ((y = (int *)malloc(count * sizeof(int))) == NULL)
        return 0;
    if ((z = (int *)malloc(count * sizeof(int))) == NULL)
        return 0;
    for (i=0; i<count;i++)
        random_star(i);
    return 1;
}

void test3()
{
    unsigned int i=0;

    LCD_init_graph();
    LCD_clear_graph();

    if (!init_stars(SCOUNT))
        return;
    see_stars();
    while(!kbhit())
    {
        delay(5);
        move_star(i);
        i++;
        if(i == SCOUNT)
            i=0;
    }

}

void test4()
{
    LCD_init_graph();
    LCD_clear_graph();

    LCD_putpixel(100,100,1);
    LCD_circle(100,100,5,1);
    LCD_clear_graph();
    LCD_window(5,5,87,23,1);
    LCD_moveto(10,10);
    LCD_loadfont("8x8_font.lcd");
    LCD_outtext("Friti LCD");
}

#include "inpout32.h"	//win32 IO access

int main()
{
	win32_open_port();
    test1();
    test2();
    test3();
    test4();
	return 0;
}