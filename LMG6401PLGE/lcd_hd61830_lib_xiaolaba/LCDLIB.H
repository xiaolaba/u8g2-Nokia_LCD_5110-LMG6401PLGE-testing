// exported functions of LCD library

// inits LCD to graphic mode
extern void LCD_init_graph(void);
// loads a font file
extern unsigned int LCD_loadfont(char *name);
// cleares the screen, graphic mode
extern void LCD_clear_graph(void);
// resets the entire RAM memory
extern void LCD_clear_memory(void);
// set = 1 for set and 0 for clear a pixel
extern void LCD_putpixel(unsigned int x,unsigned int y, unsigned char set);
// line parameters: start x, start y, end x, end y, set/clear
extern void LCD_line(unsigned int xi,unsigned int yi,unsigned int xf,unsigned int yf,unsigned char set);
// rectangle parameters: left x, left y, right x, right y, set/clear
extern void LCD_rectangle(unsigned int lx,unsigned int ly,unsigned int rx,unsigned int ry,unsigned char set);
// circle parameters: center x, center y, radius, set/clear
extern void LCD_circle(unsigned int x,unsigned int y,unsigned int r,unsigned char set);
// draws a filled rectangle
extern void LCD_bar(unsigned int lx,unsigned int ly,unsigned int rx,unsigned int ry, unsigned char set);
// sets current graphic coordinates
extern void LCD_moveto(unsigned int x,unsigned int y);
// draws a line to the specificated graphic coordinates
extern void LCD_lineto(unsigned int x,unsigned int y);
// outputs a text to current position
extern void LCD_outtext(unsigned char *ms);
// outputs a text to given text coordinate position
extern void LCD_outtextxy(unsigned int x,unsigned int y,unsigned char *ms);
// draws a round corner rectangle
extern void LCD_window(unsigned int lx,unsigned int ly,unsigned int rx,unsigned int ry, unsigned char set);

// inits LCD to text mode
extern void LCD_init_text(void);
// setx x and y as current cursor position
extern void LCD_gotoxy(unsigned int x, unsigned int y);
// dispalays character c at current cursor position
extern void LCD_putch(unsigned char c);
// cleares the screen, text mode
extern void LCD_clear_text(void);


extern int win32_open_port(void);
extern void StopBeep(void);
extern void Beep(unsigned int freq);
