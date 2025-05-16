#include <X11/X.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include <pngconf.h>
#include <setjmp.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <png.h>

unsigned char * to_rgb(XImage * image, int width, int height){
    unsigned char* rgb_data = malloc(width * height * 3);
    
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            unsigned long pixel = XGetPixel(image, x, y);

            unsigned char b = pixel & 0xff;
            unsigned char g = (pixel >> 8) & 0xff;
            unsigned char r = (pixel >> 16) & 0xff;

            int i = (y * width + x) * 3;
            rgb_data[i] = r;
            rgb_data[i+1] = g;
            rgb_data[i+2] = b;
        }
    }

    FILE *f = fopen("raw.rgb", "wb");
    fwrite(rgb_data, 1, width * height * 3, f);
    fclose(f);

    return rgb_data;
}

void to_png(unsigned char * rgb_data, int width, int height){
    FILE *fp = fopen("image.png", "wb");
    if(!fp){
        printf("Cannot open file exiting \n");
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);

    if(!png || !info || setjmp(png_jmpbuf(png))){
        printf("cannot create png file, exiting \n");
        fclose(fp);
        return;
    }

    png_init_io(png, fp);

    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png, info);

    png_bytep row_pointers[height];
    for (int y = 0; y < height; ++y) {
        row_pointers[y] = rgb_data + y * width * 3;
    }

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

int main(){
    printf("Starting program");

    Display *rootDisplay = XOpenDisplay(NULL);
    Window rootWindow;
    int screenNumber;
    XWindowAttributes returnWindowAttributes;
    
    if(rootDisplay == NULL)
    {
        printf("cannot open connection to display");
        return 0;
    }

    printf("Opened display");

    screenNumber = XDefaultScreen(rootDisplay);
    rootWindow = RootWindow(rootDisplay, screenNumber);
    XGetWindowAttributes(rootDisplay, rootWindow, &returnWindowAttributes);

    Drawable d = rootWindow;
    int x = returnWindowAttributes.x;
    int y = returnWindowAttributes.y;
    int width = returnWindowAttributes.width;
    int height = returnWindowAttributes.height;
    
    printf("%d - %d , %d - %d \n",x, y, width, height);

    XImage *image = XGetImage(rootDisplay, d, x, y, width, height, AllPlanes, ZPixmap);
    if(!image){
        printf("Cannot get image \n");
        return 0;
    }

    unsigned char * rgb_data = to_rgb(image, width, height);
    to_png(rgb_data,width, height);

    XCloseDisplay(rootDisplay);
    return 0;
}