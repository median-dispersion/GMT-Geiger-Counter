# Converting a GIMP image to C source code

The files contained in this directory are the graphics used by the main firmware, saved as GIMP projects. To render images using a microcontroller like the ESP32 S3 directly without external storage, they first need to be converted into a format where they can be included as program code. These are the steps using GIMP to convert the Geiger counter graphics to C source code so that they can be embedded into the program code.

## 🛫 Exporting an image

1. Open a GIMP project
2. File > Export As
3. In the export file dialog: Select File Type > C source code (*.c)
4. Press Export
5. In the Export Image as C-Source dialog: Type in a name for the variable
6. Uncheck everything except: Save as RGB565 (16-bit)
7. Set opacity to 100%
8. Press Export

## 📝 Modifying the export

Modify the exported `.c` file in such a way that the result looks like this:

```c++
#include "Graphics.h"

const uint8_t data[] = {

    /* IMAGE DATA */

};

Image NAME_OF_IMAGE = {WIDTH_PIXEL, HEIGHT_PIXEL, (const uint16_t*)(data)};
```

Afterward, rename the file to `.cpp` to make it a C++ source file. For a real example, look at any of the `Image*.cpp` files in the main firmware directory.

## ⏬ Importing an image

Include the image source code into the project by adding this to the `Graphics.h` file:

```c++
extern Image NAME_OF_IMAGE;
```