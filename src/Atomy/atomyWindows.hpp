#include <windows.h>
#include <iostream>
#include <vector>
namespace Atomy
{
  namespace Mouse {
    enum MouseButton {
      LeftButton,
      MiddleButton,
      RightButton
    };
    void moveMouseTo(int x, int y) { SetCursorPos(x, y); }
    void mouseButtonDown(MouseButton button = LeftButton) {
      INPUT input = {};
      input.type = INPUT_MOUSE;
      input.mi.dwFlags = (button == LeftButton) ? MOUSEEVENTF_LEFTDOWN :
                         (button == MiddleButton) ? MOUSEEVENTF_MIDDLEDOWN :
                         MOUSEEVENTF_RIGHTDOWN;
      SendInput(1, &input, sizeof(INPUT));
    }
    void mouseButtonUp(MouseButton button = LeftButton) {
      INPUT input = {};
      input.type = INPUT_MOUSE;
      input.mi.dwFlags = (button == LeftButton) ? MOUSEEVENTF_LEFTUP :
                         (button == MiddleButton) ? MOUSEEVENTF_MIDDLEUP :
                         MOUSEEVENTF_RIGHTUP;
      SendInput(1, &input, sizeof(INPUT));
    }
    void mouseButtonClick(MouseButton button = LeftButton) {
      mouseButtonDown(button);
      mouseButtonUp(button);
    }
    void getMousePosition(int &x, int &y) {
      POINT p;
      if (GetCursorPos(&p)) {
        x = p.x;
        y = p.y;
      } else {
        std::cerr << "Failed to get mouse position\n";
      }
    }
    void scrollMouse(int deltaY) {
      INPUT input = {};
      input.type = INPUT_MOUSE;
      input.mi.dwFlags = MOUSEEVENTF_WHEEL;
      input.mi.mouseData = deltaY * WHEEL_DELTA;
      SendInput(1, &input, sizeof(INPUT));
    }
    bool isMouseButtonPressed(MouseButton button = LeftButton) {
      int vkButton = (button == LeftButton) ? VK_LBUTTON :
                     (button == MiddleButton) ? VK_MBUTTON :
                     VK_RBUTTON;
      return (GetAsyncKeyState(vkButton) & 0x8000) != 0;
    }
  }
  namespace Keyboard {
    void keyDown(int key) {
      INPUT input = {};
      input.type = INPUT_KEYBOARD;
      input.ki.wVk = key;
      SendInput(1, &input, sizeof(INPUT));
    }
    void keyUp(int key) {
      INPUT input = {};
      input.type = INPUT_KEYBOARD;
      input.ki.wVk = key;
      input.ki.dwFlags = KEYEVENTF_KEYUP;
      SendInput(1, &input, sizeof(INPUT));
    }
    void keyPress(int key) {
      keyDown(key);
      keyUp(key);
    }
    bool isKeyPressed(int key) {
      return (GetAsyncKeyState(key) & 0x8000) != 0;
    }
  }
  namespace Monitor {
    void getScreenSize(int &width, int &height) {
      RECT desktop;
      GetWindowRect(GetDesktopWindow(), &desktop);
      width = desktop.right - desktop.left;
      height = desktop.bottom - desktop.top;
    }
    void takeScreenshot(const std::string &filename) {
      HDC hScreenDC = GetDC(nullptr);
      HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
      int width = GetSystemMetrics(SM_CXSCREEN);
      int height = GetSystemMetrics(SM_CYSCREEN);
      HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
      SelectObject(hMemoryDC, hBitmap);
      BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

      BITMAPINFOHEADER bi = {};
      bi.biSize = sizeof(BITMAPINFOHEADER);
      bi.biWidth = width;
      bi.biHeight = -height; // Negative to flip the image
      bi.biPlanes = 1;
      bi.biBitCount = 32;
      bi.biCompression = BI_RGB;

      std::vector<unsigned char> pixels(width * height * 4);
      GetDIBits(hMemoryDC, hBitmap, 0, height, pixels.data(), reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

      stbi_write_png(filename.c_str(), width, height, 4, pixels.data(), width * 4);

      DeleteObject(hBitmap);
      DeleteDC(hMemoryDC);
      ReleaseDC(nullptr, hScreenDC);
    }
  }
  namespace General {
    void sleep(int duration, const std::string &unit = "ms") {
      if (unit == "us")
        Sleep(duration / 1000);
      else if (unit == "ms")
        Sleep(duration);
      else if (unit == "s")
        Sleep(duration * 1000);
      else if (unit == "m")
        Sleep(duration * 60000);
      else if (unit == "h")
        Sleep(duration * 3600000);
      else
        std::cerr << "Unsupported time unit: " << unit << "\n";
    }
  }
} 