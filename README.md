# Atomy

A Desktop Automation Library in C++ (Linux/Windows)  
=============================================

The **Atomy** library provides simple mouse, keyboard, screen control, and screenshot functionality through a single header file.

## Quick Usage

1. Place the `atomy.hpp` file in your project.
2. Add the following line to your code:
```cpp
#include "atomy.hpp"
```
3. Use the functions directly from the `Atomy` namespace.

---

## Mouse Functions

### Move the Mouse
```cpp
Atomy::Mouse::moveMouseTo(x, y); // Moves the cursor to coordinates (x, y)
```

### Mouse Button Press
```cpp
Atomy::Mouse::mouseKeyDown(Atomy::LeftButton);   // Press the left mouse button
Atomy::Mouse::mouseKeyUp(Atomy::LeftButton);     // Release the left mouse button
Atomy::Mouse::clickMouse(Atomy::RightButton);    // Click the right mouse button
```

### Get Mouse Position
```cpp
int x, y;
Atomy::Mouse::getMousePosition(x, y);
```

### Scroll the Mouse
```cpp
Atomy::Mouse::scrollMouse(1);   // Scroll up
Atomy::Mouse::scrollMouse(-1);  // Scroll down
```

### Check Mouse Button State
```cpp
if (Atomy::Mouse::isMouseButtonPressed(Atomy::LeftButton)) {
    // Left button is currently pressed
}
```

---

## Keyboard Functions

<p>Using keyboard functions can be a bit tricky, as you must pass platform-specific key codes. On Linux, use `XK`; on Windows, use `VK`.</p>

### KeyDown
Linux:
```cpp
Atomy::Keyboard::keyDown(XK_Return);
```
Windows:
```cpp
Atomy::Keyboard::keyDown(VK_Return);
```

## Available Keyboard Functions
* `keyDown`
* `keyUp`
* `keyPress`
* `isKeyPressed`

You can use all of them under the `Atomy::Keyboard` namespace.

---

## Screen Functions

### Get Screen Size
```cpp
int width, height;
Atomy::Monitor::getScreenSize(width, height);
```

### Take a Screenshot
```cpp
Atomy::Monitor::takeScreenshot("screenshot.png");
```

---

## General Functions

### Sleep
```cpp
Atomy::General::sleep(500, "ms"); // Wait for 500 milliseconds
Atomy::General::sleep(2, "s");    // Wait for 2 seconds
```

---

## Notes
- The library supports both Linux and Windows out of the box.
- On Linux, you must link against `X11` and `Xtst` (`-lX11 -lXtst`).
- Some functions may require elevated permissions depending on the system.

---

## Full Example
```cpp
#include "atomy.hpp"

int main() {
    Atomy::Mouse::moveMouseTo(100, 100);
    Atomy::Mouse::clickMouseButton(Atomy::Mouse::MouseButton::LeftButton);
    int x, y;
    Atomy::Mouse::getMousePosition(x, y);
    std::cout << "Mouse at: " << x << ", " << y << std::endl;
    Atomy::General::sleep(1, "s");
    Atomy::Monitor::takeScreenshot("shot.png");
    return 0;
}
```

---

## License

The library is free and open-source, available under the **MIT** license.