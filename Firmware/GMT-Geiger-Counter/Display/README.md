# Converting a GIMP image to C source code

To render images using a microcontroller like the ESP32 S3 directly without external storage, they first need to be converted into a format where they can be included as program code. These are the steps using GIMP to convert the Geiger Counter displays to C source code so that they can be embedded into the program code.

## 🛫 Exporting an image

1. Open a GIMP project
2. File > Export As
3. In the export file dialog: Select File Type > C source code (*.c)
4. Press Export
5. In the Export Image as C-Source dialog: Type in a name for the variable
6. Uncheck everything except: Save as RGB565 (16-bit)
7. Set opacity to 100%
8. Press Export

## ⏬ Importing an image

Include the exported C source code into the project with:

```c++
#include "NameOfFile.c"
```

Draw the image using the Adafruit GFX library with:

```c++
canvas.drawRGBBitmap(0, 0, (const uint16_t *) NameOfVariable.pixel_data, NameOfVariable.width, NameOfVariable.height);
```