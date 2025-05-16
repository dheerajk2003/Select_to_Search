#include <X11/X.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<stdio.h>
#include<stdlib.h>

void to_rgb(XImage * image, int width, int height){
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

    to_rgb(image, width, height);

    XCloseDisplay(rootDisplay);
    return 0;
}