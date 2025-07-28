#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include "stb_image_write.h"
namespace Atomy
{
  namespace Mouse
  {
    enum MouseButton
    {
      LeftButton,
      MiddleButton,
      RightButton
    };

    void moveMouseTo(int x, int y)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      XTestFakeMotionEvent(display, 0, x, y, CurrentTime);
      XFlush(display);
      XCloseDisplay(display);
    }

    void mouseButtonDown(MouseButton button = LeftButton)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      int code = (button == LeftButton) ? Button1 : (button == MiddleButton) ? Button2
                                                                             : Button3;

      XTestFakeButtonEvent(display, code, True, CurrentTime);
      XFlush(display);
      XCloseDisplay(display);
    }

    void mouseButtonUp(MouseButton button = LeftButton)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      int code = (button == LeftButton) ? Button1 : (button == MiddleButton) ? Button2
                                                                             : Button3;

      XTestFakeButtonEvent(display, code, False, CurrentTime);
      XFlush(display);
      XCloseDisplay(display);
    }

    void clickMouseButton(MouseButton button = LeftButton)
    {
      mouseButtonDown(button);
      mouseButtonUp(button);
    }

    void getMousePosition(int &x, int &y)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      Window root, child;
      int rx, ry, wx, wy;
      unsigned int mask;
      if (XQueryPointer(display, DefaultRootWindow(display), &root, &child, &rx, &ry, &wx, &wy, &mask))
      {
        x = rx;
        y = ry;
      }
      else
      {
        std::cerr << "Failed to get mouse position\n";
      }
      XCloseDisplay(display);
    }

    void scrollMouse(int deltaY)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      int button = (deltaY > 0) ? Button4 : Button5;
      for (int i = 0; i < std::abs(deltaY); ++i)
      {
        XTestFakeButtonEvent(display, button, True, CurrentTime);
        XTestFakeButtonEvent(display, button, False, CurrentTime);
      }
      XFlush(display);
      XCloseDisplay(display);
    }

    bool isMouseButtonPressed(MouseButton button = LeftButton)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return false;
      }
      Window root, child;
      int rx, ry, wx, wy;
      unsigned int mask;
      Bool result = XQueryPointer(display, DefaultRootWindow(display),
                                  &root, &child, &rx, &ry, &wx, &wy, &mask);
      XCloseDisplay(display);
      if (!result)
        return false;

      switch (button)
      {
      case LeftButton:
        return mask & Button1Mask;
      case MiddleButton:
        return mask & Button2Mask;
      case RightButton:
        return mask & Button3Mask;
      }
      return false;
    }
  };
  namespace Keyboard
  {
    void keyDown(KeySym key)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      KeyCode code = XKeysymToKeycode(display, key);
      if (code == 0)
      {
        std::cerr << "Invalid key symbol\n";
        XCloseDisplay(display);
        return;
      }
      XTestFakeKeyEvent(display, code, True, CurrentTime);
      XFlush(display);
      XCloseDisplay(display);
    }

    void keyUp(KeySym key)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      KeyCode code = XKeysymToKeycode(display, key);
      if (code == 0)
      {
        std::cerr << "Invalid key symbol\n";
        XCloseDisplay(display);
        return;
      }
      XTestFakeKeyEvent(display, code, False, CurrentTime);
      XFlush(display);
      XCloseDisplay(display);
    }

    void keyPress(KeySym key)
    {
      keyDown(key);
      keyUp(key);
    }

    bool isKeyPressed(KeySym key)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return false;
      }
      char keys[32];
      XQueryKeymap(display, keys);
      KeyCode code = XKeysymToKeycode(display, key);
      bool pressed = keys[code / 8] & (1 << (code % 8));
      XCloseDisplay(display);
      return pressed;
    }
  };
  namespace Monitor
  {
    void getScreenSize(int &width, int &height)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      Screen *screen = DefaultScreenOfDisplay(display);
      width = screen->width;
      height = screen->height;
      XCloseDisplay(display);
    }
    void takeScreenshot(const std::string &filename)
    {
      Display *display = XOpenDisplay(nullptr);
      if (!display)
      {
        std::cerr << "Cannot open display\n";
        return;
      }
      Window root = DefaultRootWindow(display);
      XWindowAttributes attr;
      XGetWindowAttributes(display, root, &attr);

      XImage *image = XGetImage(display, root, 0, 0, attr.width, attr.height, AllPlanes, ZPixmap);
      if (!image)
      {
        std::cerr << "Failed to get image\n";
        XCloseDisplay(display);
        return;
      }
      int channels = 3;
      if (image->depth == 32)
      {
        channels = 4;
      }
      else if (image->depth == 24)
      {
        channels = 3;
      }
      else
      {
        std::cerr << "Unsupported image depth: " << image->depth << "\n";
        XDestroyImage(image);
        XCloseDisplay(display);
        return;
      }
      int width = image->width;
      int height = image->height;
      std::vector<unsigned char> pixels(width * height * channels);
      for (int y = 0; y < height; ++y)
      {
        for (int x = 0; x < width; ++x)
        {
          unsigned long pixel = XGetPixel(image, x, y);
          unsigned char blue = pixel & 0xff;
          unsigned char green = (pixel >> 8) & 0xff;
          unsigned char red = (pixel >> 16) & 0xff;
          int index = (y * width + x) * channels;
          pixels[index + 0] = red;
          pixels[index + 1] = green;
          pixels[index + 2] = blue;
        }
      }
      if (!stbi_write_png(filename.c_str(), width, height, channels, pixels.data(), width * channels))
      {
        std::cerr << "Failed to write image to file\n";
      }
      XDestroyImage(image);
      XCloseDisplay(display);
    }
  };
  namespace General
  {
    void sleep(int duration, const std::string &unit = "ms")
    {
      if (unit == "us")
        usleep(duration);
      else if (unit == "ms")
        usleep(duration * 1000);
      else if (unit == "s")
        usleep(duration * 1000000);
      else if (unit == "m")
        usleep(duration * 60000000);
      else if (unit == "h")
        usleep(duration * 3600000000);
      else
        std::cerr << "Invalid time unit\n";
    }
  };
}
