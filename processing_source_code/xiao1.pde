// demo : https://youtu.be/NrdAuk-bxzk 
// ref : https://www.youtube.com/watch?v=0L2n8Tg2FwI
// by xiaolaba, 2021-AUG-17


String img_in = "xiao1.JPG";  // case sensitive
String img_out = "xiao5.jpg"; // case sensitive

PImage xiao1; // image source
PImage newImage = createImage(100,48,ARGB);   // target image

void setup () {
  size (200,48);
  //size (1024,768);
  
//  xiao1 = loadImage("xiao1.JPG");
  xiao1 = loadImage(img_in);

  xiao1.filter(GRAY);  // color to grey scale image
  image (xiao1, 0, 0);
}

int index(int x, int y) {
  return x + y * xiao1.width;
}


void draw() {
  xiao1.loadPixels();
  for (int y=0; y < xiao1.height-1; y++ ) {
    for (int x=1; x < xiao1.width-1; x++ ){
       color pix = xiao1.pixels[index(x,y)];      
       float oldR = red (pix);
       float oldG = green (pix);
       float oldB = blue (pix);
       
       int factor = 1 ;
       
       // quantise and error
       int newR = round (factor * oldR/255) * (255/factor);  // 0,1,2,3, or 4 x 255
       int newG = round (factor * oldG/255) * (255/factor);
       int newB = round (factor * oldB/255) * (255/factor);
       xiao1.pixels[index(x,y)] = color(newR,newG,newB);
        
       float errR = oldR - newR;
       float errG = oldG - newG;
       float errB = oldB - newB;
       
       int index = index(x+1, y );
       color c = xiao1.pixels[index];
       float r = red(c);
       float g = green(c);
       float b = blue(c);
       r = r + errR * 7/16.0;
       g = g + errG * 7/16.0;
       b = b + errB * 7/16.0;
       xiao1.pixels[index] = color(r,g,b);
       

       index = index(x-1, y+1 );
       c = xiao1.pixels[index];
       r = red(c);
       g = green(c);
       b= blue(c);
       r = r + errR * 3/16.0;
       g = g + errG * 3/16.0;
       b = b + errB * 3/16.0;
       xiao1.pixels[index] = color(r,g,b);       

       index = index(x, y+1 );
       c = xiao1.pixels[index];
       r = red(c);
       g = green(c);
       b= blue(c);
       r = r + errR * 5/16.0;
       g = g + errG * 5/16.0;
       b = b + errB * 5/16.0;
       xiao1.pixels[index] = color(r,g,b);   

       index = index(x+1, y+1 );
       c = xiao1.pixels[index];
       r = red(c);
       g = green(c);
       b= blue(c);
       r = r + errR * 1/16.0;
       g = g + errG * 1/16.0;
       b = b + errB * 1/16.0;
       xiao1.pixels[index] = color(r,g,b); 
       
       
       //xiao1.pixels[index] = color(r,g,b);

/*       
       pixels[x + 1][y    ] := pixels[x + 1][y    ] + quant_error × 7 / 16;
       pixels[x - 1][y + 1] := pixels[x - 1][y + 1] + quant_error × 3 / 16;
       pixels[x    ][y + 1] := pixels[x    ][y + 1] + quant_error × 5 / 16;
       pixels[x + 1][y + 1] := pixels[x + 1][y + 1] + quant_error × 1 / 16;
*/       
       
       
    }
  }
  xiao1.updatePixels();
  
  image(xiao1, 100, 0);  // show the image
  
  newImage = xiao1.get();  // set the object for file output
  
  newImage.save("xiao5.jpg");  // save to file
    
}
