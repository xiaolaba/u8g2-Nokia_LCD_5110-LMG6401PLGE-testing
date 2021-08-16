/*
                  LCD  Font Generator

*/

#include <stdio.h>
#include <conio.h>
//#include <graphics.h>

typedef unsigned char byte;
FILE *outf;

void getit()
{
    byte sv[8],t;
    int i,k;

    for(i=0;i<8;i++)
    {
        sv[i]=0;
        for(k=0;k<8;k++)
        {
            t=getpixel(k+10,i+10);
            if(t)
                sv[i]|=((unsigned char)0x01 << k);
        }
    }
    fwrite(sv,8,1,outf);
}

void putit(int wh)
{
    char c[2];
    c[1]='\x0';
    c[0]=wh;
    setcolor(0);
    setfillstyle(SOLID_FILL,0);
    bar3d(10,10,20,20,0,0);
    setcolor(1);
    settextstyle(0,0,1);
    moveto(10,10);
    outtext(c);
}

void main(int nr, char *ln[])
{
    int gd=DETECT,gm;
    int i;

    if(nr<2)
    {
        puts("No output filename in command line.");
        return;
    }
    outf=fopen(ln[1],"wb");
    if(!outf)
    {
        puts("Couldn't open output file.");
        return;
    }
    initgraph(&gd,&gm,"c:\\bc\\bgi");
    for(i=0;i<256;i++)
    {
        putit(i);
        getit();
    }
    closegraph();
    fclose(outf);
    puts("OK. Font file generated.");
}
