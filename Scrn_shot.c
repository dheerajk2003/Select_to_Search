#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <png.h>
#include <pngconf.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include"Req_res.c"
#include"Print_text.c"

Display *rootDisplay;
Window rootWindow;
int screenNumber;
XWindowAttributes returnWindowAttributes;
Drawable d;
Cursor crosshair;

unsigned char *to_rgb(XImage *image, int width, int height) {
  unsigned char *rgb_data = malloc(width * height * 3);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      unsigned long pixel = XGetPixel(image, x, y);

      unsigned char b = pixel & 0xff;
      unsigned char g = (pixel >> 8) & 0xff;
      unsigned char r = (pixel >> 16) & 0xff;

      int i = (y * width + x) * 3;
      rgb_data[i] = r;
      rgb_data[i + 1] = g;
      rgb_data[i + 2] = b;
    }
  }

  FILE *f = fopen("raw.rgb", "wb");
  fwrite(rgb_data, 1, width * height * 3, f);
  fclose(f);

  return rgb_data;
}

void to_png(unsigned char *rgb_data, int width, int height) {
  FILE *fp = fopen("image.png", "wb");
  if (!fp) {
    printf("Cannot open file exiting \n");
    return;
  }

  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);

  if (!png || !info || setjmp(png_jmpbuf(png))) {
    printf("cannot create png file, exiting \n");
    fclose(fp);
    return;
  }

  png_init_io(png, fp);

  png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

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

void init() {
  rootDisplay = XOpenDisplay(NULL);
  if (!rootDisplay) {
    printf("cannot open connection to display \n");
    return;
  }

  screenNumber = XDefaultScreen(rootDisplay);
  rootWindow = RootWindow(rootDisplay, screenNumber);
  XGetWindowAttributes(rootDisplay, rootWindow, &returnWindowAttributes);
  d = rootWindow;
}

void ScreenShot(int x, int y, int width, int height) {
  init();

  if (width == 0 && height == 0) {
    width = returnWindowAttributes.width;
    height = returnWindowAttributes.height;
  }

  printf("%d - %d , %d - %d \n", x, y, width, height);

  XImage *image =
      XGetImage(rootDisplay, d, x, y, width, height, AllPlanes, ZPixmap);
  if (!image) {
    printf("Cannot get image \n");
    return;
  }

  unsigned char *rgb_data = to_rgb(image, width, height);
  to_png(rgb_data, width, height);

  free(rgb_data);

  XCloseDisplay(rootDisplay);
}

void SelectScreen() {
  init();
  int xStart, yStart, xEnd, yEnd, lastX, lastY;
  xStart = yStart = xEnd = yEnd = lastX = lastY = 0;
  int displayWidth = DisplayWidth(rootDisplay, screenNumber);
  int displayHeight = DisplayHeight(rootDisplay, screenNumber);

  XSetWindowAttributes attr;
  attr.override_redirect = True;
  attr.event_mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

  Window newWindow =
      XCreateWindow(rootDisplay, rootWindow, 0, 0, displayWidth, displayHeight,
                    0, CopyFromParent, InputOutput, CopyFromParent,
                    CWOverrideRedirect | CWEventMask, &attr);
  XMapWindow(rootDisplay, newWindow);
  XFlush(rootDisplay);

  GC gc = XCreateGC(rootDisplay, newWindow, 0, NULL);
  d = newWindow;
  XSetFunction(rootDisplay, gc, GXxor);
  XSetForeground(rootDisplay, gc, WhitePixel(rootDisplay, screenNumber));

  crosshair = XCreateFontCursor(rootDisplay, XC_crosshair);

  // XSelectInput(rootDisplay, newWindow, ButtonPressMask | ButtonReleaseMask);
  XEvent ev;
  int status = XGrabPointer(rootDisplay, newWindow, True, PointerMotionMask,
                            GrabModeAsync, GrabModeAsync, newWindow, crosshair,
                            CurrentTime);
  Bool pressed = False;
  while (1) {
    XNextEvent(rootDisplay, &ev);

    if (ev.type == ButtonPress && ev.xbutton.button == 1) {
      printf("pressed \n");
      xStart = ev.xbutton.x_root;
      yStart = ev.xbutton.y_root;
      lastX = xStart;
      lastY = yStart;
      pressed = True;
    }
    if (pressed && ev.type == MotionNotify) {
      XDrawRectangle(rootDisplay, d, gc, xStart, yStart, lastX - xStart,
                     lastY - yStart);
      lastX = ev.xmotion.x;
      lastY = ev.xmotion.y;
      XDrawRectangle(rootDisplay, d, gc, xStart, yStart, lastX - xStart,
                     lastY - yStart);
      XFlush(rootDisplay);
    }
    if (ev.type == ButtonRelease && ev.xbutton.button == 1) {
      printf("released \n");
      XDrawRectangle(rootDisplay, d, gc, xStart, yStart, lastX - xStart,
                     lastY - yStart);
      xEnd = ev.xbutton.x_root;
      yEnd = ev.xbutton.y_root;
      break;
    }
  }

  XUnmapWindow(rootDisplay, newWindow);
  XFlush(rootDisplay);
  XFreeCursor(rootDisplay, crosshair);
  ScreenShot(xStart, yStart, xEnd - xStart, yEnd - yStart);
}

int main() {
  // ScreenShot(0,0,0,0);
  SelectScreen();
  char *json_text = requesting("image.png", "What is in this image describe , if its a product also give buying link. (And don't give markdown , just simple text. Your responce should not exceed 2000 characters.)");
  printf("JSON Text = %s", json_text);
  char *text = get_text(json_text);
  Notify(text);
  printf("Response = %s\n", text);

  // requesting("image.png", "if there is an product in the given image then give links , otherwise just describe image.");
  return 0;
}