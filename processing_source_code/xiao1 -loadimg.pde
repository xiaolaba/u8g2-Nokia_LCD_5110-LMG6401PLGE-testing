PImage xiao1;

void setup () {
  size (84,48);
  xiao1 = loadImage("xiao1.jpg");
  image (xiao1, 0,0);
 
}

void draw() {
  xiao1.loadPixels();
  for (int x=0; x < xiao1.width; x++ ){
    for (int y=0; y < xiao1.height; y++ ) {
       int index = x + y * xiao1.width;
       int pix = xiao1.pixels[index];
       
       float r = red (pix);
       float g = green (pix);
       float b = blue (pix);
       
       r = round (r/255) * 255;
       g = round (g/255) * 255;
       b = round (b/255) * 255;
       
       xiao1.pixels[index] = color(r,g,b);
       
    }
  }
  xiao1.updatePixels();
  image(xiao1, 512,0);
    
}
